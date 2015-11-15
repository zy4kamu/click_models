#include "MyLearner.h"
#include "day_data.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "pairspreperation.h"
//#include "embbedingbydays.h"
#include "personalizedclickmodel.h"
#include "Embedding.h"
#include "collaborative_filtering.h"
#include <ctime>
#include <ctime>

//void GetHistogramm(MyLearner& learner, int step);

//string out_directory = "/home/stepan/click_models_data/";
static string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
//static string out_directory = "/home/anna/Рабочий стол/work/data/";


void RandomPermutationOfPairs(const string& fileIn, const string& fileOut)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> uniformDistribution(0, 1);
    vector<string> lines;
    FileManager::Read(fileIn, &lines);
    vector<std::pair<size_t, double> > pairs;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        pairs.emplace_back(i, uniformDistribution(generator));
    }
    std::sort(pairs.begin(), pairs.end(), [](const std::pair<size_t, double>& x, const std::pair<size_t, double>& y) { return x.second < y.second; });
    vector<string> permuted;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        permuted.emplace_back(lines[pairs[i].first]);
    }
    FileManager::Write(fileOut, permuted);
}

void Test(MyLearner& learner)
{
    //string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    uumap queryUser(out_directory + "query_user_1_25");
    uumap userUrl(out_directory + "user_url_1_25");
    uumap queryRank(out_directory + "query_rank_1_25");
    DayData dayData = read_day(out_directory + "data_by_days/27.txt");

    size_t enumerator = 0;
    size_t numPlus = 0;
    size_t numMinus = 0;
    size_t stupidNumPlus = 0;
    size_t stupidNumMinus = 0;
    double distanceMinus = 0;
    double distancePlus = 0;
    clock_t start = clock();

    double mean_distance_minus = 0.;
    double mean_distance_plus = 0.;

    //bool shouldStop = false;
    for(const auto& item0 : dayData)
    {
        //if (shouldStop) break;
        for (const auto& item1 : item0.second)
        {
            const Query& history = item1.second;
            //if (++enumerator <= 10000) continue;
            if (++enumerator % 1000 == 0) {
                double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                std::cout << elapsedTime << ": " << enumerator << " "
                          << stupidNumPlus << " " << stupidNumMinus << " " << (double)stupidNumPlus / (stupidNumPlus + stupidNumMinus) << "; "
                          << numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus)
                          << " DISTANCE " << distancePlus/numPlus << " " << distanceMinus/numMinus <<std::endl;
                //shouldStop = true;
                //break;
            }
            // get query, person and clicked url
            size_t query = history.id;
            size_t user = history.person;
            size_t url = -1;

            const vector<double>& rank0 = queryRank.watch(query, 0);
            const vector<double>& rank1 = queryRank.watch(query, 1);
            if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
            {
                continue;
            }

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

            if (queryUser.watch(query).size() < 10) continue;

            const unordered_map<size_t, vector<double> >& users_map = queryUser.watch(query);
            unordered_set<size_t> users;
            for (auto& item : users_map)
            {
                users.insert(item.first);
            }
            vector<std::pair<size_t, double> > nearest = learner.GetNearest(user, 1, users);
            if (nearest.size() == 0)
            {
                continue;
            }
            size_t nearestUser = nearest[0].first;
            const vector<double>& urls = userUrl.watch(nearestUser, url);
            if (urls.size() > 0) {
                distancePlus += learner.Distance(nearestUser, user);
                ++numPlus;
                mean_distance_plus += learner.Distance(nearestUser, user);
            } else {
                distanceMinus += learner.Distance(nearestUser, user);
                ++numMinus;
                 mean_distance_minus += learner.Distance(nearestUser, user);
            }

            auto zz = users.begin();
            //if (users.size() > 1) zz++;
            size_t stupidNearsetUser = *zz;
            const vector<double>& stupidUrls = userUrl.watch(stupidNearsetUser, url);
            if (stupidUrls.size() > 0) {
                ++stupidNumPlus;
            } else {

                ++stupidNumMinus;
            }
        }
    }
    std::cout << "Ready!!!" << std::endl;

}

void Test1(MyLearner& learner,int day)
{
    //string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    uumap queryUser(out_directory + "query_user_1_" + std::to_string(day));
    uumap userUrl(out_directory + "user_url_1_" + std::to_string(day));
    uumap queryRank(out_directory + "query_rank_1_" + std::to_string(day));
    DayData dayData = read_day(out_directory + "data_by_days/27.txt");

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
            if (++enumerator % 10000 == 0) {
                double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                std::cout << elapsedTime << ": " << enumerator << " " << "BASIC "
                          << numBasicPlus << " " << numBasicMinus << " " << (double)numBasicPlus / (numBasicPlus + numBasicMinus)
                          << " EMBEDDING " <<numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << ";"
                          << numOnFirst << " " << numNOotOnFirst << endl;
            }
            // get query, person and clicked url
            size_t query = history.id;
            size_t user = history.person;
            size_t url = -1;

            const vector<double>& rank0 = queryRank.watch(query, 0);
            const vector<double>& rank1 = queryRank.watch(query, 1);
            if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
            {
                continue;
            }

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
            //if (queryUser.watch(query).size() > 10) continue;
            if (queryUser.watch(query).size() <  4) continue;
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
            const unordered_map<size_t, vector<double> >& users_map = queryUser.watch(query);
            unordered_set<size_t> users;
            for (auto& item : users_map)
            {
                users.insert(item.first);
            }

            // New
            int clickedBestRank = -1;
            vector<std::pair<size_t, double> > nearest = learner.GetNearest(user, 1, users);
            size_t nearestUser1 = nearest[3].first;
            //double usersDistance = learner.Distance(user, nearestUser1);
            double prob = 1;//histogramm.upper_bound(usersDistance)->second;
            if  (prob > 0)
            {
                const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser1);
                for (size_t i = 0; i < 2; ++i)
                 {
                      size_t url = history.urls[i];
                      auto found = nearestUserUrls.find(url);
                      if (found != nearestUserUrls.end() && found->second.size() > 0)
                      {
                           clickedBestRank = i;
                           break;
                      }
                 }
            }
            //if (prob <= 0.55) continue;
            if (clickedBestRank < 0) continue;
            //else
            /*{
                vector<double> evristics(10, 0);
                vector<double> clicks(10, 0);
                for (size_t nearestUser : users)
                {
                    const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser);
                    //double usersDistance = learner.Distance(user, nearestUser);
                    double prob = 1;//histogramm.lower_bound(usersDistance)->second;//std::exp(-usersDistance / 5.); //histogramm.lower_bound(usersDistance)->second;
                    for (size_t i = 0; i < 10; ++i)
                    {
                        size_t url = history.urls[i];
                        auto found = nearestUserUrls.find(url);
                        if (found != nearestUserUrls.end() && found->second.size() > 0)
                        {
                            if(prob > 0.5)
                            {
                                clickedBestRank = i;
                                evristics[clickedBestRank] += 1.;
                                clicks[clickedBestRank] += 1.;
                            }
                            break;
                        }
                    }

                }

                clickedBestRank = 0;
                double maxEvristics = -1000000;
                for (size_t i = 0; i < 10; ++i)
                {
                    if (evristics[i] > maxEvristics)
                    {
                        clickedBestRank = i;
                        maxEvristics = evristics[i];
                    }
                }
            }*/
            //new

            /*vector<std::pair<size_t, double> > nearest = queryUser.watch(query);
            if (nearest.size() == 0)
            {
                continue;
            }*/
            /*vector<std::pair<size_t, double> > nearest = learner.GetNearest(user, 1, users);
            size_t nearestUser = nearest[0].first;

            const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser);
            size_t clickedBestRank = 0;
            for (size_t i = 0; i < 2; ++i)
            {
                size_t url = history.urls[i];
                auto found = nearestUserUrls.find(url);
                if (found != nearestUserUrls.end() && found->second.size() > 0)
                {
                    clickedBestRank = i;
                    break;
                }
            }*/

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
    std::cout << "LAST RESULT " << enumerator << " " << "BASIC "
              << numBasicPlus << " " << numBasicMinus << " " << (double)numBasicPlus / (numBasicPlus + numBasicMinus)
              << " EMBEDDING " <<numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << ";"
              << numOnFirst << " " << numNOotOnFirst << endl;

    std::cout << "Ready!!!" << std::endl;

}

int Get_result(const std::vector<double>& evristic, const Query& history)
{
    int clickedBestRank = 0;
    double max_click = evristic[0];
    for (int i = 0; i < 10; ++i)
    {
        if (max_click < evristic[i])
        {
            clickedBestRank = i;
            max_click = evristic[i];
        }
    }
    if (history.type[clickedBestRank] == 2)
    {
        return 1;
    }
    else return 0;
}

int Get_bin(int n)
{
    std::vector<int> bins = {0,10,30,50,80,100, 200, 300, 500, 1000};
    for (int i = 0; i < bins.size(); ++i)
    {
        if (n < bins[i])
            return std::max(int(0), i-1);
    }
    return bins.size() - 1;
}

void Test2(std::map<size_t, size_t>& users_in_train)
{
    std::map<size_t, std::map<size_t,std::pair<int, int>>> res;
    std::vector<int> bins = {0,10,30,50,80,100, 200, 300, 500, 1000};
    std::ofstream out(out_directory + "data_stat/histogramms/hist_100");

    clock_t start = clock();
    std::cout << "reading embedding ..." << std::endl;
    Embedding embedding(out_directory + "embedding/model", 100);
    std::cout << "have read embedding for " << double(clock() - start) / CLOCKS_PER_SEC << " seconds ..." << std::endl;

    start = clock();
    uumap queryUser(out_directory + "query_user_1_25");
    uumap userUrl(out_directory + "user_url_1_25");
    uumap queryRank(out_directory + "query_rank_1_25");
    std::cout << "have read counters for " << double(clock() - start) / CLOCKS_PER_SEC << " seconds ..." << std::endl;

    start = clock();
    DayData dayData = read_day(out_directory + "data_by_days/27.txt");
    std::cout << "have read day data for " << double(clock() - start) / CLOCKS_PER_SEC << " seconds ..." << std::endl;

    size_t enumerator = 0;
    size_t numPlus = 0;
    size_t numMinus = 0;
    size_t numBasicPlus = 0;
    size_t numBasicMinus = 0;
    size_t numOnFirst = 0;
    size_t numNOotOnFirst = 0;
    double my_pairs = 0;
    double pairs = 0;
    for(const auto& item0 : dayData)
    {
        for (const auto& item1 : item0.second)
        {
            const Query& history = item1.second;
            if (++enumerator % 10000 == 0) {
                double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                std::cout << elapsedTime << ": " << enumerator << " " << "BASIC "
                          << numBasicPlus << " " << numBasicMinus << " " << (double)numBasicPlus / (numBasicPlus + numBasicMinus)
                          << " EMBEDDING " <<numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << ";"
                          << numOnFirst << " " << numNOotOnFirst <<" MY " << my_pairs << " " <<pairs <<endl;
            }
            // get query, person and clicked url
            size_t query = history.id;
            size_t user = history.person;
            size_t url = -1;

            // first evristic skip data (if first rank is twice more than second then skip)
            const vector<double>& rank0 = queryRank.watch(query, 0);
            const vector<double>& rank1 = queryRank.watch(query, 1);
            if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
            {
                continue;
            }

            // second evristic skip data (skip urls which this user has already seen)
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

            // skip seldom queries
            if (queryUser.watch(query).size() <  10) continue;
            //if (queryUser.watch(query).size() >  6) continue;

            //user not in train
            if (users_in_train[user] < 2) continue;

            // skip serps where there were not deep click
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

            // get users which inserted this query
            const unordered_map<size_t, vector<double> >& users_map = queryUser.watch(query);
            unordered_set<size_t> users;
            for (auto& item : users_map)
            {
                users.insert(item.first);
            }

            // get nearest user
            int clickedBestRank = -1;
            vector<std::pair<size_t, double> > nearest = embedding.GetNearest(user, users.size(), users);
            //if (nearest[0].second > 2) continue;
            //if (users.size() < 400) continue;
            vector<double> a;
            for (int i = 0; i < 10; ++i)
            {
                a.push_back(1e-10*(i+1));
            }
            vector<double> evristic(a);
            vector<double> my_evristic(a);
            // predict best rank by nearest users by summ
            int n_users = 0;
            std::map<size_t, pair<int, int>> r_one;
            //std::shuffle(nearest.begin(), nearest.end(), std::default_random_engine(0));
            std::pair<int, int> last(0, 0);
            for (size_t j = 0; j < std::min(size_t(400000), nearest.size()); ++j)
            {
                auto nearestUser = nearest[j];
                const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser.first);
                clickedBestRank = -1;
                //if (users_in_train[nearestUser.first] < 2) continue;
                for (size_t i = 0; i < 10; ++i)
                {
                    size_t url = history.urls[i];
                    auto found = nearestUserUrls.find(url);
                    if (found != nearestUserUrls.end() && found->second.size() > 0)
                    {
                        clickedBestRank = i;
                        break;
                    }
                }
                if (clickedBestRank >= 0)
                {

                     evristic[clickedBestRank] +=  1;//std::exp(-nearestUser.second);
                     my_evristic[clickedBestRank] += std::exp(-nearestUser.second);
                     //std::cout << nearestUser.second << " ";
                     //if (nearestUser.second <= 2)
                         n_users += 1;
                     int click_type = Get_result(evristic, history);
                     if (r_one.find(n_users) == r_one.end())
                     {
                         r_one[n_users] = std::pair<int, int>(0, 0);
                     }
                     if (click_type == 1)
                     {
                         r_one[n_users].first += 1;
                         last.first = 1;
                         last.second = 0;
                     }
                     else
                     {
                         r_one[n_users].second += 1;
                         last.first = 0;
                         last.second = 1;

                     }
                }
                //if (n_users >= 100) break;
            }


            //if (n_users <  80) continue;
            //std::cout << "\n";
            int bin = Get_bin(n_users);
            //std::cout << bin << "\n";
            res[bin];
            for (auto it = r_one.begin(); it != r_one.end(); ++it)
            {
                //if (it->first > bins[bin])
                //    continue;
                if (res[bin].find(it->first) == res[bin].end()) res[bin][it->first] = std::pair<int,int> (0,0);
                res[bin][it->first].first += it->second.first;
                res[bin][it->first].second += it->second.second;
            }
            int bin1 = std::min(bin+1, int(bins.size()-1));
            for (int i = n_users+1; i < bins[bin1]; ++i)
            {
                if (res[bin].find(i) == res[bin].end()) res[bin][i] = std::pair<int,int> (0,0);
                res[bin][i].first += last.first;
                res[bin][i].second += last.second;
            }

            clickedBestRank = 0;
            double max_click = evristic[0];
            for (int i = 0; i < 10; ++i)
            {
                if (max_click < evristic[i])
                {
                    clickedBestRank = i;
                    max_click = evristic[i];
                }
            }
            // calculate statistics
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
    std::cout << "LAST RESULT " << enumerator << " " << "BASIC "
              << numBasicPlus << " " << numBasicMinus << " " << (double)numBasicPlus / (numBasicPlus + numBasicMinus)
              << " EMBEDDING " <<numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << ";"
              << numOnFirst << " " << numNOotOnFirst << endl;

    std::cout << "Ready!!!" << std::endl;
//    std::ofstream out(out_directory + "data_stat/histogramms/hist_100");
//    for (auto it = res.begin(); it != res.end(); ++it)
//    {
//        out << it->first << " ";
//        int last_value = 0;
//        for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
//        {
//            out << it1->second.first / (it1->second.first + it1->second.second + 1e-10) << " ";
//            last_value = it1->second.first + it1->second.second;
//        }
//        out << "\n";
//        std::cout << last_value << "\n";
//    }
//    out.close();
}

int main()
{
     //prepare pairs
//     uumap queryUser(out_directory + "query_user_1_24");
//     uumap userUrl(out_directory + "user_url_1_24");
//     uumap queryRank(out_directory + "query_rank_1_24");
//     DayData dayData = read_day(out_directory + "data_by_days/25.txt");
//     PreparePairs(out_directory + "data_stat/pairs_25", queryUser, userUrl, queryRank, dayData);

    // learn
/*    std::function<double(double)> probFunctor = [](double x) -> double { return std::exp(-x/5.); };
    std::function<double(double)> divLogFunctor = [](double x) -> double { return -0.2; };
    MyLearner learner(out_directory + "users", probFunctor, divLogFunctor, 100);
    Learn(learner, out_directory, out_directory + "data_stat/pairs_26");
    std::cout << "READY!\n"*/;
    
//learn collaborative filtering
//    uumap queryUser(out_directory + "query_user_1_25");
//    uumap userUrl(out_directory + "user_url_1_25");
//    uumap queryRank(out_directory + "query_rank_1_25");
//    DayData dayData = read_day(out_directory + "data_by_days/26.txt");
    collaborative_filtering learner(-1., 20, out_directory + "users", out_directory + "documents");
//      std::vector<Example> examples;
//      std::vector<bool> truth(10, false);
//      truth[2] = true;
//      examples.push_back(Example(learner.embedding[0],0));
//      examples.push_back(Example(learner.embedding[1],0));
//      examples.push_back(Example(learner.embedding[2],0));
//      examples.push_back(Example(learner.embedding[3],1));
//      examples.push_back(Example(learner.embedding[4],1));
//      examples.push_back(Example(learner.embedding[5],1));
//      examples.push_back(Example(learner.embedding[6],1));
//      examples.push_back(Example(learner.embedding[7],2));
//      examples.push_back(Example(learner.embedding[8],2));
//      for (int i = 0; i < 10; ++i)
//      {
//        learner.One_step(examples,truth,9, true);
//      }
    for (int i = 0; i < 100; ++i)
    {
        learner.rate /= std::sqrt(i+1);
        std::cout << " N ITERATION " << i << std::endl;
        bool print = i % 1 == 0;
        learner.Learn_by_several_daya(out_directory,25,27, print);
        learner.rate *= std::sqrt(i+1);
    }
//    queryUser.clear();
//    userUrl.clear();
//    queryRank.clear();
//    dayData.clear();
    learner.Print(out_directory + "embedding/model");

//    std::map<size_t, size_t> users_in_train =  Get_number_trainig_example_with_user(out_directory + "data_stat/pairs_26");
//    Test2(users_in_train);




   //DayData dayData27 = read_day(out_directory + "data_by_days/27.txt");
   //PreparePairs(queryUser, userUrl, queryRank, dayData27);
//     Embedding embedding(out_directory + "embedding/model2", 100);
//     GetGeneralHistogramm(embedding, 1001);
   //GetHistogramm(out_directory + "auxiliary/histogram", out_directory + "auxiliary/pairs_27", embedding, 100);
   //GetHistogramm(learner, 1000);
   //std::map<double, double> histogramm = GetHistogramm(out_directory + "data_stat/histogramms/histogramm_0");
   //histogramm[100000000] = 0.2;
   //Test(learner);
   //Test1(learner, 25);

//   EmbbedingByDays embedding(out_directory + "users", out_directory + "auxiliary/pairs_del", out_directory + "auxiliary/embbiding_by_one_week",
//                             out_directory + "data_by_days/", out_directory,
//                             probFunctor, divLogFunctor);
//   embedding.RunLearn(27);

   /*PersonalizedClickModel p = PersonalizedClickModel(10,11);
   p.Learn(0.1);
   p.Test();*/
}
