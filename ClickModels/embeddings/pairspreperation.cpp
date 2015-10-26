#include "pairspreperation.h"

/*LearningExample::LearningExample(size_t us0, size_t us1, int l) : user0(us0), user1(us1), label(l) {}


PairsPreperation::PairsPreperation(const uumap& queryUser, const uumap& UserUrl, const uumap& queryRank,
                                   const string& pathToDayData)
{
    pairs.clear();
    DayData dayData = read_day(pathToDayData);
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
                const vector<double>& found = UserUrl.watch(user, history.urls[i]);
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
                size_t similarUserUrl = -1;
                size_t similarUser = item.first;
                if (item.second.size() == 0) continue;
                bool found = false;
                for (size_t i = 0; i < 10; ++i)
                {
                    const vector<double>& urls = UserUrl.watch(similarUser, history.urls[i]);
                    if (urls.size() > 0)
                    {
                        found = true;
                        similarUserUrl = i;
                        break;
                    }
                }
                if (!found) continue;

                if (user == similarUser) continue;
                int label = 0;
                if (history.type[similarUserUrl] == 2)
                {
                    label = 1;
                    ++numPlus;
                }
                else
                {
                    ++numMinus;
                }
                LearningExample pair(user, similarUser, label);
                pairs.push_back(pair);
            }
        }
    }
    std::cout << "Ready!!!" << std::endl;
}

const vector<LearningExample>& PairsPreperation::get_pairs() const
{
    return pairs;
}
*/
void PreparePairs(const string& outFile, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, DayData& dayData)
{

    std::cout << "Run Prepare Pairs" << endl;
    ofstream out(outFile);
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
            //if (queryUser.watch(query).size() > 10) continue;
            if (queryUser.watch(query).size() < 10) continue;
            const unordered_map<size_t, vector<double> >& users = queryUser.watch(query);

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

std::map<size_t, size_t> Get_number_trainig_example_with_user(const std::string& pairs_file)
{
    std::map<size_t, size_t> result;
    size_t user0, user1, label;
    int line_number = 0;
    ifstream in(pairs_file);
    while(!in.eof())
    {
        if (++line_number % 1000000 == 0)
        {
            std::cout << line_number << "\n";
        }
        in >> user0 >> user1 >> label;
        result[user0] += 1;
        result[user1] += 1;
    }
    return result;

}

std::map<double, double> GetHistogramm(const std::string& his_file)
{
    std::map<double, double> histogramm;
    histogramm[0] = 1.;
    ifstream in(his_file);
    while (!in.eof())
    {
        double key;
        double value;
        in >> key >> value;
        histogramm[key] = value;
    }
    in.close();
    return histogramm;
}

void GetHistogramm(
    const string& outFile, const string& pairsFile,
    Embedding& learner, std::map<size_t, size_t>& users_in_train)
{
    std::cout << "Run GetHisogramm\n";
    ofstream out(outFile);
    vector<size_t> users0, users1, labels;
    vector<pair<double, size_t>> distances;
    size_t user0, user1, label;
    size_t N = 1;
    for (size_t j = 0; j < N; ++j)
    {
        size_t last_user = 0;
        ifstream in(pairsFile);
        clock_t start = clock();
        size_t enumerator = 0;
        while (true)
        {
            while(!in.eof())
            {

                in >> user0 >> user1 >> label;
                if (last_user != user0)
                {
                    std::sort(distances.begin(), distances.end(), [](const pair<double,size_t>& x, const pair<double,size_t>& y)
                    {
                        return x.first < y.first;
                    });
                    if (users_in_train[last_user] > 1)
                    {
                        out << last_user << " ";
                        double res = 0;
                        for (int i = 0; i < distances.size(); ++i)
                        {
                            res += labels[distances[i].second];
                            out << res / (i+1) << " ";
                            //out << users_in_train[users1[distances[i].second]] << " ";
                        }
                        out << "\n";
                    }
                    last_user = user0;
                    enumerator = 0;
                    users0.clear();
                    users1.clear();
                    labels.clear();
                    distances.clear();
                }
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

}

std::map<double, double> GetGeneralHistogramm(MyLearner& learner, int day)
{
    std::cout << "Run GetHisogramm\n";
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
    ofstream out(out_directory + "data_stat/histogramms/histogramm_" + std::to_string(day));
    vector<size_t> users0, users1, labels;
    vector<pair<double, size_t>> distances;
    size_t user0, user1, label;
    size_t N = 1;
    for (size_t j = 0; j < N; ++j)
    {
        size_t last_user = 0;
        ifstream in(out_directory + "data_stat/pairs");
        clock_t start = clock();
        size_t enumerator = 0;
        while (true)
        {
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
    /*for (size_t i = 0; i < distances.size(); ++i)
    {
        //std::cout << distances[i].second << "\n";
        out << users0[distances[i].second] << " " << users1[distances[i].second] << " "
                                           << distances[i].first << " " << labels[distances[i].second] << "\n";
    }*/

    int n_bins = 200;
    std::map<double,double> histogramm;
    histogramm[0.] = 1.;
    int step = distances.size() / n_bins;


    size_t i = 0;
    while (i < distances.size())
    {
        int n_elements = 0;
        int n_positive_elements = 0;
        double last_d = distances[i].first;
        int j = 0;
        while (i < distances.size() && (j < step || last_d < distances[i].first))
        {
            n_elements += 1;
            if (labels[distances[i].second] == 1) n_positive_elements += 1;
            last_d = distances[i].first;
            ++i;
            ++j;
        }
        histogramm[last_d] = double(n_positive_elements) / n_elements;
        out << last_d << " " << histogramm[last_d] << "\n";
    }
    out.close();
    std::cout << "Ent GestHistogramm\n";
    return histogramm;
}

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

void Learn(MyLearner& learner, const string& outDirectory, const string& pairsfile, int day)
{
    /*DayData dayData26 = read_day(out_directory + "data_by_days/26.txt");
    DayData dayData27 = read_day(out_directory + "data_by_days/27.txt");
    uumap queryUser(out_directory + "query_user_1_25");
    uumap userUrl(out_directory + "user_url_1_25");
    uumap queryRank(out_directory + "query_rank_1_25");*/
    vector<size_t> users0, users1, labels;
    size_t user0, user1, label;
    size_t N = 10;
    for (size_t j = 0; j < N; ++j)
    {
        std::cout << "Step number = " << j << std::endl;
        ifstream in(pairsfile);

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
                std::thread t(&learn, &learner, std::ref(users0), std::ref(users1), std::ref(labels), i, numThreads, .1);
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
    std::cout << "Printing to file..." << std::endl;
    learner.Print(outDirectory + "embedding/model");
}
