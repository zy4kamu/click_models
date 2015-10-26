#ifndef PAIRSPREPERATION_H
#define PAIRSPREPERATION_H
#include "../utils.h"
#include "../uumap.h"
#include "../day_data.h"
#include "../counters.h"
#include "MyLearner.h"
#include "Embedding.h"

/*struct LearningExample
{
    size_t user0;
    size_t user1;
    int label;
    LearningExample(size_t us0, size_t us1, int l);
};*/
void GetHistogramm(
    const string& outFile, const string& pairsFile,
    Embedding& learner, std::map<size_t, size_t>& users_in_train);
std::map<double, double> GetHistogramm(const std::string& his_file);
std::map<double, double> GetGeneralHistogramm(MyLearner& learner, int day);

std::map<size_t, size_t> Get_number_trainig_example_with_user(const std::string& pairs_file);
void PreparePairs(const string& outFile, const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, DayData& dayData);

void Learn(MyLearner& learner, const string& outDirectory, const string& pairsfile, int day=0);


/*class PairsPreperation
{
public:
    PairsPreperation(const uumap& queryUser, const uumap& UserUrl, const uumap& queryRank, const string& pathToDayData);
    const vector<LearningExample>& get_pairs() const;
private:
    vector<LearningExample> pairs;
};*/

#endif // PAIRSPREPERATION_H
