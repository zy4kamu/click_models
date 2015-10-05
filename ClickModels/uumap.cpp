#include "uumap.h"

uumap::uumap()
{
}

uumap::uumap(const string& file)
{
    ifstream in(file);
    if (!in.is_open())
    {
        std::cout << "couldn't read file " << file << std::endl;
        return;
    }
    std::cout << "start reading file " << file << std::endl;
    size_t size, numFeat;
    in >> size >> numFeat;
    for (size_t i = 0; i < size; ++i)
    {
        size_t first, second;
        in >> first >> second;
        vector<double>& features = this->get(first, second, 0);
        features.resize(numFeat);
        for (size_t j = 0; j < numFeat; ++j) in >> features[j];
    }
    in.close();
    std::cout << "finished reading file " << file << std::endl;
}

void uumap::save(const string& file)
{
    ofstream out(file);
    std::cout << "start writing to file " << file << std::endl;
    size_t size = 0;
    for(const auto& item0 : data)
    {
        size += item0.second.size();
    }
    out << size << " " << data.begin()->second.begin()->second.size() << std::endl;
    for(const auto& item0 : data)
    {
        for (const auto& item1 : item0.second)
        {
            out << item0.first << " " << item1.first;
            for (double feature : item1.second)
            {
                out << " " << feature;
            }
            out << std::endl;
        }
    }
    out.close();
    std::cout << "finished writing to file " << file << std::endl;
}

vector<double>& uumap::get(size_t key1, size_t key2, size_t default_size = 0)
{
    unordered_map<size_t, vector<double> >& found = take(this->data, key1);
    return take(found, key2, vector<double>(default_size, 0));
}

const vector<double>& uumap::watch(size_t key1, size_t key2) const
{
    static const vector<double> default_vec;
    auto found = this->data.find(key1);
    if (found == this->data.end()) {
        return default_vec;
    } else {
        auto found1 = found->second.find(key2);
        if (found1 == found->second.end()) {
            return default_vec;
        } else {
            return found1->second;
        }
    }
}

const unordered_map<size_t, vector<double> >& uumap::watch(size_t key1) const
{
    static const unordered_map<size_t, vector<double> > default_map;
    auto found = this->data.find(key1);
    if (found == this->data.end()) {
        return default_map;
    } else {
        return found->second;
    }
}

void uumap::clear()
{
    this->data.clear();
}
