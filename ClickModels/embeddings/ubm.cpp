//#include "ubm.h"
//#include "../counters.h"
//#include <numeric>
//#include <math.h>

//static int enumerator = 0;

//double DivCost(double x, double y)
//{
//    return -1./ (1 + std::exp(-(x-y)));
//}

//double Attractivness(double x)
//{
//    double y = x;
//    if (y > 5) y = 5;
//    if (y < -5) y = -5;
//    double res =  1. / (1 + exp(-x));
//    //if (std::abs(res) < 0.05) return 0.05;
//    //if (std::abs(res) > 0.95) return 0.95;
//    return res;
//}

//double DivAttractivness(double x)
//{
//    double res = Attractivness(x) * Attractivness(x) * exp(-x);
//    //if (std::abs(res) < 0.05) return 0.05;
//    //if (std::abs(res) > 0.95) return 0.95;
//    return res;
//}

//vector<Query> DayDataToVec(const DayData& day)
//{
//    std::cout << "Run DayData to Vec\n";
//    std::vector<Query> data;
//    for(const auto& item0 : day)
//    {
//        for (const auto& item1 : item0.second)
//        {
//            data.push_back(item1.second);
//        }
//    }
//    std::cout << "End DayData to Vec\n";
//    return data;
//}

//void GetResult(const Query& history,const std::vector<double>& evristic, Result1& r)
//{
//    int clickedBestRank = 0;
//    double max_click = evristic[0];
//    double sum_evristic = 0;

//    for (int i = 0; i < 10; ++i)
//    {
//        sum_evristic += evristic[i];
//        if (max_click < evristic[i])
//        {
//            clickedBestRank = i;
//            max_click = evristic[i];
//        }
//    }
//    // calculate statistics
//    if (history.type[clickedBestRank] == 2) {
//        ++r.right_answers;

//    } else {
//        ++r.wrong_answers;
//    }
//}

//bool  Filter1(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
//{
//    size_t query = history.id;
//    size_t user = history.person;
//    int url = -1;

//    const vector<double>& rank0 = queryRank.watch(query, 0);
//    const vector<double>& rank1 = queryRank.watch(query, 1);
//    if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
//    {
//     return false;
//    }
//    return true;
//}

//bool  Filter2(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
//{
//    size_t query = history.id;
//    size_t user = history.person;
//    int url = -1;

//    for (size_t i = 0; i < 10; ++i)
//    {
//     const vector<double>& found = userUrl.watch(user, history.urls[i]);
//     if (found.size() > 0 && found[0] > 1 - 1e-5)
//     {
//         return false;
//     }
//    }
//    return true;
//}

//bool Filter3(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
//{
//    size_t query = history.id;
//    if (queryUser.watch(query).size() < 10) return false;
//    return true;
//}

//bool Filter4(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
//{
//    bool found = false;
//    for (size_t i = 0; i < 10; ++i)
//    {
//     if(history.type[i] == 2)
//     {
//         found = true;
//         break;
//     }
//    }
//    if (!found) return false;
//    return true;
//}

//bool GetFilter(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
//{
//    typedef bool(*filter) (const uumap&, const uumap&, const uumap&, const Query&);
//    vector<filter> filters = {Filter1, Filter2, /*Filter3,*/ Filter4};
//    for (size_t i = 0; i < filters.size(); ++i)
//    {
//        if (!filters[i](queryUser, userUrl, queryRank, history)) return false;
//    }
//    return true;
//}



//UBM::UBM(const std::string& out_dir, int begin_day, int end_day):
//    out_dir(out_dir), start_day(begin_day), end_day(end_day), rate(-1.)
//{
//    res_file.open(out_dir + "data_stat/histogramms/result");
//     for (int i = begin_day; i <= end_day; ++i)
//     {
//        DayData day = read_day(out_dir + "data_by_days/"+ std::to_string(i) + ".txt");
//        for(const auto& item0 : day)
//        {
//            for (const auto& item1 : item0.second)
//            {
//                auto history = item1.second;
//                int query = history.id;
//                for (int rank = 0; rank < 10; ++rank)
//                {
//                    int url = history.urls[rank];
//                    attractivness[query][url] = 0.;
//                    for (int last_rank = -1; last_rank < rank; ++last_rank)
//                    {
//                        examination[rank][last_rank] = 0.;
//                    }
//                }
//            }
//        }
//     }
//}

//std::vector<double> UBM::res_one_position(const Query& history)
//{
//    std::vector<double> res(10, 0.);
//    int query = history.id;
//    int last_rank = -1;
//    double summ_all = 0;
//    for (int i = 0; i < 10; ++i)
//    {
//        int url = history.urls[i];
//        res[i] = Attractivness(attractivness[query][url]) * Attractivness(examination[i][last_rank]);
//        if (history.type[i] == 2) last_rank = i;
//    }
//    return res;
//}

//void UBM::UpdateMyMetrics(const Query& history)
//{
//    ++enumerator;
//    int query = history.id;
//    std:vector<double> position_distribution = res_one_position(history);
//    double sum_all = std::accumulate(position_distribution.begin(), position_distribution.end(), 0.);
//    std::vector<double> coeffs(10, 0.);
//    for (int rang = 0; rang < 10; ++rang)
//    {
//         int rang_truth = (history.type[rang]  == 2);
//         for (int rang1 = rang; rang1 < 10; ++rang1)
//         {
//             int rang1_truth = (history.type[rang1] == 2);
//             double d_cost = DivCost(position_distribution[rang], position_distribution[rang1]);
//             if (rang_truth > rang1_truth)
//             {
//                 coeffs[rang] += d_cost;
//                 coeffs[rang1] -= d_cost;
//             }
//             else if (rang_truth < rang1_truth)
//             {
//                 coeffs[rang] -= d_cost;
//                 coeffs[rang1] += d_cost;
//             }

//          }
//     }
//    int last_rank = -1;


//     for (int  i = 0; i < 10; ++i)
//     {
//         int url = history.urls[i];

//         double old_attr = attractivness[query][url];
//         attractivness[query][url] += rate * coeffs[i] * Attractivness(examination[i][last_rank]) *
//                 DivAttractivness(old_attr);
//         examination[i][last_rank] += rate * coeffs[i] * Attractivness(old_attr) *
//                 DivAttractivness(examination[i][last_rank]);
//         if (history.type[i] == 2) last_rank = i;
//     }
//}

//void UBM::MyLearn()
//{

//    for (int it = 0; it < 50; ++it)
//    {
//        rate /= std::sqrt(it + 1.);
//        Counters counters;
//        for (int i = start_day; i < end_day; ++i)
//        {
//            DayData day = read_day(out_dir + "data_by_days/"+ std::to_string(i) + ".txt");
//            for(const auto& item0 : day)
//            {
//                for (const auto& item1 : item0.second)
//                {
//                    if (GetFilter(counters.query_user, counters.user_url, counters.query_rank, item1.second))
//                     UpdateMyMetrics(item1.second);
//                }
//            }
//            calculate_counters(day, counters);
//        }
//        rate *= std::sqrt(it + 1);
//        std::cout << examination[0][-1] <<" " << examination[1][-1]
//                << " " << examination[1][0] << std::endl;
//        Test(counters.query_user, counters.user_url,
//             counters.query_rank, end_day, out_dir);
//    }
//}

//void UBM::EM_step(const DayData& day, uumap_1& s_u_q, uumap_1& s_r_r1, uumap_1& old_attr, uumap_1& old_exam,
//                  const uumap& queryUser, const uumap& userUrl, const uumap& queryRank)
//{
//    int enumerator = 0;
//    for(const auto& item0 : day)
//    {
//        for (const auto& item1 : item0.second)
//        {
//            if (++enumerator % 100000 == 0) std::cout << enumerator << std::endl;
//            int last_rank = -1;
//            auto history = item1.second;
//            int query = history.id;
//            if (GetFilter(queryUser, userUrl, queryRank, history))
//            {
//                for (int rank = 0; rank < 10; ++rank)
//                {
//                    int url = history.urls[rank];
//                    s_u_q[url][query] += 1;
//                    s_r_r1[rank][last_rank] += 1;
//                    if (history.type[rank] == 2)
//                    {
//                        attractivness[query][url] += 1;
//                        examination[rank][last_rank] += 1;
//                        last_rank = rank;
//                    }
//                    else
//                    {
//                        double d = 1 - old_exam[rank][last_rank] * old_attr[query][url];
//                        //std::cout << old_exam[rank][last_rank] << " " << old_attr[query][url] << "\n";
//                        attractivness[query][url] += ((1 - old_exam[rank][last_rank]) * old_attr[query][url]) / d;
//                        examination[rank][last_rank] += ((old_exam[rank][last_rank]) * (1 - old_attr[query][url])) / d;
//                    }
//                }
//            }
//        }
//    }
//}

//void UBM::LearnEM()
//{

//    for (int it = 0; it < 10; ++it)
//    {
//        uumap_1 s_u_q;
//        uumap_1 s_r_r1;
//        uumap_1 old_attr = attractivness;
//        attractivness.clear();
//        uumap_1 old_exam(examination);
//        examination.clear();
//        Counters counters;
//        for (int i = start_day; i < end_day; ++i)
//        {
//            DayData day = read_day(out_dir + "data_by_days/"+ std::to_string(i) + ".txt");

//            EM_step(day, s_u_q, s_r_r1, old_attr, old_exam, counters.query_user, counters.user_url,
//                    counters.query_rank);
//            calculate_counters(day, counters);
//        }
//        std::cout << "size = " << s_u_q.size() << "\n";
//        for (auto it_url = s_u_q.begin(); it_url != s_u_q.end(); ++it_url)
//        {
//            for (auto it_q = it_url->second.begin(); it_q != it_url->second.end(); ++it_q)
//            {
//                attractivness[it_q->first][it_url->first] /= it_q->second;
//            }
//        }
//        for (auto it_r = s_r_r1.begin(); it_r != s_r_r1.end(); ++it_r)
//        {
//            for (auto it_r1 = it_r->second.begin(); it_r1 != it_r->second.end(); ++it_r1)
//            {
//                examination[it_r->first][it_r1->first] /= it_r1->second;
//            }
//        }
//        std::cout << (examination[0][-1]) <<" " << (examination[1][-1])
//                << " " << (examination[1][1]) << std::endl;
//        Test(counters.query_user, counters.user_url,
//             counters.query_rank, end_day, out_dir);
//        counters.clear();
//    }
//}

//void UBM::TestOneEx(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
//               const Query& history,
//               Result1& ev, Result1& my, Result1& ranker)
//{
//    size_t query = history.id;

//    // get users which inserted this query
//    const unordered_map<size_t, vector<double> >& users_map = queryUser.watch(query);
//    std::unordered_set<size_t> users;
//    for (auto& item : users_map)
//    {
//        users.insert(item.first);
//    }

//    int clickedBestRank = -1;
//    vector<double> a;
//    for (int i = 0; i < 10; ++i)
//    {
//        a.push_back(1e-10*(10.-i));
//    }
//    vector<double> evristic(a);
//    vector<double> my_evristic(a);
//    for (auto j = users_map.begin(); j != users_map.end(); ++j)
//    {
//        size_t nearestUser = j->first;
//        const unordered_map<size_t, vector<double> >& nearestUserUrls = userUrl.watch(nearestUser);
//        clickedBestRank = -1;
//        for (size_t i = 0; i < 2; ++i)
//        {
//            size_t url = history.urls[i];
//            auto found = nearestUserUrls.find(url);
//            if (found != nearestUserUrls.end() && found->second.size() > 0)
//            {
//                evristic[i] +=  1;
//                //clickedBestRank = i;
//                //break;
//            }
//        }
////        if (clickedBestRank >= 0)
////        {

////             evristic[clickedBestRank] +=  1;
////        }
//    }

//    int last_rank = -1;
//    bool calculate = false;
//    my_evristic[0] = attractivness[query][history.urls[0]]; /* (examination[0][-1]);*/
//    my_evristic[1] = attractivness[query][history.urls[1]];
//            /*(my_evristic[0] * (examination[1][0]) +
//            (1 - my_evristic[0])*(examination[1][-1]))*/;


//    if (calculate) return;
//    GetResult(history, evristic, ev);
//    GetResult(history, my_evristic, my);
//    if (history.type[0] == 2)
//    {
//        ranker.right_answers += 1;
//    }
//    else
//    {
//        ranker.wrong_answers += 1;
//    }

//}

//void UBM::Test(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day,
//                                   const std::string& pathToData)
//{
//    std::cout << "Run TEST On day " << test_day << std::endl;
//    DayData dayData = read_day(pathToData + "data_by_days/"+ std::to_string(test_day) + ".txt");
//    std::vector<Query> dayDataVec = DayDataToVec(dayData);
//    dayData.clear();

//    auto start = std::chrono::system_clock::now();;
//    Result1 my;
//    Result1 ev;
//    Result1 ranker;

//    for (int i = 0; i < dayDataVec.size(); ++i)
//    {
//        if (GetFilter(queryUser, userUrl, queryRank, dayDataVec[i]) && Filter3(queryUser, userUrl, queryRank, dayDataVec[i]))
//        {
//            TestOneEx(queryUser, userUrl, queryRank, dayDataVec[i], ev, my, ranker);
//        }
//    }
//    auto end = std::chrono::system_clock::now();

//     std::cout << "Ranker " << ranker.right_answers << " " << ranker.wrong_answers << " " << (double)ranker.right_answers / (ranker.wrong_answers + ranker.right_answers) << ";"
//            << " EMBEDDING " << ev.right_answers << " " << ev.wrong_answers << " " << (double)ev.right_answers / (ev.wrong_answers + ev.right_answers) << ";"
//            << " MY_EMBEDDING " << my.right_answers << " " << my.wrong_answers << " " << (double)my.right_answers / (my.wrong_answers + my.right_answers) << ";\n"
//                  << std::endl;

//     res_file << "Ranker " << ranker.right_answers << " " << ranker.wrong_answers << " " << (double)ranker.right_answers / (ranker.wrong_answers + ranker.right_answers) << ";"
//            << " EMBEDDING " << ev.right_answers << " " << ev.wrong_answers << " " << (double)ev.right_answers / (ev.wrong_answers + ev.right_answers) << ";"
//            << " MY_EMBEDDING " << my.right_answers << " " << my.wrong_answers << " " << (double)my.right_answers / (my.wrong_answers + my.right_answers) << ";\n"
//                  << std::endl;

//    std::chrono::duration<double> diff = end-start;
//    std::cout << "Ready!!! " << diff.count() <<std::endl;

//    std::cout << "END TEST On day " << test_day << std::endl;
//}
