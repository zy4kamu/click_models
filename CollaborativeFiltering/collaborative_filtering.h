#ifndef COLLABORATIVE_FILTERING
#define COLLABORATIVE_FILTERING

#include <vector>

#include "embedding.h"
#include "day_data.h"
#include "embeddings/our_similarity.h"
#include "embeddings/collaborative_filtering.h"


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

    vector<size_t> sort(const Query& serp, const Counters& counts1);

    double calculateLogLikelihood(const Query& serp);

    void clear();

    void setParameters(const vector<double>& params);
    vector<double> calculateClickProbabilities(const Query& serp);
    vector<double> calculateClickProbabilitiesMy(const Query& serp, const Counters& counters);

    Embedding userEmbedding;
    Embedding queryEmbedding;
    Embedding docEmbedding;
    double* examinations;
    vector<double> parameters;
private:
    double estimateAttractiveness(size_t user, size_t query, size_t doc);



    void readExaminations(const string& folder);

    size_t dimension;
};

}

#endif
