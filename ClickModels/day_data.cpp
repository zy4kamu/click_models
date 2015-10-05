#include "day_data.h"

void separate_by_day(const string& file, const string& outFolder)
{
    vector<ofstream*> outs;
    for (size_t i = 0; i < 30; ++i)
    {
        string toOpen = outFolder + std::to_string(i) + ".txt";
        outs.emplace_back(new ofstream(toOpen, std::ios::binary | std::ios::out));
    }
    ifstream input(file);
    size_t person, session, query, day, url, rank;
    int type;
    size_t enumerator = 0;
    while(!input.eof())
    {
        input >> person >> session >> query >> day >> url >> type >> rank;
        ofstream* out = outs[day];
        out->write((char*)&person, sizeof(size_t));
        out->write((char*)&session, sizeof(size_t));
        out->write((char*)&query, sizeof(size_t));
        out->write((char*)&url, sizeof(size_t));
        out->write((char*)&type, sizeof(int));
        out->write((char*)&rank, sizeof(size_t));
        if (++enumerator % 100000 == 0) {
            std::cout << enumerator << std::endl;
        }
    }
    input.close();
    for (size_t i = 0; i < 30; ++i)
    {
        outs[i]->close();
        delete outs[i];
    }
}

DayData read_day(const string& file)
{
    std::cout << "reading file " << file << std::endl;
    DayData data;
    ifstream input(file, std::ios::binary | std::ios::in);
    size_t person, session, query, url, rank;
    int type;
    size_t enumerator = 0;
    while(!input.eof())
    {
        if (++enumerator % 1000000 == 0) {
            std::cout << "read day from file " << file << " ; iteration = " << enumerator << std::endl;
        }
        input.read((char*)&person, sizeof(size_t));
        input.read((char*)&session, sizeof(size_t));
        input.read((char*)&query, sizeof(size_t));
        input.read((char*)&url, sizeof(size_t));
        input.read((char*)&type, sizeof(int));
        input.read((char*)&rank, sizeof(size_t));
        Query& addedQuery = take(take(data, person), session);
        addedQuery.id = query;
        addedQuery.person = person;
        addedQuery.urls[rank] = url;
        addedQuery.type[rank] = type;
    }
    input.close();
    return data;
}

