//#ifndef UBM_H
//#define UBM_H
//#include<map>
//#include <unordered_set>
//#include "../day_data.h"
//#include "../uumap.h"


//struct Result1
//{
//    size_t right_answers;
//    size_t wrong_answers;
//    Result1(): right_answers(0), wrong_answers(0) {}
//};

//class UBM
//{
//private:
//    int start_day;
//    int end_day;
//    std::string out_dir;
//    std::ofstream res_file;
//    double rate;
//private:
//    typedef std::unordered_map<int, std::unordered_map<int, double>> uumap_1;
//    uumap_1 attractivness;
//    uumap_1 examination;
//public:
//    void UpdateMyMetrics(const Query& history);
//    std::vector<double> res_one_position(const Query& history);
//    void MyLearn();
//public:
//    void EM_step(const DayData& d, uumap_1& s_u_q, uumap_1& s_r_r1, uumap_1& old_attr, uumap_1& old_exam,
//                 const uumap& queryUser, const uumap& userUrl, const uumap& queryRank);
//    void LearnEM();
//    void TestOneEx(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
//                   const Query& history,
//                   Result1& ev, Result1& my, Result1& ranker);
//    void Test(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day,
//                                       const std::string& pathToData);
//    UBM(const std::string& out_dir, int begin_day, int end_day);
//};

//#endif // UBM_H
