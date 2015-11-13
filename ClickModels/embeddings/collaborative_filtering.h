#ifndef COLLABORATIVE_FILTERING_H
#define COLLABORATIVE_FILTERING_H

#include<vector>
#include<map>
#include<numeric>
#include<random>
#include<math.h>
#include <thread>
#include "FileWriter.h"
#include "FileReader.h"
#include "../uumap.h"
#include "../day_data.h"
#include "../counters.h"
#include "Embedding.h"
#include <mutex>




class Similarity_function
{
public:
    std::function<double(const std::vector<double>& ,const std::vector<double>&)> similarity;
    std::function<std::vector<double>
    (const std::vector<double>&, const std::vector<double>&)> divSimilarity;
    Similarity_function(std::function<double(const std::vector<double>& ,const std::vector<double>&)> s,
                        std::function<std::vector<double>
                        (const std::vector<double>&, const std::vector<double>&)> div) : similarity(s), divSimilarity(div) {}
};

class Example
{
public:
    size_t user;
    size_t rang_click_document;
    Example(size_t u, size_t r): user(u), rang_click_document(r) {}
};

struct VectorWithLoc
{
    std::vector<double>& v;
    std::mutex m;
    VectorWithLoc(std::vector<double>& v) : v(v) {}
};

struct ResultForOneEx
{
    double first_dist;
    double second_dist;
    int truth;
    int base_result1;
    int ranker;
    ResultForOneEx(double fd, double sd, int t) : first_dist(fd), second_dist(sd), truth(t) {}
};

struct Result
{
    size_t corect_pairs;
    size_t right_answers;
    size_t wrong_answers;
    std::vector<ResultForOneEx> distToAnswers;
    Result(): corect_pairs(0), right_answers(0), wrong_answers(0) {}
};

//probability to buy something sum(sim(u,u_i)*f) 
class collaborative_filtering
{
private:

    int dim;
    std::unordered_map<size_t, std::vector<double>> embedding;
    std::unordered_map<size_t, std::vector<double>> document_embedding;
    std::ofstream res_file;
    Similarity_function f;
private:
    std::vector<double> res_one_position(const std::vector<Example>& examples, size_t user) const;

public:
    collaborative_filtering(double rate_, int dim_, const string& usersFile);
    void LearnOneEx(const Query& history, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank);
    void LearnOneEx1(const std::vector<Query>& dayDataVec, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                             size_t coreIndex, size_t numCores);
    void Learn(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const DayData& dayData);
    void One_step(const std::vector<Example>& examples,
                                           const std::vector<bool>& truth,
                                           size_t user);
    void One_step1( const std::vector<std::vector<Example>>& examples,
                                            const std::vector<std::vector<bool>>& truth,
                                            const std::vector<size_t>& users,
                                            size_t coreIndex, size_t numCores);
    void Learn_by_several_daya(const std::string& pathToData, int start_learning_day, int end_learning_day, bool print);
    std::unordered_set<int> Test(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day, const std::string& pathToData);

    bool GetFilter(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
    void Test2(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day,
                                       const std::string& pathToData, const std::unordered_set<int>& examples);
    void TestOneEx(const uumap& queryUser, const uumap& userUrl,const Query& history, Result& ranker, Result& ev, Result& my);
    void TestOneEx1(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                             const std::vector<Query>& dayDataVec,
                                             Result& ranker, Result& ev, Result& my,
                                             std::unordered_set<int>& examples,
                                             size_t coreIndex, size_t numCores);
    void TestOneExByIndex1( const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                             const std::vector<Query>& dayDataVec,
                                             Result& ranker, Result& ev, Result& my,
                                             const std::unordered_set<int>& examples,
                                             size_t coreIndex, size_t numCores);
    void Print(const string& file) const;
    double rate;
};

#endif // COLLABORATIVE_FILTERING_H
