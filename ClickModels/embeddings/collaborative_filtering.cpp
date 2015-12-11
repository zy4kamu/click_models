#include "collaborative_filtering.h"
#include <chrono>
#include <thread>
#include <algorithm>

static string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
//static string out_directory = "/home/stepan/click_models_data/";

size_t numThreads = 4;
size_t numThreads_learning = 4;
vector<double> ScalarProduct(const std::vector<double>& x, const std::vector<double>& y)
{
    vector<double> res(10, 0.);
    for (int i = 0; i < x.size(); ++i)
    {
        res[i] = x[i] * y[i];
    }
    return res;
}

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
    return std::exp(-std::sqrt(res));

}

template <typename T>
void clear_vector(std::vector<T*>& results)
{
    for (auto it = results.begin(); it < results.end(); ++it)
    {
        delete *it;
    }
    results.clear();
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
    if (std::abs(coef) > 1e+10 || coef != coef)
    {
        std::cout << coef << " " << coef1 << " " << coef2 <<  " " << coef3 << "\n";
        std::exit(1);
    }
    std::vector<double> res_v(x.size(), 0.);
    bool cout_ = false;
    for (size_t i = 0; i < x.size(); ++i)
    {
        res_v[i] = (x[i] - y[i]) * coef;

    }
    return res_v;
}
//-------------------------------------------------------
void Get_documents()
{
    std::unordered_set<size_t> documents;
    for (int i = 1; i < 4; ++i)
    {
       DayData day = read_day(out_directory +"data_by_days/" +std::to_string(i) + ".txt");
       for(const auto& item0 : day)
       {
           for (const auto& item1 : item0.second)
           {
               auto history = item1.second;
               for (int j = 0; j < 10; ++j)
               {
                   documents.insert(history.urls[j]);
               }
           }
       }

    }
    std::ofstream d(out_directory + "initial_model_parameters_3/documents");
    for (auto it = documents.begin(); it != documents.end(); ++it)
    {
        d << *(it) << "\n";
    }
    d.close();
}

void Get_query()
{
    std::unordered_set<size_t> queries;
    for (int i = 1; i < 4; ++i)
    {
       DayData day = read_day(out_directory +"data_by_days/" +std::to_string(i) + ".txt");
       for(const auto& item0 : day)
       {
           for (const auto& item1 : item0.second)
           {
               auto history = item1.second;
               for (int j = 0; j < 10; ++j)
               {
                   queries.insert(history.id);
               }
           }
       }

    }
    std::ofstream d(out_directory + "initial_model_parameters_3/queries");
    for (auto it = queries.begin(); it != queries.end(); ++it)
    {
        d << *(it) << "\n";
    }
    d.close();
}

void Get_users()
{
    std::unordered_set<size_t>users;
    for (int i = 1; i < 4; ++i)
    {
       DayData day = read_day(out_directory +"data_by_days/" +std::to_string(i) + ".txt");
       for(const auto& item0 : day)
       {
           for (const auto& item1 : item0.second)
           {
               auto history = item1.second;
               for (int j = 0; j < 10; ++j)
               {
                   users.insert(history.person);
               }
           }
       }

    }
    std::ofstream d(out_directory + "initial_model_parameters_3/users");
    for (auto it = users.begin(); it != users.end(); ++it)
    {
        d << *(it) << "\n";
    }
    d.close();
}

std::vector<double> GetQueryCtr(const uumap& queryRank, size_t query)
{
    std::vector<double> res(10, 0.);
    for (int j = 0; j < 10; ++j)
    {
        const vector<double>& rank0 = queryRank.watch(query, j);
        double r = 1;
        if (rank0.size() > 0)
        {
            r += rank0[0];
        }
        res[j] += r;
    }

    return res;
}

double Cost(double x, double y)
{
    double del = 1./ (1 + std::exp(-(x-y)));
    if (del < 1e-5) del = 1e-5;
    return std::log(del);
}

double DivCost(double x, double y)
{
    return -1./ (1 + std::exp(-(x-y)));
}
//-------------------------------------------------------
collaborative_filtering::collaborative_filtering(double rate_, int dim_, const string& usersFile,
                                                 const string& queriesFile, const std::string& result_file,
                                                 int min_n, int max_n):
    rate(rate_), dim(dim_), f(similarity, divSimilarity), min_number(min_n), max_number(max_n)
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

        user_query_embedding[user] = std::vector<double> (dim, 0.);
        for (double& item : user_query_embedding[user])
        {
            item = distribution(generator);
        }

    }

    enumerator = 0;
//    vector<size_t> documents;
//    FileManager::Read(documentsFile, &documents);
//    for (size_t document : documents)
//    {
//        if (++enumerator % 1000000 == 0)
//        {
//            std::cout << "MyLearner reading file " << documentsFile <<
//                "; iteration = " << enumerator << std::endl;
//        }
//        document_embedding[document] = std::vector<double> (dim, 0.);
//        for (double& item : document_embedding[document])
//        {
//            item = distribution(generator);
//        }
//    }

    vector<size_t> queries;
    FileManager::Read(queriesFile, &queries);
    for (size_t query : queries)
    {
        if (++enumerator % 1000000 == 0)
        {
            std::cout << "MyLearner reading file " << queriesFile <<
                "; iteration = " << enumerator << std::endl;
        }
        query_embedding[query] = std::vector<double> (11, 0.);
    }
    res_file.open(result_file);
    //res_file.open(out_directory + "data_stat/histogramms/result.txt");
}

void summ(VectorWithLoc& x, const std::vector<double>&y, double rate)
{
    std::lock_guard<std::mutex> lock(x.m);
    for (size_t i = 0; i < x.v.size(); ++i)
    {
        x.v[i] += y[i]*rate;
    }
}

vector<Query> DayDataToVec(const DayData& day)
{
    std::cout << "Run DayData to Vec\n";
    std::vector<Query> data;
    for(const auto& item0 : day)
    {
        for (const auto& item1 : item0.second)
        {
            data.push_back(item1.second);
        }
    }
    std::cout << "End DayData to Vec\n";
    return data;
}

double collaborative_filtering::user_user_query_similarity(int user1, int user2, int query)
{
    double res = 0;
    for (int i = 0; i < dim; ++i)
    {
        res += user_query_embedding[user1][i] * user_query_embedding[user2][i] * query_embedding[query][i];
    }
    return res;
}

std::vector<double> collaborative_filtering::res_one_position(const std::vector<Example>& examples, const std::vector<double>& user)
{
    std::vector<double> res(10, 1e-5);
    for (size_t i = 0; i < examples.size(); ++i)
    {
        res[examples[i].rang_click_document] +=
                f.similarity(user, examples[i].user);
    }
    return res;
}

std::vector<double> collaborative_filtering::res_one_position(const std::vector<Example>& examples, const std::vector<double>& user,
                                                              int user_id, int query)
{
    std::vector<double> res(10, 1e-5);
    for (size_t i = 0; i < examples.size(); ++i)
    {
        res[examples[i].rang_click_document] +=
                /*f.similarity(user, examples[i].user) **/
                user_user_query_similarity(user_id, examples[i].user_id, query);
    }
    return res;
}

static size_t enumerator = 0;
//static size_t correct_answers = 0;

double LogLikelihood(const std::vector<bool>& truth, const std::vector<double>& position_distribution, double sum)
{
    double res = 0;
    for (size_t i = 0; i < truth.size(); ++i)
    {
        if (truth[i])
        {
            res += std::log(position_distribution[i]/sum);
        }
        else
        {
            res += std::log(1 - position_distribution[i]/sum);
        }
    }
    return res;

}



int Correct_answer(const std::vector<double>& stat, const std::vector<bool>& truth)
{
   double max_stat = stat[0]-1;
   int res = 0;
   for (int i = 0; i < 10; ++i)
   {
       if (stat[i] > max_stat)
       {
           max_stat = stat[i];
           res = truth[i];
       }
   }
   return res;
}

std::vector<double> GetCoeffs(const std::vector<double>& position_distribution,
                              const std::vector<bool>& truth)
{
    std::vector<double> coeffs(10, 0.);
    //correct_answers += Correct_answer(position_distribution, truth);
    double sum_all = std::accumulate(position_distribution.begin(), position_distribution.end(), 0.);
    for (int rang = 0; rang < 10; ++rang)
    {
        int rang_truth = truth[rang];
        for (int rang1 = rang+1; rang1 < 10; ++rang1)
        {
            int rang1_truth = truth[rang1];
            if (rang_truth != rang1_truth)
            {
                double d_cost = DivCost(position_distribution[rang] / sum_all, position_distribution[rang1] / sum_all);
                double rang_coeff  = (sum_all - position_distribution[rang]) / (sum_all * sum_all);
                rang_coeff += position_distribution[rang1] / (sum_all * sum_all);
                double rang1_coeff = (sum_all - position_distribution[rang1]) / (sum_all * sum_all);
                rang_coeff += position_distribution[rang] / (sum_all * sum_all);
                if (rang_truth > rang1_truth)
                {
                    coeffs[rang] += d_cost * rang_coeff;
                    coeffs[rang1] += d_cost * rang1_coeff;
                }
                else
                {
                    coeffs[rang] -= d_cost * rang_coeff;
                    coeffs[rang1] -= d_cost * rang1_coeff;
                }

            }
        }
    }
    return coeffs;
}

void collaborative_filtering::One_step(const std::vector<Example>& examples,
                                       const std::vector<bool>& truth,
                                       std::vector<double>& user,
                                       int user_id,
                                       int query)
{
    ++enumerator;
    std::vector<double> new_vector_for_user(dim, 0.);
    std:vector<double> position_distribution(10, 0.);
    position_distribution = res_one_position(examples, user, user_id, query);
    std::vector<double> coeffs = GetCoeffs(position_distribution, truth);

    std::vector<double> new_vector_for_user_query(dim, 0);
    std::vector<double> new_vector_query(dim, 0);
    for (size_t i = 0; i < examples.size(); ++i)
    {

//        VectorWithLoc new_vector_for_user_(new_vector_for_user);
//        VectorWithLoc user_embedding(examples[i].user);
//        summ(new_vector_for_user_, f.divSimilarity(user, examples[i].user),
//             coeffs[examples[i].rang_click_document] * rate *
//                user_user_query_similarity(user_id, examples[i].user_id, query));
//        summ(user_embedding, f.divSimilarity(examples[i].user, user),
//                coeffs[examples[i].rang_click_document] * rate *
//                user_user_query_similarity(user_id, examples[i].user_id, query));
        vector<double> user_user = ScalarProduct(user_query_embedding[user_id],
                                                 user_query_embedding[examples[i].user_id]);
        vector<double> user_example_query = ScalarProduct(query_embedding[query],
                                                 user_query_embedding[examples[i].user_id]);
        vector<double> user_query = ScalarProduct(query_embedding[query],
                                                  user_query_embedding[user_id]);
        double user_user_sim = 1 ; //f.similarity(user, examples[i].user);
        VectorWithLoc new_vector_for_user_query_(new_vector_for_user_query);
        summ(new_vector_for_user_query_, user_example_query,
             coeffs[examples[i].rang_click_document] * rate * user_user_sim);
        VectorWithLoc user_query_(user_query_embedding[examples[i].user_id]);
        summ(user_query_, user_query,
             coeffs[examples[i].rang_click_document] * rate * user_user_sim);
        VectorWithLoc new_vector_query_(new_vector_query);
        summ(new_vector_query_, user_user,
             coeffs[examples[i].rang_click_document] * rate * user_user_sim);

    }

//    VectorWithLoc user_embedding(user);
//    summ(user_embedding, new_vector_for_user, 1.);
    //std::cout << "1 ";
    VectorWithLoc user_query_(user_query_embedding[user_id]);
    summ(user_query_, new_vector_for_user_query, 1.);
    //std::cout << "2 ";
    VectorWithLoc query_(query_embedding[query]);
    summ(query_, new_vector_query, 1.);
    //std::cout << "3\n";


}
void collaborative_filtering::One_step(const std::vector<Example>& examples,
                                       const std::vector<bool>& truth,
                                       std::vector<double>& user)
{
    ++enumerator;
    std::vector<double> new_vector_for_user(dim, 0.);
    std:vector<double> position_distribution(10, 0.);
    position_distribution = res_one_position(examples, user);
    std::vector<double> coeffs = GetCoeffs(position_distribution, truth);

    std::vector<double> new_vector_for_user_query(dim, 0);
    std::vector<double> new_vector_query(dim, 0);
    for (size_t i = 0; i < examples.size(); ++i)
    {
        VectorWithLoc new_vector_for_user_(new_vector_for_user);
        VectorWithLoc user_embedding(examples[i].user);
        summ(new_vector_for_user_, f.divSimilarity(user, examples[i].user),
             coeffs[examples[i].rang_click_document] * rate);
        summ(user_embedding, f.divSimilarity(examples[i].user, user),
                coeffs[examples[i].rang_click_document] * rate);

    }

    VectorWithLoc user_embedding(user);
    summ(user_embedding, new_vector_for_user, 1.);
}

void collaborative_filtering::LearnOneEx1(const std::vector<Query>& dayDataVec, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                         size_t coreIndex, size_t numCores)
{
    for (size_t i = coreIndex; i < dayDataVec.size(); i += numCores)
    {
        LearnOneEx(dayDataVec[i], queryUser, userUrl, queryRank);
        //LearnOneExForDocuments(dayDataVec[i], queryUser, userUrl, queryRank);
    }
}

void collaborative_filtering::LearnOneEx(const Query& history, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank)
{

    std::vector<bool> truth(10, false);
    // get query, person and clicked url
    size_t query = history.id;
    size_t user = history.person;
    size_t url = -1;

    //first filter
    for (int i = 0; i < 10; ++i)
    {
        if (history.type[i] == 2)
        {
            query_embedding[query][i] += 1;
            break;
        }
    }
    query_embedding[query][10] += 1;


    if (!GetFilter(queryUser, userUrl, queryRank, history)) return;

    for (size_t i = 0; i < 10; ++i)
    {
        if(history.type[i] == 2)
        {
            truth[i] = true;
        }
    }

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
        examples.push_back(Example(embedding[similarUser], similarUserUrl, similarUser));
    }
    std::vector<double> queryCtr = GetQueryCtr(queryRank, query);
    One_step(examples, truth, embedding[user]);

}

void collaborative_filtering::LearnOneExForDocuments(const Query& history, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank)
{

//    std::vector<bool> truth(10, false);
//    // get query, person and clicked url
//    size_t query = history.id;
//    size_t user = history.person;
//    size_t url = -1;

//    if (!GetFilter(queryUser, userUrl, queryRank, history)) return;
//    for (size_t i = 0; i < 10; ++i)
//    {
//        if(history.type[i] == 2)
//        {
//            truth[i] = true;
//        }
//    }

//     //Statistics
//     std::vector<Example> examples;
//     for (int i = 0; i < 10; ++i)
//     {
//         examples.push_back(Example(document_embedding[history.urls[i]], i));
//     }
//     std::vector<double> queryCtr = GetQueryCtr(queryRank, query);
//     One_step(examples, truth, embedding[user]);


}


void collaborative_filtering::Learn(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const DayData& dayData)
{
    clock_t start = clock();
    std::vector<Query> dayDataVec = DayDataToVec(dayData);
    std::vector<std::thread> threads;
    //size_t numThreads = 8;
    for (size_t i = 0; i < numThreads_learning; ++i)
    {
          std::thread t(&collaborative_filtering::LearnOneEx1, this, std::ref(dayDataVec), std::ref(queryUser), std::ref(userUrl),std::ref(queryRank),
                                                                i, numThreads_learning);
          threads.push_back(std::move(t));
    }

    for (size_t i = 0; i < numThreads_learning; ++i)
    {
                    threads[i].join();
    }

    std::cout << "Ready!!!" << std::endl;
}

void collaborative_filtering::Learn_by_several_daya(const std::string& pathToData, int start_learning_day, int end_learning_day, bool print)
{
     Counters counters;
//    for (int i = 1; i < start_learning_day; ++i)
//    {
//        DayData dayData = read_day(pathToData +"data_by_days/" +std::to_string(i) + ".txt");
//        std::cout << "End reading " << i <<  "day\n";
//        calculate_counters(dayData, counters);
//    }

    clock_t start = clock();
    uumap queryUser(pathToData + "query_user_1_25");
    uumap userUrl(pathToData + "user_url_1_25");
    uumap queryRank(pathToData + "query_rank_1_25");
    counters.Set_user_url(userUrl);
    userUrl.clear();
    counters.Set_query_user(queryUser);
    queryUser.clear();
    counters.Set_query_rank(queryRank);
    queryRank.clear();
    std::cout << "have read counters for " << double(clock() - start) / CLOCKS_PER_SEC << " seconds ..." << std::endl;

    std::cout << "Run_Learning\n";
    std::unordered_set<int> filter;
    for (int i = start_learning_day; i < end_learning_day; ++i)
    {
         DayData dayData = read_day(pathToData + "data_by_days/"+ std::to_string(i) + ".txt");
         for (int j = 0; j <  1; ++j)
         {
         //   Learn(counters.query_user, counters.user_url, counters.query_rank, dayData);
         }
         calculate_counters(dayData, counters);
         //dayData.clear();
         if (i >= 10 && print)
         {
             filter = Test(counters.query_user, counters.user_url, counters.query_rank, i+1, pathToData);
         }

//         if (i == end_learning_day - 1)
//         {
//            Test2(counters.query_user, counters.user_url, counters.query_rank, end_learning_day, pathToData, filter);
//         }
    }

}



std::pair<size_t, size_t> Calculate_pairs(const Query& history, std::vector<double>& evristic, std::vector<double>& my_evristic)
{       
    std::pair<size_t, size_t> res(0, 0);
    for (int i = 0; i < 1; ++i)
    {
        for (int j = i+1; j < i+2; ++j)
        {
            int i_t = (history.type[i] == 2);
            int j_t = (history.type[j] == 2);
            if (i_t != j_t)
            {
                if ((evristic[i] > evristic[j]) == (i_t > j_t)) res.first += 1;
                if ((my_evristic[i] > my_evristic[j]) == (i_t > j_t)) res.second += 1;
            }
         }

     }
    return res;
}

void Get_second_click(const std::vector<double>& evristic, int rank_best_click, Result& r)
{
    int clickedBestRank = -1;
    double max_click = -1;
    double sum = 0;

    for (int i = 0; i < 10; ++i)
    {
        sum += evristic[i];
        if (max_click < evristic[i] && rank_best_click != i)
        {
            clickedBestRank = i;
            max_click = evristic[i];
        }
    }
}

bool GetResult(const Query& history,const std::vector<double>& evristic, Result& r, bool b)
{
    std::vector<std::pair<double, int>> res(10);
    for (int i = 0; i < 10; ++i)
    {
        res[i] = std::pair<double, int> (evristic[i], i);
    }
    std::sort(res.begin(), res.end(), [](std::pair<double, int> a, std::pair<double, int> b)
                                                    {
                                                      return a.first > b.first;
                                                    });
    // calculate statistics
    if (b && res[0].second < 4)
        return false;
    if (history.type[res[0].second] == 2)
    {
        ++r.right_answers;
        ++r.right_answers2;
        ++r.right_answers3;
    }
    else if (history.type[res[1].second] == 2)
    {
        ++r.right_answers2;
        ++r.right_answers3;
    }
    else if (history.type[res[2].second] == 2)
    {
        ++r.right_answers3;
    }
    ++r.wrong_answers;
    return true;
}



bool FindPersonWithSameClick(const Query& history, const vector<double>& evristic, const vector<double>& bias)
{
    for (size_t i = 0; i < 10; ++i)
    {
        if (history.type[i] == 2 && evristic[i]-bias[i] > 1e-10) return true;
    }
    return false;
}

bool FindPersonThatCLicksOnTop3Position(const vector<double>& evristic, const vector<double>& bias)
{
    for (size_t i = 0; i < 3; ++i)
    {
        if (evristic[i]-bias[i] < 1e-10) return false;
    }
    return true;
}

bool IsHeruisticMoreThanThreshold(const vector<double>& evristic, const vector<double>& bias)
{
    if (*std::max_element(evristic.begin(), evristic.end()) < 2.66)
    {
        return false;
    }
    return true;
}

void collaborative_filtering::TestOneEx(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                        const Query& history, Result& ranker,
                                        Result& ev, Result& my)
{
    size_t query = history.id;
    size_t user = history.person;
    std::vector<double> query_ctr = GetQueryCtr(queryRank, query);

    // get users which inserted this query
    const unordered_map<size_t, vector<double> >& users_map = queryUser.watch(query);
    unordered_set<size_t> users;
    for (auto& item : users_map)
    {
        users.insert(item.first);
    }

    int clickedBestRank = -1;
    vector<double> a;
    for (int i = 0; i < 10; ++i)
    {
        a.push_back(1e-10*(10.-i));
    }
    vector<double> evristic(a);
    vector<double> my_evristic(a);
    double sumUsersSimilarity = 0;
//    for (auto j = users_map.begin(); j != users_map.end(); ++j)
//    {
//        size_t nearestUser = j->first;
//        const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser);
//        clickedBestRank = -1;
//        for (size_t i = 0; i < 10; ++i)
//        {
//            size_t url = history.urls[i];
//            auto found = nearestUserUrls.find(url);
//            if (found != nearestUserUrls.end() && found->second.size() > 0)
//            {

//                //evristic[i] +=  1;
//                //my_evristic[i] += similarity(embedding[user], embedding[nearestUser]);
//                clickedBestRank = i;
//                break;
//            }
//        }
//        if (clickedBestRank >= 0)
//        {

//             evristic[clickedBestRank] +=  1;
//             my_evristic[clickedBestRank] += similarity(embedding[user], embedding[nearestUser]);
//                     user_user_query_similarity(user, nearestUser, query);
//        }
//    }
    double del = 1;
//    for(int i = 0; i < 10; ++i)
//    {
//        del += query_embedding[query][i];
//        my_evristic[i] *= del * query_ctr[i];
//        evristic[i] *= del * query_ctr[i];
//    }

    //if (!FindPersonWithSameClick(history, evristic, a)) return;
    //if (!IsHeruisticMoreThanThreshold(my_evristic, a)) return;

    std::pair<size_t, size_t> pairs_res = Calculate_pairs(history, evristic, my_evristic);
    std::vector<double> ranker_(10);
    for(int i = 0; i < 10; ++i)
    {
        ranker_[i] = 10-i;
    }
    for (size_t i = 0; i < 10; ++i)
    {
         const vector<double>& found = userUrl.watch(user, history.urls[i]);
         if (found.size() > 0 && found[0] > 1 - 1e-5)
         {
             evristic[i] += 1;
             break;
         }
    }

    double sum_evristic = 0;
    double sum_ctr = 0;
    double sum_my_evristic = 0;
    double sum_top = 0;
    for (int i = 0; i < 10; ++i)
    {
        sum_evristic += evristic[i];
        sum_ctr += query_ctr[i];
        sum_my_evristic += my_evristic[i];
    }
    if (true)
    {
        GetResult(history, evristic, ev, false);
        GetResult(history, ranker_, ranker, false);
        GetResult(history, my_evristic, my, false);
//        for (int j = 0; j < 10; ++j)
//        {
//            res_file << query_ctr[j] / sum_ctr << " ";
//        }
//        for (int j = 0; j < 10; ++j)
//        {
//            res_file << query_embedding[query][j] / (query_embedding[query][10] + 1e-10) << " ";
//        }
//        for (int j = 0; j < 10; ++j)
//        {
//            res_file << evristic[j] / sum_evristic << " ";
//        }
//        res_file << queryUser.watch(query).size() << " ";
//        for (int j = 0; j < 10; ++j)
//        {
//            res_file << my_evristic[j] / sum_my_evristic << " ";
//        }
//        for (size_t i = 0; i < 10; ++i)
//        {
//            if(history.type[i] == 2)
//            {
//                res_file << "1 ";
//            }
//            else
//            {
//                res_file << "0 ";
//            }
//        }
//        int t = 1;
//        for (size_t i = 0; i < 3; ++i)
//        {
//            if(history.type[i] == 2)
//            {
//                t = 0;
//            }
//        }

//        res_file << t << "\n";

    }




}

void collaborative_filtering::TestOneEx1(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                         const std::vector<Query>& dayDataVec,
                                         Result& ranker, Result& ev, Result& my,
                                         std::unordered_set<int>& examples,
                                         size_t coreIndex, size_t numCores)
{
    for (size_t i = coreIndex; i < dayDataVec.size(); i += numCores)
    {
       //if (i % 1000 == 0) std::cout << i << std::endl;
       if (GetFilterForTest(queryUser, userUrl, queryRank, dayDataVec[i]))
       {
           examples.insert(i);
           TestOneEx(queryUser, userUrl, queryRank, dayDataVec[i], ranker, ev, my);
       }
    }
}

void collaborative_filtering::TestOneExByIndex1(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                         const std::vector<Query>& dayDataVec,
                                         Result& ranker, Result& ev, Result& my,
                                         const std::unordered_set<int>& examples,
                                         size_t coreIndex, size_t numCores)
{

    for (size_t i = coreIndex; i < dayDataVec.size(); i += numCores)
    {
       //if (i % 1000 == 0) std::cout << i << std::endl;
       if (examples.find(i) != examples.end() && Filter2(queryUser,userUrl,queryRank,dayDataVec[i]) &&
               Filter3Mine(queryUser, userUrl, queryRank, dayDataVec[i], min_number, max_number))

       {
            TestOneEx(queryUser, userUrl, queryRank, dayDataVec[i], ranker, ev, my);
       }
    }
}
std::unordered_set<int> collaborative_filtering::Test(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day,
                                   const std::string& pathToData)
{
    std::cout << "Run TEST On day " << test_day << std::endl;
    DayData dayData = read_day(pathToData + "data_by_days/"+ std::to_string(test_day) + ".txt");
    std::vector<Query> dayDataVec = DayDataToVec(dayData);
    dayData.clear();

    auto start = std::chrono::system_clock::now();
    Result ranker;
    Result my;
    Result ev;
    std::unordered_set<int> examples_in_last_days;
    std::vector<std::thread> threads;
    std::vector<Result*> results;
    std::vector<Result*> my_results;
    std::vector<Result*> ranker_results;

    std::vector<std::unordered_set<int>*> examples;

    //size_t numThreads = 4;
    for (size_t i = 0; i < numThreads; ++i)
    {
        results.push_back(new Result());
        my_results.push_back(new Result());
        ranker_results.push_back(new Result());
        examples.push_back(new std::unordered_set<int>());
    }

    for (size_t i = 0; i < numThreads; ++i)
    {
        std::cout << i << "\n";
        std::thread t(&collaborative_filtering::TestOneEx1, this,
                      std::ref(queryUser), std::ref(userUrl), std::ref(queryRank), std::ref(dayDataVec),
                      std::ref(*ranker_results[i]), std::ref(*results[i]), std::ref(*my_results[i]),
                      std::ref(*examples[i]),
                      i, numThreads);
        threads.push_back(std::move(t));
    }
    std::cout << "End\n";
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads[i].join();
    }
    std::cout << "End\n";
    for (size_t i = 0; i < numThreads; ++i)
    {
        my.corect_pairs += my_results[i]->corect_pairs;
        my.right_answers += my_results[i]->right_answers;
        my.wrong_answers += my_results[i]->wrong_answers;
        my.right_answers2 += my_results[i]->right_answers2;
        my.right_answers3 += my_results[i]->right_answers3;
        ev.corect_pairs += results[i]->corect_pairs;
        ev.right_answers += results[i]->right_answers;
        ev.right_answers2 += results[i]->right_answers2;
        ev.right_answers3 += results[i]->right_answers3;
        ev.wrong_answers += results[i]->wrong_answers;
        ranker.right_answers += ranker_results[i]->right_answers;
        ranker.right_answers2 += ranker_results[i]->right_answers2;
        ranker.right_answers3 += ranker_results[i]->right_answers3;
        ranker.wrong_answers += ranker_results[i]->wrong_answers;
        examples_in_last_days.insert(examples[i]->begin(), examples[i]->end());

    }

    clear_vector(my_results);
    clear_vector(results);
    clear_vector(ranker_results);
    clear_vector(examples);
    auto end = std::chrono::system_clock::now();
    std::cout << " BASIC " << ranker.right_answers << " "
                           << ranker.right_answers2  << " "
                           << ranker.right_answers3 << " "
                           << ranker.wrong_answers
            << " EMBEDDING " << ev.right_answers << " "
            << ev.right_answers2 << " "
            << ev.right_answers3 << " "
            << ev.wrong_answers << " "
             << " MY_EMBEDDING "
             << my.right_answers << " "
             << my.right_answers2 << " "
             << my.right_answers3 << " "
             << ev.wrong_answers
             <<  std::endl;
    std::chrono::duration<double> diff = end-start;
    std::cout << "Ready!!! " << diff.count() <<std::endl;

    std::cout << "END TEST On day " << test_day << std::endl;
    std::cout << examples_in_last_days.size() << "\n";
    return examples_in_last_days;
}

void collaborative_filtering::Test2(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day,
                                   const std::string& pathToData, const std::unordered_set<int>& examples)
{
    std::cout << examples.size() << "\n";
    std::cout << "Run TEST On day " << test_day << std::endl;
    DayData dayData = read_day(pathToData + "data_by_days/"+ std::to_string(test_day) + ".txt");
    std::vector<Query> dayDataVec = DayDataToVec(dayData);
    dayData.clear();

    auto start = std::chrono::system_clock::now();
    Result ranker;
    Result my;
    Result ev;

    std::vector<std::thread> threads;
    std::vector<Result*> results;
    std::vector<Result*> my_results;
    std::vector<Result*> ranker_results;


    //size_t numThreads = 4;
    for (size_t i = 0; i < numThreads; ++i)
    {
        results.push_back(new Result());
        my_results.push_back(new Result());
        ranker_results.push_back(new Result());
    }

    for (size_t i = 0; i < numThreads; ++i)
    {
        std::cout << i << "\n";
        std::thread t(&collaborative_filtering::TestOneExByIndex1, this,
                      std::ref(queryUser), std::ref(userUrl), std::ref(queryRank), std::ref(dayDataVec),
                      std::ref(*ranker_results[i]), std::ref(*results[i]), std::ref(*my_results[i]),
                      std::ref(examples),
                      i, numThreads);
        threads.push_back(std::move(t));
    }
    std::cout << "End\n";
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads[i].join();
    }
    std::cout << "End\n";
    for (size_t i = 0; i < numThreads; ++i)
    {
        my.corect_pairs += my_results[i]->corect_pairs;
        my.right_answers += my_results[i]->right_answers;
        my.wrong_answers += my_results[i]->wrong_answers;
        ev.corect_pairs += results[i]->corect_pairs;
        ev.right_answers += results[i]->right_answers;
        ev.wrong_answers += results[i]->wrong_answers;
        ranker.right_answers += ranker_results[i]->right_answers;
        ranker.wrong_answers += ranker_results[i]->wrong_answers;

    }

    clear_vector(my_results);
    clear_vector(results);
    clear_vector(ranker_results);




    auto end = std::chrono::system_clock::now();
    res_file << " BASIC " << ranker.right_answers << " " << ranker.wrong_answers << " " << (double)ranker.right_answers / (ranker.wrong_answers + ranker.right_answers) << ";"
            << " EMBEDDING " << ev.right_answers << " " << ev.wrong_answers << " " << (double)ev.right_answers / (ev.wrong_answers + ev.right_answers) << ";"
             << " MY_EMBEDDING " << my.right_answers << " " << my.wrong_answers << " " << (double)my.right_answers / (my.wrong_answers + my.right_answers) << ";\n"
                  << " MY " << my.corect_pairs << " " << ev.corect_pairs <<endl;;

    std::chrono::duration<double> diff = end-start;
    std::cout << "Ready!!! " << diff.count() <<std::endl;

    std::cout << "END TEST On day " << test_day << std::endl;
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
