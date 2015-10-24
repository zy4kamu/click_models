#include "personalizedclickmodel.h"


double Scalar_product(const std::vector<double>& x, const std::vector<double>& y)
{
    size_t dim = x.size();
    double res = 0;
    for (int i = 0; i < dim; ++i)
    {
        res += x[i]*y[i];
    }
    return res;
}

vector<double> GenerateRandomVector(int dim,
                                    std::uniform_real_distribution<double>& uniformDistribution)
{
    std::vector<double> result(dim, 0);
    std::default_random_engine generator;
    for (int i = 0; i < dim; ++i)
    {
        result[i] = uniformDistribution(generator);
    }
    return result;
}

void Normalize_the_vector(std::vector<double>& v)
{
    size_t v_len = v.size();
    double s = 0;
    double min_value = v[0];
    for (int i = 0; i < v_len; ++i)
    {
        s += v[i];
        if (min_value > v[i]) min_value = v[i];
    }
    for (int i = 0; i < v_len; ++i)
    {
        v[i] = (v[i] - min_value) / (s - v_len * min_value);
    }

}

PersonalizedClickModel::PersonalizedClickModel(int d, int e_day):dim(d), examination(10,0), end_day(e_day)
{
    for (int i = 0; i < 10; ++i)
    {
        examination[i] = 1./ (i+1);
    }
    Normalize_the_vector(examination);
    sigma_u  = 1.;
    sigma_q = 1.;
    sigma_d = 1.;
    sigma = 1.;
    size_t enumerator = 0;
    clock_t start = clock();
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/data_by_days/";
    for (int i = 1; i <= end_day; ++i)
    {
        DayData dayData = read_day(out_directory + std::to_string(i) + ".txt");
        for(const auto& item0 : dayData)
        {
            for (const auto& item1 : item0.second)
            {
                const Query& history = item1.second;
                if (++enumerator % 10000 == 0)
                {
                    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                    std::cout << elapsedTime << ": " << enumerator << std::endl;
                }
                    // get query, person and clicked url
                size_t query = history.id;
                size_t user = history.person;
                std::uniform_real_distribution<double> uniformDistribution(0,1);
                std::default_random_engine generator;
                queries[query] = GenerateRandomVector(dim, uniformDistribution);
                Normalize_the_vector(queries[query]);
                users[user] = GenerateRandomVector(dim, uniformDistribution);
                Normalize_the_vector(users[user]);
                for (int i = 0; i < 10; ++i)
                {
                    size_t url = history.urls[i];
                    documents[url] = GenerateRandomVector(dim, uniformDistribution);
                    Normalize_the_vector(documents[url]);
                    attractivness_u_q_d[user][query][url] = uniformDistribution(generator);
                }
            }
        }

     }
 }

double PersonalizedClickModel::Attractivness(size_t user, size_t document, size_t query)
{
    double attractivness = 0;
    const std::vector<double>& user_vector = users[user];
    const std::vector<double>& query_vector = queries[query];
    const std::vector<double>& document_vector = documents[document];
    for (int d = 0; d < dim; ++d)
    {
        attractivness += user_vector[d] * query_vector[d] * document_vector[d];
    }
    return attractivness;
}

double PersonalizedClickModel::Probability_irrelevant(size_t user, size_t document, size_t query, size_t rank)
{
    double attractivness = Attractivness(user, document, query);
    return examination[rank] * (1. - attractivness) / (1 - attractivness * examination[rank] + 1e-10);
}

void PersonalizedClickModel::Update(const std::string& pathToDayData, double step)
{
    DayData dayData = read_day(pathToDayData);
    size_t enumerator = 0;
    clock_t start = clock();
    for(const auto& item0 : dayData)
    {
        for (const auto& item1 : item0.second)
        {
            const Query& history = item1.second;
            if (enumerator++ % 10000 == 0)
            {
                double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                std::cout << elapsedTime << ": " << enumerator << std::endl;
            }
            // get query, person and clicked url
            size_t query = history.id;
            size_t user = history.person;
            for (int i = 0; i < 10; ++i)
            {
                size_t url = history.urls[i];
                int type = history.type[i];
                double attractivness = Attractivness(user, url,query);
                double sigma_sq = 1./(sigma*sigma);
                double betta_grad = 1./(examination[i]+1e-10);
                double alpha_grad = 1./(attractivness_u_q_d[user][query][url]+1e-10);
                alpha_grad -= sigma_sq * (attractivness_u_q_d[user][query][url] - attractivness);
                std::vector<double> u_grad(dim, 0.);

                std::vector<double> q_grad(dim, 0.);
                std::vector<double> d_grad(dim, 0.);
                for (int d = 0; d < dim; ++d)
                {
                    u_grad[d] += sigma_sq * (attractivness_u_q_d[user][query][url] - attractivness) *
                                queries[query][d] * documents[url][d] - (1./(sigma_u * sigma_u)) * users[user][d];
                    q_grad[d] += sigma_sq * (attractivness_u_q_d[user][query][url] - attractivness) *
                                users[user][d] * documents[url][d] - (1./(sigma_q * sigma_q)) * queries[query][d];
                    d_grad[d] += sigma_sq * (attractivness_u_q_d[user][query][url] - attractivness) *
                                users[user][d] * queries[query][d] - (1./(sigma_d * sigma_d)) * documents[url][d];
                }
                if (history.type[i] != 2)
                {
                    double irrelevant = Probability_irrelevant(user, url, query, i);

                    betta_grad = betta_grad * irrelevant - (1. - irrelevant)/ (1-examination[i] + 1e-10);
                    alpha_grad *= irrelevant;
                    for (int d = 0; d < dim; ++d)
                    {
                        u_grad[d] *= irrelevant;
                        q_grad[d] *= irrelevant;
                        d_grad[d] *= irrelevant;
                    }

                }
                examination[i] += step * betta_grad;
                attractivness_u_q_d[user][query][url] += step * alpha_grad;
                for (int d = 0; d < dim; ++d)
                {
                    users[user][d] += step * u_grad[d];
                    queries[query][d] += step * q_grad[d];
                    documents[url][d] += step * d_grad[d];
                }
                Normalize_the_vector(examination);
                Normalize_the_vector(users[user]);
                Normalize_the_vector(queries[query]);
                Normalize_the_vector(documents[url]);

            }
        }
    }
    std::cout << "Ready!!!" << std::endl;
}

void PersonalizedClickModel::Learn(double step)
{
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/data_by_days/";
    for (int i = 1; i < end_day; ++i)
    {
        std::cout << "Get day " << i <<std::endl;
        Update(out_directory + std::to_string(i) + ".txt", 0.1);
        for (int r = 0; r < 10; ++r)
        {
            std::cout << examination[r] << " ";
        }
        std::cout << std::endl;
    }
}

void PersonalizedClickModel::Test()
{

    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    uumap queryUser(out_directory + "query_user_1_10");
    uumap userUrl(out_directory + "user_url_1_10");
    uumap queryRank(out_directory + "query_rank_1_10");
    DayData dayData = read_day(out_directory + "data_by_days/" + std::to_string(end_day) + ".txt");

    size_t enumerator = 0;
    size_t numPlus = 0;
    size_t numMinus = 0;
    size_t numBasicPlus = 0;
    size_t numBasicMinus = 0;
    size_t numOnFirst = 0;
    size_t numNOotOnFirst = 0;
    clock_t start = clock();
    //bool shouldStop = false;
    for(const auto& item0 : dayData)
    {
        for (const auto& item1 : item0.second)
        {
            const Query& history = item1.second;
            if (enumerator++ % 1000 == 0) {
                double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                std::cout << elapsedTime << ": " << enumerator << " " << "BASIC "
                          << numBasicPlus << " " << numBasicMinus << " " << (double)numBasicPlus / (numBasicPlus + numBasicMinus)
                          << " EMBEDDING " <<numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << ";"
                          << numOnFirst << " " << numNOotOnFirst << std::endl;
            }
            // get query, person and clicked url
            size_t query = history.id;
            size_t user = history.person;
            size_t url = -1;

            const vector<double>& rank0 = queryRank.watch(query, 0);
            const vector<double>& rank1 = queryRank.watch(query, 1);
            /*if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
            {
                continue;
            }*/

            /*bool should_break = false;
            for (size_t i = 0; i < 10; ++i)
            {
                const vector<double>& found = userUrl.watch(user, history.urls[i]);
                if (found.size() > 0 && found[0] > 1 - 1e-5)
                {
                    should_break = true;
                    break;
                }
            }
            if (should_break) continue;*/

            //if (queryUser.watch(query).size() < 10) continue;

            bool found = false;
            for (size_t i = 0; i < 10; ++i)
            {
                if(history.type[i] == 2)
                {
                    found = true;
                    url = history.urls[i];
                    break;
                }
            }
            if (!found) continue;

            vector<double> evristics(10,0);
            for (int i = 0; i < 10; ++i)
            {
                //std::cout << "AAAAA " << i << std::endl;
                size_t url = history.urls[i];
                evristics[i] = Attractivness(user,url,query) * 1./(i+2); //examination[i];
            }


            size_t clickedBestRank = 0;
            double maxEvristics = -1000000;
            for (size_t i = 0; i < 10; ++i)
            {
                if (evristics[i] > maxEvristics)
                {
                    clickedBestRank = i;
                    maxEvristics = evristics[i];
                }
            }
            if (clickedBestRank > 1) clickedBestRank = 0;
            //std::cout << clickedBestRank << std::endl;
            if (history.type[clickedBestRank] == 2) {
                ++numPlus;
            } else {
                ++numMinus;
            }
            if (clickedBestRank == 0) numOnFirst++;
            else ++numNOotOnFirst;

            if (history.type[0] == 2) {
                ++numBasicPlus;
            } else {
                ++numBasicMinus;
            }
        }
    }
    std::cout << "Ready!!!" << std::endl;

}
