#include <collaborative_filtering.h>
#include <algorithm>

#include "FileReader.h"
#include "FileWriter.h"

namespace collaborative_filtering
{

static const size_t SERP_SIZE = 10;

CollaborativeFiltering::CollaborativeFiltering(size_t dimension)
    : userEmbedding(dimension)
    , queryEmbedding(dimension)
    , docEmbedding(dimension)
{
}

CollaborativeFiltering::CollaborativeFiltering(
    const string& folder, size_t dimension)
    : dimension(dimension)
    , userEmbedding(dimension)
    , queryEmbedding(dimension)
    , docEmbedding(dimension)
{
    this->userEmbedding.readDictionary(folder + "users");
    this->queryEmbedding.readDictionary(folder + "queries");
    this->docEmbedding.readDictionary(folder + "docs");

    size_t fullSize = SERP_SIZE + dimension * (
        this->userEmbedding.size() +
        this->queryEmbedding.size() +
        this->docEmbedding.size());
    this->parameters.resize(fullSize);

    this->readExaminations(folder);

    double* ptr = this->parameters.begin().base();

    ptr += SERP_SIZE;
    this->userEmbedding.setPtr(ptr);
    this->userEmbedding.readEmbedding(folder + "users");

    ptr += this->userEmbedding.size() * dimension;
    this->queryEmbedding.setPtr(ptr);
    this->queryEmbedding.readEmbedding(folder + "queries");

    ptr += this->queryEmbedding.size() * dimension;
    this->docEmbedding.setPtr(ptr);
    this->docEmbedding.readEmbedding(folder + "docs");
}

void CollaborativeFiltering::readExaminations(const string& folder)
{
    vector<double> ex;
    FileManager::Read(folder + "examinations", &ex);
    for (size_t i = 0; i < ex.size(); ++i)
    {
        this->examinations[i] = examinations[i];
    }
}

void CollaborativeFiltering::initialize(const vector<size_t>& users
    , const vector<size_t>& queries
    , const vector<size_t>& docs
    , size_t dimension)
{
    this->dimension = dimension;

    size_t fullSize = SERP_SIZE + dimension * (users.size() + queries.size() + docs.size());
    this->parameters.resize(fullSize);

    double* ptr = this->parameters.begin().base();
    this->examinations = ptr;
    for (size_t i = 0; i < SERP_SIZE; ++i)
        this->examinations[i] = 0.5;

    ptr += SERP_SIZE;
    this->userEmbedding.setPtr(ptr);
    this->userEmbedding.initialize(users, dimension);

    ptr += users.size() * dimension;
    this->queryEmbedding.setPtr(ptr);
    this->queryEmbedding.initialize(queries, dimension);

    ptr += queries.size() * dimension;
    this->docEmbedding.setPtr(ptr);
    this->docEmbedding.initialize(docs, dimension);
}

void CollaborativeFiltering::write(const string& folder)
{
    this->userEmbedding.write(folder + "users");
    this->queryEmbedding.write(folder + "queries");
    this->docEmbedding.write(folder + "docs");
    vector<double> ex(this->examinations, this->examinations + SERP_SIZE);
    FileManager::Write(folder + "examinations", ex);
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
