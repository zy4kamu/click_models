#ifndef COLLABORATIVE_FILTERING
#define COLLABORATIVE_FILTERING

#include <vector>

#include "embedding.h"
#include "day_data.h"

namespace collaborativefiltering
{

using std::vector;

class CollaborativeFiltering
{
public:
    CollaborativeFiltering(size_t dimension);

    CollaborativeFiltering(const string& folder, size_t dimension);

    void initialize(const vector<size_t>& users
        , const vector<size_t>& queries
        , const vector<size_t>& docs
        , size_t dimension);

    void write(const string& folder);

    vector<size_t> sort(const Query& serp);

    double calculateLogLikelihood(const Query& serp);

    void clear();

    void setParameters(const vector<double>& params);

    Embedding userEmbedding;
    Embedding queryEmbedding;
    Embedding docEmbedding;
    double* examinations;
    vector<double> parameters;
private:
    double estimateAttractiveness(size_t user, size_t query, size_t doc);

    vector<double> calculateClickProbabilities(const Query& serp);

    void readExaminations(const string& folder);

    size_t dimension;
};

}

#endif
