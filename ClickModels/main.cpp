#include "strategies/strategy0.h"
#include "tester.hpp"
#include "counters.h"

//class Strategy1
//{
//public:
//    Strategy1()
//    {
//    }

//    vector<int> Calculate(const Query& query)
//    {
//        for (size_t i = 0; i < 10; ++i)
//        {
//            if (get(get(userUrlParameters, query.person), query.urls[i], vector<double>(1, 0))[0] >= 1 - 1e-5)
//            {
//                return {};
//            }
//        }

//        if (get(get(queryRankParameters, query.id), size_t(0), vector<double>(1,0))[0] >=
//            2 * get(get(queryRankParameters, query.id), size_t(1), vector<double>(1,0))[0])
//        {
//            return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//        }

//        if (get(queryUserParameters, query.id).size() < 10)
//        {
//            return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//        }

//        vector<double> prob(10);
//        unordered_map<size_t, vector<double> >& users = get(queryUserParameters, query.id);
//        for (auto& user : users)
//        {
//            if (user.second.size() == 0) continue;
//            unordered_map<size_t, vector<double> >& docs = get(userUrlParameters, user.first);
//            for (size_t i = 0; i < 10; ++i)
//            {
//                auto found = docs.find(query.urls[i]);
//                if (found != docs.end() && found->second[0] > 0.5) {
//                    prob[i] += 1;
//                    break;
//                }
//            }
//        }

//        vector<int> ranging = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//        std::sort(ranging.begin(), ranging.end(), [&](int i, int j) { return prob[i] > prob[j]; } );
//        return ranging;
//    }
//};

//class Strategy2
//{
//public:
//    vector<int> Calculate(const Query& query)
//    {
//        vector<int> v = s.Calculate(query);
//        vector<int> v1 = s1.Calculate(query);

//        if (v.size() == 0 || v1.size() == 0) return {};
//        if (v1[0] != 0) return v1;
//        else return v;
//    }

//    Strategy s;
//    Strategy1 s1;
//};

//class Strategy3
//{
//public:
//    vector<int> Calculate(const Query& query)
//    {
//        for (size_t i = 0; i < 10; ++i)
//        {
//            if (get(get(userUrlParameters, query.person), query.urls[i], vector<double>(1, 0))[0] >= 1 - 1e-5)
//            {
//                return {};
//            }
//        }

//        if (get(get(userRankParameters, query.id), size_t(0), vector<double>(1,0))[0] >=
//            2 * get(get(queryRankParameters, query.id), size_t(1), vector<double>(1,0))[0])
//        {
//            return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//        }

//        if (get(queryUserParameters, query.id).size() < 10)
//        {
//            return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//        }

//        vector<double> prob(10);
//        double currentProb = 1;
//        for (size_t i = 0; i < 10; ++i)
//        {
//            vector<double>& pars = get(get(queryUrlParameters, query.id), query.urls[i]);
//            vector<double>& pars2 = get(get(userUrlParameters, query.person), i);
//            if (pars.size() == 4 && pars2.size() == 4 && pars[1] > 5 && pars2[1] > 5) {
//                prob[i] = currentProb * pars[1];
//                currentProb *= 1.0 - pars[3] / pars[1];
//                //currentProb *= 1.0 - pars2[3] / pars2[1];
//            } else if (pars2.size() == 4 && pars2[1] > 5) {
//                prob[i] = currentProb * pars[1];
//                //currentProb *= 1.0 - pars2[3] / pars2[1];
//            } else {
//                prob[i] = 0.1;
//            }
//        }
//        vector<int> ranging = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//        std::sort(ranging.begin(), ranging.end(), [&](int i, int j) { return prob[i] > prob[j]; } );
//        return ranging;
//    }
//};


template<class _Strategy>
void TryTester()
{
    Tester<_Strategy> tester("/home/stepan/Anna/big_data/days/27.txt");
    _Strategy strategy;
    tester.calculate(strategy);
    tester.print();
}

int main() 
{
    std::cout << "click-models: main.cpp is running..." << std::endl;
    //const string dataFolder = "/home/stepan/click_models_data/";
    string dataFolder= "/Users/annasepliaraskaia/Desktop/work/";
      //string dataFolder = "/home/anna/Рабочий стол/work/data/";
// 1.
    auto start = clock();
    separate_by_day(dataFolder + "kaggle_yandex/my_data/train", dataFolder + "data_by_days/");
    auto end = clock();
    std::cout << double(end - start) /  CLOCKS_PER_SEC << std::endl;
// 2.
//    calculate_counters(dataFolder + "data_by_days/", 1, 24);

// 3.
//    TryTester<Strategy0>();
}
