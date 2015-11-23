#include "learner.h"
#include "LBFGS.h"
#include <cstring>
#include <cmath>
#include "uumap.h"
#include "embeddings/collaborative_filtering.h"

namespace collaborativefiltering
{

Learner::Learner(const string& folder, size_t dimension)
    : model(folder, dimension)
    // , gradient(folder, dimension)
{
    //this->dimension = this->model.parameters.size();
}

void Learner::learn()
{
//    Optimization::LBFGS lbfgs;
//    vector<double> initialPoint = model.parameters;
//    vector<double> minPoint(initialPoint.size());
//    Optimization::ICallable eventHandler;
//    lbfgs.Optimize(*this, &eventHandler, initialPoint, &minPoint, 1000);
    while(true)
    {
        this->makeOneStep__();
    }
}
uumap queryUser = uumap(PATH_TO_DATA + "query_user_1_25");
uumap userUrl = uumap(PATH_TO_DATA + "query_user_1_25");
uumap queryRank = uumap(PATH_TO_DATA + "query_user_1_25");

double Learner::CalculateValue(size_t start, size_t end)
{
    size_t enumerator = 0;
    double sum = 0;
    double top1Before = 0;
    double top1After = 0;
    for (size_t i = start; i <= end; ++i)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                if (!GetFilter(queryUser, userUrl, queryRank, serp))
                {
                    continue;
                }
                top1Before += serp.type[0] == 2 ? 1 : 0;
                sum += this->model.calculateLogLikelihood(serp);
                top1After += serp.type[this->model.sort(serp)[0]] == 2 ? 1 : 0;
                enumerator += 1;
            }
        }
    }
    std::cout << enumerator << " top1 before = " << top1Before / enumerator << "; top1 after = " << top1After / enumerator << std::endl;
    return sum / double(enumerator);
}

//void Learner::CalculateGradient(const vector<double>& point,
//    vector<double>* gradient)
//{
//    this->model.setParameters(point);
//    this->calculateGradient();
//    memcpy(&(gradient->at(0)), &(this->gradient.parameters[0]),
//        this->gradient.parameters.size() * sizeof(double));
//}

//void Learner::calculateGradient()
//{
//    this->gradient.clear();
//    for (size_t i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY; ++i)
//    {
//        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
//        for (const auto& personData : data)
//        {
//            const unordered_map<size_t, Query>& sessionData = personData.second;
//            for (const auto& session : sessionData)
//            {
//                const Query& serp = session.second;
//                size_t query = serp.id;
//                size_t user = serp.person;
//                for (size_t i = 0; i < SERP_SIZE; ++i)
//                {
//                    size_t doc = serp.urls[i];
//                    bool click = serp.type[i] == 2;
//                    if (click) {
//                        this->makePositiveUpdate(query, user, doc, i);
//                    } else {
//                        this->makeNegativeUpdate(query, user, doc, i);
//                    }
//                }
//            }
//        }
//    }
//}

//static double regularize(double val)
//{
//    val = std::max(val, EPS);
//    val = std::min(val, 1.0 - EPS);
//    return val;
//}

//void Learner::makePositiveUpdate(size_t query, size_t user, size_t doc, size_t rank)
//{
//    gradient.examinations[rank] -= 1.0 / regularize(model.examinations[rank]);
//    double* u = model.userEmbedding.get(user);
//    double* d = model.docEmbedding.get(doc);
//    double* q = model.queryEmbedding.get(query);
//    double uqd = 0;
//    for (size_t i = 0; i < DIMENSION; ++i)
//    {
//        uqd += regularize(u[i]) * regularize(d[i]) * regularize(q[i]);
//    }
//    double* gu = gradient.userEmbedding.get(user);
//    double* gd = gradient.docEmbedding.get(doc);
//    double* gq = gradient.queryEmbedding.get(query);
//    for (size_t i = 0; i < DIMENSION; ++i)
//    {
//        gu[i] -= regularize(d[i]) * regularize(q[i]) / (uqd + EPS);
//        gd[i] -= regularize(u[i]) * regularize(q[i]) / (uqd + EPS);
//        gq[i] -= regularize(u[i]) * regularize(d[i]) / (uqd + EPS);
//    }
//}

//void Learner::makeNegativeUpdate(size_t query, size_t user, size_t doc, size_t rank)
//{
//    double examination = regularize(model.examinations[rank]);
//    double* u = model.userEmbedding.get(user);
//    double* d = model.docEmbedding.get(doc);
//    double* q = model.queryEmbedding.get(query);
//    double uqd = 0;
//    for (size_t i = 0; i < DIMENSION; ++i)
//    {
//        uqd += regularize(u[i]) * regularize(d[i]) * regularize(q[i]);
//    }
//    double divisor = 1.0 - examination * uqd;

//    gradient.examinations[rank] += uqd / (divisor + EPS);

//    double* gu = gradient.userEmbedding.get(user);
//    double* gd = gradient.docEmbedding.get(doc);
//    double* gq = gradient.queryEmbedding.get(query);
//    for (size_t i = 0; i < DIMENSION; ++i)
//    {
//        gu[i] += examination * regularize(d[i]) * regularize(q[i]) / (divisor + EPS);
//        gd[i] += examination * regularize(u[i]) * regularize(q[i]) / (divisor + EPS);
//        gq[i] += examination * regularize(u[i]) * regularize(d[i]) / (divisor + EPS);
//    }
//}

/********************************************************************/

static double regularize(double val, double min=MIN_EMBEDDING_VALUE, double max=MAX_EMBEDDING_VALUE)
{
    val = std::max(min, val);
    return std::min(max, val);
}

void Learner::makeOneStep__()
{
    for (size_t i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY; ++i)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                size_t query = serp.id;
                size_t user = serp.person;
                for (size_t i = 0; i < SERP_SIZE; ++i)
                {
                    size_t doc = serp.urls[i];
                    bool click = serp.type[i] == 2;
                    if (click) {
                        this->makePositiveUpdate__(query, user, doc, i);
                    } else {
                        this->makeNegativeUpdate__(query, user, doc, i);
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < SERP_SIZE; ++i)
    {
        std::cout << i << ":" << this->model.examinations[i] << " ";
        if (i % 4 == 1) std::cout << std::endl;
    }
    std::cout << "\n\nValue = " << this->CalculateValue(FIRST_TRAINING_DAY, FIRST_TRAINING_DAY) << std::endl;
    std::cout << "\n\nValue = " << this->CalculateValue(LAST_TRAINING_DAY + 1, LAST_TRAINING_DAY + 1) << std::endl;
}

void normalize(double* ar)
{
    double sum = 0;
    for (size_t i = 0; i < DIMENSION;++i)
    {
        sum += ar[i];
    }
    for (size_t i = 0; i < DIMENSION;++i)
    {
        ar[i] /= sum;
    }
}

void Learner::makePositiveUpdate__(size_t query, size_t user, size_t doc, size_t rank)
{
    model.examinations[rank] += LEARNING_RATE / (model.examinations[rank] + EPS);
    model.examinations[rank] = regularize(model.examinations[rank], 0.01, 0.99);
    if (std::isnan(model.examinations[rank]))
    {
        std::cout << "broken";
    }
    double* u = model.userEmbedding.get(user);
    double* d = model.docEmbedding.get(doc);
    double* q = model.queryEmbedding.get(query);
    double uqd = 0;
    for (size_t i = 0; i < DIMENSION; ++i)
    {
        uqd += u[i] * d[i] * q[i];
    }
    for (size_t i = 0; i < DIMENSION; ++i)
    {
        u[i] += LEARNING_RATE * d[i] * q[i] / (uqd + EPS);
        u[i] = regularize(u[i]);
        d[i] += LEARNING_RATE * u[i] * q[i] / (uqd + EPS);
        d[i] = regularize(d[i]);
        q[i] += LEARNING_RATE * u[i] * d[i] / (uqd + EPS);
        q[i] = regularize(q[i]);
        if (std::isnan(u[i]) || std::isnan(d[i]) || std::isnan(q[i]) ||
            std::isinf(u[i]) || std::isinf(d[i]) || std::isinf(q[i]))
        {
            std::cout << "broken";
        }
    }
    normalize(u);
    normalize(d);
    normalize(q);
}

void Learner::makeNegativeUpdate__(size_t query, size_t user, size_t doc, size_t rank)
{
    double examination = model.examinations[rank];
    double* u = model.userEmbedding.get(user);
    double* d = model.docEmbedding.get(doc);
    double* q = model.queryEmbedding.get(query);
    double uqd = 0;
    for (size_t i = 0; i < DIMENSION; ++i)
    {
        uqd += u[i] * d[i] * q[i];
    }
    double divisor = 1.0 - examination * uqd;

    model.examinations[rank] -= LEARNING_RATE * uqd / (divisor + EPS);
    model.examinations[rank] = regularize(model.examinations[rank], 0.01, 0.99);
    if (std::isnan(model.examinations[rank]))
    {
        std::cout << "broken";
    }

    for (size_t i = 0; i < DIMENSION; ++i)
    {
        u[i] -= LEARNING_RATE * examination * d[i] * q[i] / (divisor + EPS);
        u[i] = regularize(u[i]);
        d[i] -= LEARNING_RATE * examination * u[i] * q[i] / (divisor + EPS);
        d[i] = regularize(d[i]);
        q[i] -= LEARNING_RATE * examination * u[i] * d[i] / (divisor + EPS);
        q[i] = regularize(q[i]);
        if (std::isnan(u[i]) || std::isnan(d[i]) || std::isnan(q[i]) ||
            std::isinf(u[i]) || std::isinf(d[i]) || std::isinf(q[i]))
        {
            std::cout << "broken";
        }
    }

    normalize(u);
    normalize(d);
    normalize(q);
}



}
