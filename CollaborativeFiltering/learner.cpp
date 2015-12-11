#include "learner.h"
#include <cstring>
#include <cmath>
#include "uumap.h"
#include "embeddings/collaborative_filtering.h"

namespace collaborativefiltering
{
double last_result = 0;
Counters counters;
void Get_counters()
{
    for (int i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY ; ++i)
    {
        DayData dayData = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        calculate_counters(dayData, counters);
    }
}

Learner::Learner(const string& folder, size_t dimension)
    : model(folder, dimension)
    // , gradient(folder, dimension)
{
    Get_counters();//this->dimension = this->model.parameters.size();
}

void Learner::learn()
{
//    Optimization::LBFGS lbfgs;
//    vector<double> initialPoint = model.parameters;
//    vector<double> minPoint(initialPoint.size());
//    Optimization::ICallable eventHandler;
//    lbfgs.Optimize(*this, &eventHandler, initialPoint, &minPoint, 1000);
    int step = 0;
    while(step < 20)
    {
        this->makeOneStep__(step);
        step += 1;
    }
}

//uumap queryUser= uumap(PATH_TO_DATA + "query_user_1_25");
//uumap userUrl = uumap(PATH_TO_DATA + "query_user_1_25");
//uumap queryRank = uumap(PATH_TO_DATA + "query_user_1_25");

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
                if (!GetFilterForTest(counters.query_user, counters.user_url, counters.query_rank, serp))
                {
                    continue;
                }
                int serp_type  = serp.type[0] == 2 ? 1 : 0;
                top1Before += serp_type;

//                int model_prediction_rank = 0;
//                size_t query = serp.id;
//                size_t user = serp.person;
//                for (size_t i = 0; i < 10; ++i)
//                {
//                 const vector<double>& found = counters.user_url.watch(user, serp.urls[i]);
//                 if (found.size() > 0 && found[0] > 1 - 1e-5)
//                 {
//                     model_prediction_rank = i;
//                     break;
//                 }
//                }

//                int model_type = serp.type[model_prediction_rank] == 2 ? 1 : 0;
                int model_type = serp.type[this->model.sort(serp, counters)[0]] == 2 ? 1 : 0;
                sum += this->model.calculateLogLikelihood(serp);
                top1After += model_type;
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

void Learner::makeOneStep__(int step)
{
    double bias = 1;
    std::cout << step << " " << LEARNING_RATE << "\n";
    bool document_step = false;
    bool user_step = false;
    bool query_step = false;
    double new_result = this->CalculateValue(LAST_TRAINING_DAY, LAST_TRAINING_DAY);

    std::cout << "\n\nValue = " << new_result << std::endl;
    std::cout << "\n\nValue = " << this->CalculateValue(LAST_TRAINING_DAY + 1, LAST_TRAINING_DAY + 1) << std::endl;
    if (step >= 2)
    {
        std::cout << "QUERY STEP\n";
        query_step = true;
    }
    else if (step < 2)
    {
        std::cout << "DOCUMENT STEP\n";
        document_step = true;
    }
    else
    {
        std::cout << "USER STEP\n";
        user_step = true;
    }
    for (size_t i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY; ++i)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                if (!GetFilter(counters.query_user, counters.user_url, counters.query_rank, serp))
                    continue;
                if (query_step)
                {
                    this->UpgradeOneExample(serp, false, false, true);
                }
                if (document_step)
                {
                    //LEARNING_RATE /= sqrt(step + 1);
                    this->UpgradeOneExample(serp, true, false, false);
                    //LEARNING_RATE *= sqrt(step + 1);
                }
                else
                {
                    double coef = sqrt(step + 1);
                    if (step > 10) coef = step;
                    //LEARNING_RATE /= coef;
                    //if (LEARNING_RATE > -0.1) LEARNER_RATE = -0.1;
                    this->UpgradeOneExample(serp, true, true, false);
                    //LEARNING_RATE *= coef;
                }
//                size_t query = serp.id;
//                size_t user = serp.person;
//                for (size_t i = 0; i < SERP_SIZE; ++i)
//                {
//                    size_t doc = serp.urls[i];
//                    bool click = serp.type[i] == 2;
//                    if (click) {
//                        this->makePositiveUpdate__(query, user, doc, i);
//                    } else {
//                        this->makeNegativeUpdate__(query, user, doc, i);
//                    }
//                }
            }
        }
    }

    for (size_t i = 0; i < SERP_SIZE; ++i)
    {
        std::cout << i << ":" << this->model.examinations[i] << " ";
        if (i % 4 == 1) std::cout << std::endl;
    }
//    double new_result = this->CalculateValue(LAST_TRAINING_DAY, LAST_TRAINING_DAY);

//    std::cout << "\n\nValue = " << new_result << std::endl;
//    std::cout << "\n\nValue = " << this->CalculateValue(LAST_TRAINING_DAY + 1, LAST_TRAINING_DAY + 1) << std::endl;
//    if (step > 2)
//    {
//        std::cout << new_result << " " << last_result << "\n";
//
//    }
    if (new_result < last_result + 0.0001) LEARNING_RATE /= 1.2;
    last_result = new_result;
}

void normalize(double* ar)
{
    double sum = 0;
    double min_value = ar[0];
    for (size_t i = 0; i < DIMENSION;++i)
    {
        sum += ar[i];
        if (min_value > ar[i]) min_value = ar[i];
    }
    for (size_t i = 0; i < DIMENSION;++i)
    {
        ar[i] = (ar[i] - min_value+0.01) / (sum - 10*(min_value-0.01));
    }
}

//void normalize_cos(double* ar)
//{
//    double sum = 0;
//    double min_value = ar[1];
//    for (size_t i = 1; i < DIMENSION;++i)
//    {
//        if (min_value > ar[i])
//            min_value = ar[i];
//    }
//    for (size_t i = 1; i < DIMENSION;++i)
//    {
//        ar[i] -= (min_value - MIN_EMBEDDING_VALUE);
//        sum += ar[i] * ar[i];
//    }
//    for (size_t i = 1; i < DIMENSION;++i)
//    {
//        ar[i] /= sqrt(sum);
//    }
//}

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

void Learner::UpgradeOneExample(const Query& serp, bool change_document, bool change_user, bool change_query)
{
    size_t user = serp.person;
    size_t query = serp.id;

    double* u = model.userEmbedding.get(user);
    double* q = model.queryEmbedding.get(query);

    std::vector<bool> truth(10);
    for (int rank = 0; rank< 10; ++rank)
    {
        truth[rank] = serp.type[rank] == 2;
    }
    std::vector<double> scores = model.calculateClickProbabilitiesMy(serp, counters);
    for (int i = 0; i < 10; ++i)
    {
        const vector<double>& found = counters.user_url.watch(user, serp.urls[i]);
        if (found.size() > 0 && found[0] > 1 - 1e-5)
        {
            scores[i] /= 10;
        }
    }
    //int type_before = serp.type[this->model.sort(serp)[0]] == 2 ? 1 : 0;

    std::vector<double> coeffs = ranker.Get_Coeffs(scores, truth);
    vector<double> new_user(DIMENSION);
    vector<double> new_query(DIMENSION);
    vector<double> old_document(DIMENSION, 0);
    vector<double> new_document(DIMENSION, 0);
    std::memcpy(&new_user[0], this->model.userEmbedding.get(serp.person), DIMENSION*sizeof(double));
    std::memcpy(&new_query[0], this->model.queryEmbedding.get(serp.id), DIMENSION*sizeof(double));

    for (int rank = 0; rank < 10; ++rank)
    {
        double* d = model.docEmbedding.get(serp.urls[rank]);
        double uqd = 1;//Cos::similarity(std::vector<double>(u+1,u+DIMENSION), std::vector<double>(d+1, d+DIMENSION));
         for (int i = 0; i < DIMENSION; ++i)
         {
             if (change_user && i == 1)
             {
                 std::vector<double> div  = Cos::divSimilarity(std::vector<double>(u+1,u+DIMENSION),
                                                                          std::vector<double>(d+1, d+DIMENSION));
//                 for (int j = 1; j < DIMENSION; ++j)
//                 {
//                     double c = LEARNING_RATE * coeffs[rank] * model.examinations[rank] * u[0] * q[rank] * d[0];
//                     if (std::abs(c) > 1./ 4) c = 1./4 * (std::abs(c) / c);
//                     new_user[j] += c * div[j-1];

//                 }

             }
             if (change_query && i == 0)
             {
                new_query[rank] += LEARNING_RATE * coeffs[rank] * Examination::Value(d[0]) *
                        Examination::DivValue(q[rank]) *
                        uqd;
             }
             if (change_document)
             {
                 if (change_user && i == 1)
                 {
//                    std::vector<double> div  = our_similarity::divSimilarity(std::vector<double>(d+1,d+DIMENSION),
//                                                                             std::vector<double>(u+1, u+DIMENSION));
//                    std::vector<double> div(u+1,u+DIMENSION);

//                    for (int j = 1; j < DIMENSION; ++j)
//                    {
//                        if (rank == 0)
//                        {
//                            old_document[j] = d[j];
//                        }
//                        double c = LEARNING_RATE * coeffs[rank] * model.examinations[rank] * q[rank] * d[0];
//                        //if (std::abs(c) > 1./ 4) c = 1./4 * (std::abs(c) / c);
//                        d[j] += c * div[j-1];
//                        if (rank == 0)
//                        {
//                            new_document[j] = d[j];
//                        }
//                    }
                 }
                 if (i == 0 && !change_user)
                 {
                     double c = LEARNING_RATE * coeffs[rank]  *
                             Examination::Value(q[rank]) * uqd *
                             Examination::DivValue(d[0]);
                     d[0] += c;


                 }
             }
             //normalize_cos(&d[1]);
         }
         //normalize(d);
//         model.examinations[rank] = (examination[rank] + LEARNING_RATE * coeffs[rank] * uqd *
//                 model.examinations[rank] * model.examinations[rank] * exp(-examination[rank]));
    }
    std::memcpy(this->model.userEmbedding.get(serp.person), &new_user[0], DIMENSION*sizeof(double));
    std::memcpy(this->model.queryEmbedding.get(serp.id), &new_query[0], DIMENSION*sizeof(double));

    std::vector<double> scores1 = model.calculateClickProbabilitiesMy(serp, counters);
    //int type_after = serp.type[this->model.sort(serp)[0]] == 2 ? 1 : 0;
    if (false)
    {
        std::cout << "COEFFS\n";
        for (int i = 0; i < 10; ++i)
        {
            std::cout << truth[i] << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < 10; ++i)
        {
            std::cout << scores[i] << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < 10; ++i)
        {
            std::cout << scores1[i] << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < DIMENSION; ++i)
        {
            std::cout << old_document[i] << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < DIMENSION; ++i)
        {
            std::cout << new_document[i] << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < DIMENSION; ++i)
        {
            std::cout << u[i] << " ";
        }
        std::cout << "\n";
        std::cout << "\n";
        for (int i = 0; i < 10; ++i)
        {
            std::cout << coeffs[i] << " ";
        }
        std::cout << "\n";
        int del;
        std::cin >> del;
    }

}

}
