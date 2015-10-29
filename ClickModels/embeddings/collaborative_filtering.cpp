#include "collaborative_filtering.h"

double similarity(const std::vector<double>& x,const std::vector<double>& y)
{
    double res = 0;
    bool cout_ = false;
    for (size_t i = 0; i < x.size(); ++i)
    {
        res += (x[i] - y[i]) * (x[i] - y[i]);
        if (x[i] != x[i]) cout_ = true;
        if (y[i] != y[i]) cout_ = true;
    }
    if (cout_)
    {
        std::cout << "1\n";
        for (int i = 0; i < x.size(); ++i)
            std::cout << x[i] << " " << y[i] << " ";
        std::cout << "\n";
        std::exit(0);
    }
    return std::exp(-std::sqrt(res));

}

vector<double> divSimilarity(const std::vector<double>& x,const std::vector<double>& y)
{
    double coef = 0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        coef += (x[i] - y[i]) * (x[i] - y[i]);
    }
    double coef1 = coef;
    coef = std::sqrt(coef);
    double coef2 = coef;
    coef = std::exp(-coef) * (-1./coef);
    double coef3 = coef;
    if (abs(coef) > 1e+10 || coef != coef)
    {
        std::cout << coef << " " << coef1 << " " << coef2 <<  " " << coef3 << "\n";
        std::exit(1);
    }
    std::vector<double> res_v(x.size(), 0.);
    bool cout_ = false;
    for (size_t i = 0; i < x.size(); ++i)
    {
        res_v[i] = (x[i] - y[i]) * coef;
        if (res_v[i] != res_v[i])
        {
            cout_ = true;
        }

    }
    if (cout_)
    {
        std::cout << "2\n " << coef;
        std::exit(1);
    }
    return res_v;
}

collaborative_filtering::collaborative_filtering(double rate_, int dim_, const string& usersFile):
    rate(rate_), dim(dim_), f(similarity, divSimilarity)
{
    int enumerator = 0;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    vector<size_t> users;
    FileManager::Read(usersFile, &users);
    for (size_t user : users)
    {
        if (++enumerator % 100000 == 0)
        {
            std::cout << "MyLearner reading file " << usersFile <<
                "; iteration = " << enumerator << std::endl;
        }
        embedding[user] = std::vector<double> (dim, 0.);
        for (double& item : embedding[user])
        {
            item = distribution(generator);
        }
    }
}

void summ(std::vector<double>& x, const std::vector<double>&y, double rate)
{
    for (size_t i = 0; i < x.size(); ++i)
    {
        x[i] += y[i]*rate;
    }
}

std::vector<double> collaborative_filtering::res_one_position(const std::vector<Example>& examples, size_t user) const
{
    std::vector<double> res(10, 0.1);
    for (size_t i = 0; i < examples.size(); ++i)
    {
        res[examples[i].rang_click_document] +=
                f.similarity(embedding.at(user), embedding.at(examples[i].user));
    }
    return res;
}

void collaborative_filtering::One_step(const std::vector<Example>& examples, 
                                       const std::vector<bool>& truth,
                                       size_t user)
{
    std::vector<double> new_vector_for_user(10,0.);
    std:vector<double> position_distribution = res_one_position(examples, user);
//    for (int i = 0; i < 10; ++i)
//    {
//        std::cout << position_distribution[i] << " ";
//    }
//    std::cout << "\n";

    double sum_all = std::accumulate(position_distribution.begin(), position_distribution.end(), 0.);
    for (size_t i = 0; i < examples.size(); ++i)
    {
        double coeff = -10./ sum_all;
        for (size_t rang = 0; rang < 10; ++rang)
        {
            if (rang == examples[i].rang_click_document)
            {
                if (truth[rang])
                {
                    coeff += 1./ (position_distribution[rang] + 0.1);
                    if (coeff != coeff)
                    {
                        std::cout << coeff << " " << position_distribution[rang] << "POSITION" << " ";
                        std::exit(0);
                    }
                }
            }
            else
            {
                if (!truth[rang])
                {
                    coeff += 1/(sum_all - position_distribution[rang] + 0.1);
                    if (coeff != coeff)
                    {
                       std::cout << coeff << " " << position_distribution[rang] << "POSITION" << " ";
                       std::exit(0);
                    }
                }
            }
        }
        summ(embedding[examples[i].user], f.divSimilarity(embedding[examples[i].user], embedding[user]), coeff * rate);
        summ(new_vector_for_user, f.divSimilarity(embedding[user], embedding[examples[i].user]), coeff * rate);
    }
    summ(embedding[user], new_vector_for_user, 1.);
}

void collaborative_filtering::Learn(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, DayData& dayData)
{
    std::cout << "Run Learn" << endl;
    size_t numPlus = 0;
    size_t numMinus = 0;
    size_t enumerator = 0;
    clock_t start = clock();
    for(const auto& item0 : dayData)
    {
        for (const auto& item1 : item0.second)
        {
            std::vector<bool> truth(10, false);

            const Query& history = item1.second;
            if (++enumerator % 10000 == 0)
            {
                double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                std::cout << elapsedTime << ": " << enumerator << " " << numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << std::endl;
            }
            // get query, person and clicked url
            size_t query = history.id;
            size_t user = history.person;
            size_t url = -1;

            //first filter
            const vector<double>& rank0 = queryRank.watch(query, 0);
            const vector<double>& rank1 = queryRank.watch(query, 1);
            if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
            {
                continue;
            }

            //second filter
            bool should_break = false;
            for (size_t i = 0; i < 10; ++i)
            {
                const vector<double>& found = userUrl.watch(user, history.urls[i]);
                if (found.size() > 0 && found[0] > 1 - 1e-5)
                {
                    should_break = true;
                    break;
                }
            }
            if (should_break) continue;

            //third_filter
            bool found = false;
            for (size_t i = 0; i < 10; ++i)
            {
                if(history.type[i] == 2)
                {
                    found = true;
                    truth[i] = true;
                }
            }
            if (!found) continue;

            //forth filter
            if (queryUser.watch(query).size() < 10) continue;
            const unordered_map<size_t, vector<double> >& users = queryUser.watch(query);

            //Statistics
            std::vector<Example> examples;
            for (auto& item : users)
            {

                size_t similarUserUrl = -1;
                size_t similarUser = item.first;
                if (similarUser == user) continue;
                if (item.second.size() == 0) continue;
                bool found = false;
                for (size_t i = 0; i < 10; ++i)
                {
                    const vector<double>& urls = userUrl.watch(similarUser, history.urls[i]);
                    if (urls.size() > 0)
                    {
                        found = true;
                        similarUserUrl = i;
                        break;
                    }
                }
                if (!found) continue;
                if (user == similarUser) continue;
                examples.push_back(Example(similarUser, similarUserUrl));
                if (history.type[similarUserUrl] == 2) {
                    ++numPlus;

                } else {
                    ++numMinus;
                }
            }
            One_step(examples, truth, user);
        }
    }
    std::cout << "Ready!!!" << std::endl;
}

void collaborative_filtering::Print(const string& file) const
{
    unordered_map<size_t, size_t> usersToIndex;
    vector<double> concatenated(dim * embedding.size());
    int enumerator = 0;
    for (auto it = embedding.begin(); it != embedding.end(); ++it)
    {
        const vector<double>& emb = it->second;
        size_t position = enumerator * dim;
        for (size_t j = 0; j < dim; ++j)
        {
            concatenated[position + j] = emb[j];
        }
        usersToIndex[it->first] = enumerator;
        ++enumerator;
    }
    FILE* writer = fopen((file + ".embedding").c_str(), "wb");
    fwrite(&(*concatenated.begin()), sizeof(double), concatenated.size(), writer);
    fclose(writer);
    FileManager::Write(file + ".map", usersToIndex);
}
