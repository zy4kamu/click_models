#include <collaborative_filtering.h>
#include <algorithm>

#include "FileReader.h"
#include "FileWriter.h"

namespace collaborative_filtering
{

CollaborativeFiltering::CollaborativeFiltering()
{
}

CollaborativeFiltering::CollaborativeFiltering(
    const string& folder, size_t dimension)
    : dimension(dimension)
    , userEmbedding(folder + "users", dimension)
    , queryEmbedding(folder + "queries", dimension)
    , docEmbedding(folder + "docs", dimension)
{
    FileManager::Read(folder + "examinations", &this->examinations);
}

void CollaborativeFiltering::initialize(const vector<size_t>& users
    , const vector<size_t>& queries
    , const vector<size_t>& docs
    , size_t dimension)
{
    this->dimension = dimension;
    this->userEmbedding.initialize(users, dimension);
    this->queryEmbedding.initialize(queries, dimension);
    this->docEmbedding.initialize(docs, dimension);
    this->examinations.resize(10, 0.5);
}

void CollaborativeFiltering::write(const string& folder)
{
    this->userEmbedding.write(folder + "users");
    this->queryEmbedding.write(folder + "queries");
    this->docEmbedding.write(folder + "docs");
    FileManager::Write(folder + "examinations", this->examinations);
}

double CollaborativeFiltering::estimateAttractiveness(
    size_t user, size_t query, size_t doc)
{
    const double* uEm = this->userEmbedding.get(user);
    const double* dEm = this->docEmbedding.get(doc);
    const double* qEm = this->queryEmbedding.get(query);
    double sum = 0;
    for (size_t i = 0; i < this->dimension; ++i)
    {
        sum += uEm[i] * dEm[i] * qEm[i];
    }
    return sum;
}

vector<double> CollaborativeFiltering::calculateClickProbabilities(const Query& serp)
{
    size_t size = serp.urls.size();
    vector<double> probs(size);
    for (size_t i = 0; i < size; ++i)
    {
        double attractiveness = this->estimateAttractiveness(
            serp.person, serp.id, serp.urls[i]);
        probs[i] = this->examinations[i] * attractiveness;
    }
    return probs;
}

vector<size_t> CollaborativeFiltering::sort(const Query& serp)
{
    vector<double> probs = this->calculateClickProbabilities(serp);
    vector<size_t> indecies(probs.size());
    for (size_t i = 0; i < probs.size(); ++i) indecies[i] = i;
    std::sort(indecies.begin(), indecies.end(),
        [&](size_t i1, size_t i2) {return probs[i1] > probs[i2]; });
    return indecies;
}

double CollaborativeFiltering::calculateLogLikelihood(const Query& serp)
{
    double loglikelihood = 0;
    vector<double> probs = this->calculateClickProbabilities(serp);
    for (size_t i = 0; i < serp.type.size(); ++i)
    {
        double toAdd = serp.type[i] == 2 ? std::log(probs[i]) : std::log(1.0 - probs[i]);
        if (std::isinf(toAdd)) {
            std::cout << "collaborative filtering:calculateLogLikelihood: zero probability ..." << std::endl;
        } else {
            loglikelihood += toAdd;
        }
    }
    return loglikelihood;
}

}
