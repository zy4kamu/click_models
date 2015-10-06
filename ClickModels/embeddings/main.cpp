#include "MyLearner.h"
#include "day_data.h"
#include <ctime>

void learn(MyLearner* learner, const vector<size_t>& users0,
    const vector<size_t>& users1, const vector<size_t>& labels, size_t coreIndex, size_t numCores, double rate)
{
    size_t user0, user1, label;
    size_t enumerator = 0;
    clock_t start = clock();
    for (size_t i = coreIndex; i < users0.size(); i += numCores)
    {
        user0 = users0[i]; user1 = users1[i]; label = labels[i];
        if (label == 1) learner->MakeOnePositiveStep(user0, user1, rate);
        else learner->MakeOneNegativeStep(user0, user1, rate);
    }
}

void Learn(MyLearner& learner)
{
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    vector<size_t> users0, users1, labels;
    size_t user0, user1, label;
    size_t N = 1;
    for (size_t j = 0; j < N; ++j)
    {
        ifstream in(out_directory + "data_stat/pairs_26");
        clock_t start = clock();
        size_t enumerator = 0;
        while (true)
        {
            users0.clear();
            users1.clear();
            labels.clear();
            while(!in.eof())
            {
                in >> user0 >> user1 >> label;
                users0.push_back(user0);
                users1.push_back(user1);
                labels.push_back(label);
                if (++enumerator % 50000000 == 0)
                {
                    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                    std::cout << elapsedTime << ": " << enumerator << std::endl;
                    break;
                }
            }
            vector<std::thread> threads;
            size_t numThreads = 7;
            for (size_t i = 0; i < numThreads; ++i)
            {
                std::thread t(&learn, &learner, std::ref(users0), std::ref(users1), std::ref(labels), i, numThreads, 0.1);
                threads.push_back(std::move(t));
            }
            for (size_t i = 0; i < numThreads; ++i)
            {
                threads[i].join();
            }
            if (in.eof())
            {
                break;
            }
            std::cout << "Joined threads..." << std::endl;
        }
        std::cout << "Closing file..." << std::endl;
        in.close();
    }
//    std::cout << "Printing to file..." << std::endl;
//    learner.Print("/tmp/embedding");
}

void PreparePairs()
{
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    ofstream out(out_directory + "data_stat/pairs_27");
    uumap queryUser(out_directory + "query_user_1_25");
    uumap userUrl(out_directory + "user_url_1_25");
    uumap queryRank(out_directory + "query_rank_1_25");
    DayData dayData = read_day(out_directory + "data_by_days/27.txt");

    size_t numPlus = 0;
    size_t numMinus = 0;
    size_t enumerator = 0;
    clock_t start = clock();
    for(const auto& item0 : dayData)
    {
        for (const auto& item1 : item0.second)
        {
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
            const unordered_map<size_t, vector<double> >& users = queryUser.watch(query);

            size_t enumerator = 0;
            size_t divisor = users.size() / 100;
            for (auto& item : users)
            {
//                if (users.size() > 200)
//                {
//                    if (++enumerator % divisor != 0)
//                    {
//                        continue;
//                    }
//                }
                size_t similarUserUrl = -1;
                size_t similarUser = item.first;
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
                if (history.type[similarUserUrl] == 2) {
                    out << user << " " << similarUser << " " << 1 << "\n";
                    ++numPlus;
                } else {
                    out << user << " " << similarUser << " " << 0 << "\n";
                    ++numMinus;
                }
            }
        }
    }
    std::cout << "Ready!!!" << std::endl;
    out.close();
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
            } else {
                distanceMinus += learner.Distance(nearestUser, user);
                ++numMinus;
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

void Test1(MyLearner& learner)
{
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    uumap queryUser(out_directory + "query_user_1_25");
    uumap userUrl(out_directory + "user_url_1_25");
    uumap queryRank(out_directory + "query_rank_1_25");
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
            if (++enumerator % 1000 == 0) {
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

            if (queryUser.watch(query).size() < 10) continue;

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
            vector<double> evristics(10, 0);
            for (size_t nearestUser : users)
            {
                const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser);
                size_t clickedBestRank = -1;
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
                if (clickedBestRank + 1 != 0)
                    evristics[clickedBestRank] += 1.; // / learner.Distance(nearestUser, user);
            }
            size_t clickedBestRank = 0;
            double maxEvristics = 0;
            for (size_t i = 0; i < 10; ++i)
            {
                if (evristics[i] > maxEvristics)
                {
                    clickedBestRank = i;
                    maxEvristics = evristics[i];
                }
            }
            //new

            /*vector<std::pair<size_t, double> > nearest = learner.GetNearest(user, 1, users);
            if (nearest.size() == 0)
            {
                continue;
            }
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
    std::cout << "Ready!!!" << std::endl;

}

void GetHistogramm(MyLearner& learner)
{
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    ofstream out(out_directory + "data_stat/histogramm_27");
    vector<size_t> users0, users1, labels;
    vector<pair<double, size_t>> distances;
    size_t user0, user1, label;
    size_t N = 1;
    for (size_t j = 0; j < N; ++j)
    {
        ifstream in(out_directory + "data_stat/pairs_27");
        clock_t start = clock();
        size_t enumerator = 0;
        while (true)
        {
            /*users0.clear();
            users1.clear();
            labels.clear();
            distances.clear();*/
            while(!in.eof())
            {
                in >> user0 >> user1 >> label;
                users0.push_back(user0);
                users1.push_back(user1);
                labels.push_back(label);
                distances.push_back(pair<double, size_t>(learner.Distance(user0, user1), enumerator));
                if (++enumerator % 50000000 == 0)
                {
                    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
                    std::cout << elapsedTime << ": " << enumerator << std::endl;
                    break;
                }
            }
            if (in.eof())
            {
                break;
            }
        }
        std::cout << "Closing file..." << std::endl;
        in.close();
    }
    std::sort(distances.begin(), distances.end(), [](const pair<double,size_t>& x, const pair<double,size_t>& y)
    {
        return x.first < y.first;
    });
    for (size_t i = 0; i < distances.size(); ++i)
    {
        //std::cout << distances[i].second << "\n";
        out << users0[distances[i].second] << " " << users1[distances[i].second] << " "
                                           << distances[i].first << " " << labels[distances[i].second] << "\n";
    }
//    std::cout << "Printing to file..." << std::endl;
//    learner.Print("/tmp/embedding");
}

int main()
{
   //PreparePairs();
   std::function<double(double)> probFunctor = [](double x) -> double { return std::exp(-x); };
   std::function<double(double)> divLogFunctor = [](double x) -> double { return -1; };
   MyLearner learner("/Users/annasepliaraskaia/Desktop/work/kaggle_yandex/big_data/users", probFunctor, divLogFunctor, 90);
   Learn(learner);
   GetHistogramm(learner);
   //Test(learner);

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
