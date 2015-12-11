#include <collaborative_filtering.h>
#include <algorithm>

#include "macro_parameters.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "embeddings/our_similarity.h"

namespace collaborativefiltering
{

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

    double* ptr = &(this->parameters[0]);

    ptr += SERP_SIZE;
    this->userEmbedding.setPtr(ptr);
    this->userEmbedding.readEmbedding(folder + "users.embedding");

    ptr += this->userEmbedding.size() * dimension;
    this->queryEmbedding.setPtr(ptr);
    this->queryEmbedding.readEmbedding(folder + "queries.embedding");

    ptr += this->queryEmbedding.size() * dimension;
    this->docEmbedding.setPtr(ptr);
    this->docEmbedding.readEmbedding(folder + "docs.embedding");
}

void CollaborativeFiltering::readExaminations(const string& folder)
{
    vector<double> ex;
    FileManager::Read(folder + "examinations", &ex);
    this->examinations = &(this->parameters[0]);
    for (size_t i = 0; i < ex.size(); ++i)
    {
        this->examinations[i] = ex[i];
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

    double* ptr = &(this->parameters[0]);
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

void CollaborativeFiltering::setParameters(const vector<double>& params)
{
    this->parameters = params;
    double* ptr = &(this->parameters[0]);
    this->examinations = ptr;

    ptr += SERP_SIZE;
    this->userEmbedding.setPtr(ptr);

    ptr += this->userEmbedding.size() * dimension;
    this->queryEmbedding.setPtr(ptr);

    ptr += this->queryEmbedding.size() * dimension;
    this->docEmbedding.setPtr(ptr);
}

void CollaborativeFiltering::write(const string& folder)
{
    this->userEmbedding.write(folder + "users");
    this->queryEmbedding.write(folder + "queries");
    this->docEmbedding.write(folder + "documents");
    vector<double> ex(this->examinations, this->examinations + SERP_SIZE);
    FileManager::Write(folder + "examinations", ex);
}

static double regularize(double val)
{
    val = std::max(val, EPS);
    val = std::min(val, 1.0 - EPS);
    return val;
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
        probs[i] = regularize(this->examinations[i] * attractiveness);
    }
    return probs;
}

vector<double> CollaborativeFiltering::calculateClickProbabilitiesMy(const Query& serp,
                                                        const Counters& counters)
{
    size_t size = serp.urls.size();
    int query = serp.id;
    int user = serp.person;
    double* q = this->queryEmbedding.get(query);
    double* u = this->userEmbedding.get(user);
//    const unordered_map<size_t, vector<double> >& users_map = counters.query_user.watch(query);


//    unordered_set<size_t> users;
//    for (auto& item : users_map)
//    {
//        users.insert(item.first);
//    }

//    int clickedBestRank = -1;
    vector<double> a;
    for (int i = 0; i < 10; ++i)
    {
        a.push_back(1e-10*(10.-i));
    }
    vector<double> evristic(10, 0);
//    for (auto j = users_map.begin(); j != users_map.end(); ++j)
//    {
//        size_t nearestUser = j->first;
//        const unordered_map<size_t, vector<double> >& nearestUserUrls =
//                counters.user_url.watch(nearestUser);
//        clickedBestRank = -1;
//        for (size_t i = 0; i < 10; ++i)
//        {
//            size_t url = serp.urls[i];
//            auto found = nearestUserUrls.find(url);
//            if (found != nearestUserUrls.end() && found->second.size() > 0)
//            {

//                evristic[i] +=  1;
//            }
//        }
//    }
//      for (int i = 0; i < 10; ++i)
//      {
//          const unordered_map<size_t, vector<double> >& vec1 = counters.query_url.watch(query);
//          auto vec2 = vec1.find(serp.urls[i]);
//          if (vec2 != vec1.end() && vec2->second.size() > 0)
//          {
//            evristic[i] = vec2->second[1];
//            //std::cout << vec2->second[1] << " " << vec2->second[2] << " " << serp.type[i] <<"\n";
//          }
//      }
//      int del;
//      std::cin >> del;
//    if (std::abs(q[0] - 1) < 1e-10)
//    {
//        for (int i = 1; i < DIMENSION; ++i)
//        {
//            q[i] = 1.;
//        }
//    }
//    if (std::abs(u[0] - 1) < 1e-10)
//    {
//        int j = rand() % (DIMENSION-1);
//        j += 1;
//        for (int i = 0; i < DIMENSION; ++i)
//        {
//            if (i == 0)
//            {
//                u[i] = 0.5;
//            }
//            else if (i == j)
//            {
//                u[i] = 1;
//            }
//            else
//            {
//                u[i] = 0;
//            }
//        }
//    }
    //Cos::normalize(u+1, DIMENSION - 1);
//    std::vector<double> u_(u+1,u + dimension);

    vector<double> probs(size);
    for (size_t i = 0; i < size; ++i)
    {
        double* d = this->docEmbedding.get(serp.urls[i]);
        int j = rand() % (DIMENSION-1);
        j += 1;
        if (std::abs(d[0] - 1) < 1e-10)
        {
            for (int k = 0; k < DIMENSION; ++k)
            {
                if (k == 0)
                {
                    d[k] = 0.5;
                }
                else if (k == j)
                {
                    d[k] = 0;
                }
                else
                {
                    d[k] = 0;
                }
            }
        }
        std::vector<double> d_(d+1, d+dimension);
        double attr = 1;
        double attractiveness = Examination::Value(d[0]);
        const vector<double>& found = counters.user_url.watch(user, serp.urls[i]);
        if (found.size() > 0 && found[0] > 1 - 1e-5)
        {
            evristic[i] = 1;
        }
        probs[i] = evristic[i];//(Examination::Value(q[i]) * attractiveness);
    }
    return probs;
}

vector<size_t> CollaborativeFiltering::sort(const Query& serp,
                                            const Counters& counts1)
{
    vector<double> probs = this->calculateClickProbabilitiesMy(serp, counts1);
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
        double prob = probs[i];
        double toAdd = serp.type[i] == 2 ? std::log(prob) : std::log(1.0 - prob);
        loglikelihood += toAdd;
    }
    return loglikelihood;
}

void CollaborativeFiltering::clear()
{
    memset(&(parameters[0]), 0, parameters.size() * sizeof(double));
}

}
