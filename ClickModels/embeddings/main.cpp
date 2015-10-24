#include "MyLearner.h"
#include "day_data.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "pairspreperation.h"
#include "embbedingbydays.h"
#include "personalizedclickmodel.h"
#include <ctime>

//void GetHistogramm(MyLearner& learner, int step);


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
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
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
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
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

int main()
{
   string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
   uumap queryUser(out_directory + "query_user_1_25");
   uumap userUrl(out_directory + "user_url_1_25");
   uumap queryRank(out_directory + "query_rank_1_25");
   DayData dayData = read_day(out_directory + "data_by_days/26.txt");
   PreparePairs(queryUser, userUrl, queryRank, dayData);
   std::function<double(double)> probFunctor = [](double x) -> double { return std::exp(-x/5.); };
   std::function<double(double)> divLogFunctor = [](double x) -> double { return -0.2; };
   MyLearner learner("/Users/annasepliaraskaia/Desktop/work/kaggle_yandex/big_data/users", probFunctor, divLogFunctor, 90);
   std::cout << "Run my learner\n";
   //MyLearner learner(out_directory + "embedding/alpha_0_2_20");
   Learn(learner);
   DayData dayData27 = read_day(out_directory + "data_by_days/27.txt");
   PreparePairs(queryUser, userUrl, queryRank, dayData27);
   //GetGeneralHistogramm(learner, 1000);
   GetHistogramm(learner,100);
   //GetHistogramm(learner, 1000);
   //std::map<double, double> histogramm = GetHistogramm(out_directory + "data_stat/histogramms/histogramm_0");
   //histogramm[100000000] = 0.2;
   //Test(learner);
    Test1(learner, 25);

   /*EmbbedingByDays embedding("/Users/annasepliaraskaia/Desktop/work/kaggle_yandex/big_data/users",
                             probFunctor, divLogFunctor);
   embedding.RunLearn(27);

   Test1(embedding.getLearner(), 25);*/
   /*PersonalizedClickModel p = PersonalizedClickModel(10,11);
   p.Learn(0.1);
   p.Test();*/
}


//void Learn()
//{
//    std::function<double(double)> probFunctor = [](double x) -> double { return std::exp(-x); };
//    std::function<double(double)> divLogFunctor = [](double x) -> double { return -1; };
//    MyLearner learner("/home/stepan/Anna/big_data/users", probFunctor, divLogFunctor, 100);

//    uumap queryUser("/tmp/query_user");
//    uumap userUrl("/tmp/user_url");
//    DayData dayData = read_day("/home/stepan/Anna/big_data/days/27.txt");

//    size_t numPlus = 0;
//    size_t numMinus = 0;
//    size_t enumerator = 0;
//    clock_t start = clock();
//    //bool shouldStop = false;
//    for (size_t j = 0; j < 7; ++j)
//    {
//        for(const auto& item0 : dayData)
//        {
//            //if (shouldStop) break;
//            for (const auto& item1 : item0.second)
//            {
//                const Query& history = item1.second;
//                if (++enumerator % 10000 == 0)
//                {
//                    //shouldStop = true;
//                    //break;
//                    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
//                    std::cout << elapsedTime << ": " << enumerator << " " << numPlus << " " << numMinus << " " << (double)numPlus / (numPlus + numMinus) << std::endl;
//                }
//                // get query, person and clicked url
//                size_t query = history.id;
//                size_t user = history.person;
//                size_t url = -1;
//                bool found = false;
//                for (size_t i = 0; i < 10; ++i)
//                {
//                    if(history.type[i] == 2)
//                    {
//                        found = true;
//                        url = history.urls[i];
//                        break;
//                    }
//                }
//                if (!found) continue;
//                const unordered_map<size_t, vector<double> >& users = queryUser.watch(query);
//                for (auto& item : users)
//                {
//                    size_t similarUser = item.first;
//                    if (item.second.size() == 0) continue;
//                    const vector<double>& urls = userUrl.watch(similarUser, url);
//                    if (user == similarUser) continue;
//                    if (urls.size() > 0) {
//                        learner.MakeOnePositiveStep(user, similarUser, 1);
//                        ++numPlus;
//                    } else {
//                        learner.MakeOneNegativeStep(user, similarUser, 1);
//                        ++numMinus;
//                    }
//                }
//            }
//        }
//    }
//    std::cout << "Printing to file..." << std::endl;
//    learner.Print("/tmp/embedding");
//    std::cout << "Ready!!!" << std::endl;
//}

//void SeparateByLines()
//{
//    size_t size = 1000;
//    size_t numCores = 7;
//    size_t numPerCore = size / numCores;
//    ifstream in("/tmp/embedding");
//    size_t user0, user1, label;
//    size_t enumerator = 0;
//    clock_t start = clock();
//    vector<ofstream*> outs;
//    for (size_t i = 0; i < numCores; ++i)
//    {
//        ofstream out("/tmp/pairs" + std::to_string(i));
//        outs.push_back(&out);
//    }
//    while(!in.eof())
//    {
//        in >> user0 >> user1 >> label;
//        if (++enumerator % 10000 == 0)
//        {
//            double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
//            std::cout << elapsedTime << ": " << enumerator << std::endl;
//        }
//        size_t index = enumerator / numPerCore;
//        outs[index] << user0 << " " << user1 << " " << label << "\n";
//    }
//    in.close();
//    for (size_t i = 0; i < numCores; ++i)
//    {
//        outs[i].close();
//    }
//}
