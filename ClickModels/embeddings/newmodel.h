#ifndef NEWMODEL_H
#define NEWMODEL_H

#include<unordered_map>
#include <set>
#include "../day_data.h"
#include "../counters.h"
#include "our_similarity.h"
#include "pairwaislearning.h"
#include "filters.h"


class Data
{
public:
    std::unordered_map<int, std::unordered_map<int, double>> data;
    void Put(int key1, int key2, double value);
    double Get(int key1, int key2);

};

class Data1
{
public:
    int dim;
    std::unordered_map<int, std::vector<double>> data;
    void Change(int key1, int i, double value);
    std::vector<double> Get(int key1);

};

namespace Params
{
static const string OUT_DIRECTORY =  "/Users/annasepliaraskaia/Desktop/work/";
static const string DAY_DATA_FOLDER = OUT_DIRECTORY + "data_by_days/";
static const string INPUT_FOLDER = OUT_DIRECTORY + "initial_model_parameters_3/";
static const string INITIAL_MODEL_PARAMETERS_FOLDER = OUT_DIRECTORY + "initial_model_parameters_3/";
static const size_t DIMENSION = 101;
static const size_t SERP_SIZE = 10;
static const size_t FIRST_TRAINING_DAY = 1;
static const size_t LAST_TRAINING_DAY = 20;
static const double EPS = 1e-2;
static double LEARNING_RATE = -1;
static const double MIN_EMBEDDING_VALUE = 0.1;
static const double MAX_EMBEDDING_VALUE = 0.2;
static const string PATH_TO_DATA = OUT_DIRECTORY;

};

namespace Utils_metrics
{
std::vector<size_t> Merge_two_rankers(const std::vector<size_t>& a,
                                                const std::vector<size_t>& b);
std::vector<double> DoubleClick(const Query& serp);
std::vector<double> Evristic(const Query& serp);
void Get_tops(const Query& serp,
        const std::vector<size_t>& probs, std::vector<double>& res);
double Get_NDCG(const Query& serp,
        const std::vector<size_t>& probs);
vector<size_t> sort(const std::vector<double>& probs);

};

class Model
{
public:
    virtual std::vector<double> calculateClickProbabilities(const Query& serp) = 0;
    vector<size_t> sort(const std::vector<double>& probs) {return Utils_metrics::sort(probs);}
    virtual void User_step(size_t user, size_t query, int rank,
                      const Query& serp, double min_value, double max_value,
                      std::vector<double>& coeffs) = 0;
    virtual void Query_step(size_t user, size_t query, int rank,
                      const Query& serp, double min_value, double max_value,
                      std::vector<double>& coeffs) = 0;
    virtual void Document_step(size_t user, size_t query, int rank,
                      const Query& serp, double min_value, double max_value,
                      std::vector<double>& coeffs) = 0;
    virtual void Frequency_step(size_t user, size_t query, int rank,
                                const Query& serp, double min_value, double max_value,
                                std::vector<double>& coeffs) = 0;
    double CalculateValue(size_t start, size_t end);
};

class NewModel: public Model
{
private:
    Data user_document;
    Data query_document;
    std::unordered_map<int, std::vector<double>> query_examination;
    std::unordered_map<int, std::vector<double>> frequency_examination;
    std::unordered_map<int,int> frequency_query;
public:
    NewModel();
    std::vector<double> calculateClickProbabilities(const Query& serp);
    void User_step(size_t user, size_t query, int rank,
                   const Query& serp, double min_value, double max_value,
                   std::vector<double>& coeffs);
    void Query_step(size_t user, size_t query, int rank,
                   const Query& serp, double min_value, double max_value,
                   std::vector<double>& coeffs);
    void Document_step(size_t user, size_t query, int rank,
                   const Query& serp, double min_value, double max_value,
                   std::vector<double>& coeffs);
    void Frequency_step(size_t user, size_t query, int rank,
                                    const Query& serp, double min_value, double max_value,
                                    std::vector<double>& coeffs);
};

class Learner_new_model
{
private:
    pairwaisLearning ranker;
    Model* model;
public:
    Learner_new_model();
    void learn();
    void makeOneStep(int step);
    void UpgradeOneExample(const Query& serp, int truth_type, bool change_document,
                           bool change_user, bool change_query);

};

#endif // NEWMODEL_H
