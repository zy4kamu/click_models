#ifndef COLLABORATIVE_FILTERING_H
#define COLLABORATIVE_FILTERING_H

#include<vector>
#include<map>
//#include<numeric>
#include<random>
#include<math.h>
#include <thread>
#include "FileWriter.h"
#include "FileReader.h"
#include "../uumap.h"
#include "../day_data.h"
#include "../counters.h"
#include "filters.h"
#include "Embedding.h"
#include <mutex>


void Get_documents();
void Get_query();
void Get_users();

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
    int user_id;
    std::vector<double>& user;
    size_t rang_click_document;
    Example(std::vector<double>& u, size_t r, int u_id = 0): user(u), rang_click_document(r), user_id(u_id) {}
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
    double dist_first_evristic;
    double dist_second_evristic;
    size_t first_rang;
    size_t second_rang;
    size_t query_popularity;
    int truth;
    int base_result1;
    int ranker;
    ResultForOneEx(double fd, int t) : first_dist(fd), truth(t) {}
};



//probability to buy something sum(sim(u,u_i)*f) 
class collaborative_filtering
{
private:

    int dim;

    //std::unordered_map<size_t, std::vector<double>> document_embedding;
    std::unordered_map<size_t, std::vector<double>> query_embedding;
    std::unordered_map<size_t, std::vector<double>> user_query_embedding;
    std::ofstream res_file;
    Similarity_function f;

    int min_number;
    int max_number;
private:
    double user_user_query_similarity(int user1, int user2, int query);
    std::vector<double> res_one_position(const std::vector<Example>& examples, const std::vector<double>& user,
                                                                  int user_id, int query);
    std::vector<double> res_one_position(const std::vector<Example>& examples, const std::vector<double>& user);
public:
    std::unordered_map<size_t, std::vector<double>> embedding;
public:
    collaborative_filtering(double rate_, int dim_, const string& usersFile, const string& queryFile, const std::string& result_file, int min_n, int max_n);
    void LearnOneEx(const Query& history, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank);
    void LearnOneEx1(const std::vector<Query>& dayDataVec, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank,
                                             size_t coreIndex, size_t numCores);
    void LearnOneExForDocuments(const Query& history, const uumap& queryUser,
                                const uumap& userUrl, const uumap& queryRank);
    void Learn(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const DayData& dayData);
    void One_step(const std::vector<Example>& examples,
                                           const std::vector<bool>& truth,
                                           std::vector<double>& user);
    void One_step(const std::vector<Example>& examples,
                                           const std::vector<bool>& truth,
                                           std::vector<double>& user,
                                           int user_id,
                                           int query);
    void Learn_by_several_daya(const std::string& pathToData, int start_learning_day, int end_learning_day, bool print);
    std::unordered_set<int> Test(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day, const std::string& pathToData);

    //bool GetFilter(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
    //bool GetFilterForTest(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);

    void Test2(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, int test_day,
                                       const std::string& pathToData, const std::unordered_set<int>& examples);
    void TestOneEx(const uumap& queryUser, const uumap& userUrl,const uumap& queryRank,const Query& history, Result& ranker, Result& ev, Result& my);
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
