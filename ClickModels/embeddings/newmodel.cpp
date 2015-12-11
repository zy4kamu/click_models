#include "newmodel.h"

static double last_result = 0;
static Counters counters1;
void Get_counters()
{
    for (int i = Params::FIRST_TRAINING_DAY; i <= Params::LAST_TRAINING_DAY ; ++i)
    {
        DayData dayData = read_day(Params::DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        calculate_counters(dayData, counters1);
    }
}

void Data::Put(int key1, int key2, double value)
{
    auto found = data.find(key1);
    if (found == data.end())
    {
        data[key1];
    }
    found = data.find(key1);
    found->second[key2] = value;
}

double Data::Get(int key1, int key2)
{
    auto found = data.find(key1);
    if (found == data.end())
    {
        return 0;
    }
    auto found1 = found->second.find(key2);
    if (found1 == found->second.end())
    {
        return 0;
    }
    return found->second[key2];
}

NewModel::NewModel()
{
    Get_counters();
    std::vector<double> ex(10, 0);
    for (size_t i =  Params::FIRST_TRAINING_DAY; i <=  Params::LAST_TRAINING_DAY+1; ++i)
    {
        DayData data = read_day( Params::DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                std::vector<double> rank(10, 0);
                std::vector<double> evristic = Evristic(serp);
                double sum_rank = 1e-10;
                double sum_evristic = 1e-10;
                for (int j = 0; j < Params::SERP_SIZE; ++j)
                {
                    if (counters1.query_rank.watch(serp.id, j).size() > 0)
                    {
                        sum_rank += counters1.query_rank.watch(serp.id, j)[0];
                        sum_evristic += evristic[j];
                        rank[j] = counters1.query_rank.watch(serp.id, j)[0];
                    }
                }
                query_examination[serp.id] = ex;
//                for (int j = 0; j < Params::SERP_SIZE; ++j)
//                {
//                   query_document.Put(serp.id, serp.urls[j], evristic[j] / sum_evristic);
//                   if (serp.type[i] >= 1)
//                   {
//                        user_document.Put(serp.person, serp.urls[i], 1.);
//                   }
//                   query_examination[serp.id][j] = rank[j] / sum_rank;
//                }

//                int frequency = 0;
//                if (frequency_query.find(serp.id) != frequency_query.end())
//                {
//                    frequency = frequency_query[serp.id];
//                }
//                frequency_examination[frequency+1] = ex;
//                frequency_query[serp.id] = frequency + 1;
//                if (i <= Params::LAST_TRAINING_DAY)
//                {
//                    for (int j = 0; j < Params::SERP_SIZE; ++j)
//                    {
//                        if (serp.type[j] == 2)
//                        {
//                            user_document.Put(serp.person, serp.urls[j], 100);
//                        }
//                    }
//                }

            }
        }
    }
}

std::vector<double> NewModel::calculateClickProbabilities(const Query& serp)
{
    double max_value = Params::MAX_EMBEDDING_VALUE;
    double min_value = Params::MIN_EMBEDDING_VALUE;
    int query = serp.id;
    int user = serp.person;
    std::vector<double> res(10, 0);
//    int frequency = 0;
//    if (frequency_query.find(serp.id) != frequency_query.end())
//    {
//        frequency = frequency_query[serp.id];
//    }
    for (int i = 0; i < 10; ++i)
    {
        int document = serp.urls[i];
        res[i] = Examination::Value(user_document.Get(user, document));
        res[i] *= Examination::Value(query_document.Get(query, document), min_value, max_value);
        res[i] *= Examination::Value(query_examination[query][i], min_value, max_value);
        //res[i] *= Examination::Value(frequency_examination[frequency][i], min_value, max_value);
    }
//    std::cout << "\n";
//    for (int i = 0; i < 10; ++i)
//    {
//        std::cout << serp.type[i] <<  " ";
//    }
//    std::cout << "\n\n";
    return res;
}

void NewModel::learn()
{
    int step = 0;
    while(step < 500)
    {
        this->makeOneStep(step);
        step += 1;
    }
}

void NewModel::makeOneStep(int step)
{
    double bias = 1;
    std::cout << step << " " << Params::LEARNING_RATE << "\n";
    bool document_step = false;
    bool user_step = false;
    bool query_step = false;
    double new_result = this->CalculateValue( Params::LAST_TRAINING_DAY, Params::LAST_TRAINING_DAY);

    std::cout << "\n\nValue = " << new_result << std::endl;
    std::cout << "\n\nValue = " << CalculateValue(Params::LAST_TRAINING_DAY + 1,
                                                   Params::LAST_TRAINING_DAY + 1) << std::endl;
    if (step < 5)
    {
        std::cout << "QUERY STEP\n";
        query_step = true;
    }
    else if (step >= 5 && step < 15)
    {
        std::cout << "DOCUMENT STEP\n";
        document_step = true;
    }
    else if (step >= 15)
    {
        std::cout << "USER STEP\n";
        user_step = true;
    }
    for (size_t i =  Params::FIRST_TRAINING_DAY; i <=  Params::LAST_TRAINING_DAY; ++i)
    {
        DayData data = read_day( Params::DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                if (!GetFilter(counters1.query_user, counters1.user_url, counters1.query_rank, serp))
                    continue;
                if (query_step)
                {
                    //Params::LEARNING_RATE /= sqrt(step);
                    this->UpgradeOneExample(serp, false, false, true);
                    //Params::LEARNING_RATE *= sqrt(step);
                }
                else if (document_step)
                {
                    //Params::LEARNING_RATE /= sqrt(step-8);
                    this->UpgradeOneExample(serp, true, false, false);
                    //Params::LEARNING_RATE *= sqrt(step-8);
                }
                else if (user_step)
                {
                    double coef = sqrt(step + 1);
                    if (step > 10) coef = step;
                    //Params::LEARNING_RATE /= sqrt(step-12);
                    this->UpgradeOneExample(serp, false, true, false);
                    //Params::LEARNING_RATE *= sqrt(step-12);
                }
//                else
//                {
//                    this->UpgradeOneExample(serp, false, false, false);
//                }
            }
        }
    }
    last_result = new_result;
}

vector<size_t> NewModel::sort(const std::vector<double>& probs)
{
    //vector<double> probs = this->calculateClickProbabilities(serp);
    vector<size_t> indecies(probs.size());
    for (size_t i = 0; i < probs.size(); ++i) indecies[i] = i;
    std::sort(indecies.begin(), indecies.end(),
        [&](size_t i1, size_t i2) {return probs[i1] > probs[i2]; });
    return indecies;
}

std::vector<double> NewModel::Evristic(const Query& serp)
{
    std::vector<double> evristic(10, 0);
    int query = serp.id;
    for (int i = 0; i < 10; ++i)
    {
      const unordered_map<size_t, vector<double> >& vec1 = counters1.query_url.watch(query);
      auto vec2 = vec1.find(serp.urls[i]);
      if (vec2 != vec1.end() && vec2->second.size() > 0)
      {
        evristic[i] = vec2->second[1];
        //std::cout << vec2->second[1] << " " << vec2->second[2] << " " << serp.type[i] <<"\n";
      }
    }
    return evristic;
}

std::vector<double> NewModel::DoubleClick(const Query& serp)
{
    std::vector<double> evristic(10, 0);
    int user = serp.person;
    for (int i = 0; i < 10; ++i)
    {
        const vector<double>& found = counters1.user_url.watch(user, serp.urls[i]);
        if (found.size() > 0 && found[0] > 1 - 1e-5)
        {
            evristic[i] = 1;
        }
    }
    return evristic;
}

void NewModel::Get_tops(const Query& serp,
        const std::vector<size_t>& probs, std::vector<double>& res)
{
    bool right_answer = false;
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        if (serp.type[probs[i]] == 2)
        {
            right_answer = true;
        }
        if (right_answer)
        {
            res[i] += 1;
        }
    }

}

std::vector<size_t> NewModel::Merge_two_rankers(const std::vector<size_t>& b,
                                                const std::vector<size_t>& a)

{
    int length = 0;
    int a_elements = 0;
    int b_elements = 0;
    std::vector<size_t> result(10,0);
    std::set<int> indexes;
    while (length < Params::SERP_SIZE)
    {
        int element = a[a_elements];
        int s = a_elements;
        ++a_elements;
        if (a_elements > b_elements)
        {
            s = b_elements;
            element = b[s];
            --a_elements;
            ++b_elements;
        }
        if (indexes.count(element) == 0)
        {
            result[length] = element;
            indexes.insert(element);
            ++length;
        }
    }
    return result;
}

double NewModel::Get_NDCG(const Query& serp,
        const std::vector<size_t>& probs)
{

    double res = 1e-10;
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        int type = 0;
        if (serp.type[probs[i]] == 1)
        {
            type = 1;
        }
        if (serp.type[probs[i]] == 2)
        {
            type = 2;
        }
        double d = 1.;
        for (int t = 0; t < type; ++t)
        {
            d *= 2;
        }
        res += (d-1.) / (std::log(i+2.) / std::log(2.));
    }
    return res;

}

double NewModel::CalculateValue(size_t start, size_t end)
{
    size_t enumerator = 0;
    double sum = 0;
    std::vector<double> top1Before(10,0);
    std::vector<double> top1After(10, 0);
    std::vector<double> top1Evristic(10, 0);
    std::vector<double> top1DoubleClick(10, 0);
    std::vector<double> top1AfterBefore(10, 0);

    double NDCGBefore = 0;
    double NDCGAfter = 0;
    double NDCGEvristic = 0;
    double NDCGDoubleClick = 0;
    double NDCGAfterBefore = 0;

    for (size_t i = start; i <= end; ++i)
    {
        DayData data = read_day(Params::DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                if (!GetFilterForTest(counters1.query_user, counters1.user_url, counters1.query_rank, serp))
                {
                    continue;
                }

                std::vector<double> probs = calculateClickProbabilities(serp);
                std::vector<double> evristic = Evristic(serp);
                std::vector<double> dclick = DoubleClick(serp);
                for (int i = 0; i < 10; ++i)
                {
//                    if (counters1.query_rank.watch(serp.id, i).size() > 0)
//                    {
//                        evristic[i] *= counters1.query_rank.watch(serp.id, i)[0];
//                    }
//                    if (dclick[i] > 0)
//                    {
//                        evristic[i] *= 1000;
//                    }
                }
                std::vector<double> r(10, 0.);
                std::vector<double> ideal(10, 0);
                for (int i = 0; i < 10; ++i)
                {
                    ideal[i] = serp.type[i];
                }
                double ideal_DCG = Get_NDCG(serp, sort(ideal));

                Get_tops(serp,sort(probs), top1After);
                Get_tops(serp, sort(evristic), top1Evristic);
                Get_tops(serp, sort(dclick), top1DoubleClick);
                Get_tops(serp, sort(r), top1Before);
                Get_tops(serp, Merge_two_rankers(sort(probs) , sort(r)), top1AfterBefore);

                NDCGAfter +=  (Get_NDCG(serp,sort(probs)) + 1e-10) / (ideal_DCG + 1e-10);
                NDCGEvristic += (Get_NDCG(serp, sort(evristic)) + 1e-10) / (ideal_DCG + 1e-10);
                NDCGDoubleClick += (Get_NDCG(serp, sort(dclick)) + 1e-10) / (ideal_DCG + 1e-10);
                NDCGBefore += (Get_NDCG(serp, sort(r))+ 1e-10) / (ideal_DCG + 1e-10);
                NDCGAfterBefore += (Get_NDCG(serp, Merge_two_rankers(sort(probs) , sort(r)))+ 1e-10) / (ideal_DCG + 1e-10);
                enumerator += 1;
            }
        }
    }
    std::cout << enumerator << "\n";
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        std::cout << top1Before[i] / enumerator << " ";
    }
    std::cout << " " << NDCGBefore/enumerator << "\n";
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        std::cout << top1After[i] / enumerator << " ";
    }
    std::cout << " " << NDCGAfter/enumerator << "\n";
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        std::cout << top1Evristic[i] / enumerator << " ";
    }
    std::cout << " " << NDCGEvristic/enumerator <<"\n";
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        std::cout << top1AfterBefore[i] / enumerator << " ";
    }
    std::cout << " " << NDCGAfterBefore/enumerator <<"\n";
    for (int i = 0; i < Params::SERP_SIZE; ++i)
    {
        std::cout << top1DoubleClick[i] / enumerator << " ";
    }
    std::cout << " " << NDCGDoubleClick / enumerator << "\n\n";

    return sum / double(enumerator);
}

void NewModel::UpgradeOneExample(const Query& serp, bool change_document, bool change_user, bool change_query)
{
    size_t user = serp.person;
    size_t query = serp.id;
    double max_value = Params::MAX_EMBEDDING_VALUE;
    double min_value = Params::MIN_EMBEDDING_VALUE;

//    int frequency = 0;
//    if (frequency_query.find(serp.id) != frequency_query.end())
//    {
//        frequency = frequency_query[serp.id];
//    }

    std::vector<bool> truth(10);
    for (int rank = 0; rank< 10; ++rank)
    {
        truth[rank] = serp.type[rank] >= 2;
    }
    std::vector<double> scores = calculateClickProbabilities(serp);

    std::vector<double> coeffs = ranker.Get_Coeffs(scores, truth);

    for (int rank = 0; rank < 10; ++rank)
    {
        int d = serp.urls[rank];
        if (change_user)
        {
            double u_d = user_document.Get(user, d);
            u_d += Params::LEARNING_RATE * coeffs[rank] *
                    Examination::Value(query_document.Get(query, d), min_value, max_value) *
                    Examination::Value(query_examination[query][rank], min_value, max_value) *
                    Examination::DivValue(user_document.Get(user, d));
                    //Examination::Value(frequency_examination[frequency][rank], min_value, max_value);;
            user_document.Put(user, d, u_d);

        }
        else if (change_query)
        {
            double q = query_examination[query][rank];
            q += Params::LEARNING_RATE * coeffs[rank] *
                    Examination::Value(query_document.Get(query, d), min_value, max_value) *
                    Examination::DivValue(query_examination[query][rank], min_value, max_value) *
                    Examination::Value(user_document.Get(user, d));
                   // Examination::Value(frequency_examination[frequency][rank], min_value, max_value);;
            query_examination[query][rank] = q;

        }
        else if (change_document)
        {
            double q_d = query_document.Get(query, d);
            q_d += Params::LEARNING_RATE * coeffs[rank] *
                    Examination::DivValue(query_document.Get(query, d), min_value, max_value) *
                    Examination::Value(query_examination[query][rank], min_value, max_value) *
                    Examination::Value(user_document.Get(user, d));
                    //Examination::Value(frequency_examination[frequency][rank], min_value, max_value);
            query_document.Put(query, d, q_d);

        }
//        else
//        {
//            double f_e = frequency_examination[frequency][rank];
//            f_e += Params::LEARNING_RATE * coeffs[rank] *
//                    Examination::Value(query_document.Get(query, d), min_value, max_value) *
//                    Examination::Value(query_examination[query][rank], min_value, max_value) *
//                    Examination::Value(user_document.Get(user, d)) *
//                    Examination::DivValue(frequency_examination[frequency][rank], min_value, max_value);;
//            frequency_examination[frequency][rank] = f_e;
//        }
    }

}
