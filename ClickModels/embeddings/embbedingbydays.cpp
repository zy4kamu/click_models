#include "embbedingbydays.h"
//"/Users/annasepliaraskaia/Desktop/work/kaggle_yandex/big_data/users"
EmbbedingByDays::EmbbedingByDays(const string& usersFile,
                                 std::function<double(double)> probFunctor,
                                 std::function<double(double)> divLogFunctor):
    learner(usersFile, probFunctor, divLogFunctor, 90)
{
    //std::function<double(double)> probFunctor = [](double x) -> double { return std::exp(-x); };
    //std::function<double(double)> divLogFunctor = [](double x) -> double { return -1; };

    string out_directory = "/Users/annasepliaraskaia/Desktop/work/data_by_days/";
    for (int i = 1; i < 20; ++i)
    {
        DayData dayData = read_day(out_directory + std::to_string(i) + ".txt");
        calculate_counters(dayData, counters);
    }
}

void EmbbedingByDays::OneLearningStep(const string& pathToDayData, int day)
{
    DayData dayData = read_day(pathToDayData);
    PreparePairs(counters.query_user, counters.user_url, counters.query_rank, dayData);
   // GetHistogramm(learner, day);
    Learn(learner, day);
    calculate_counters(dayData, counters);
}

void EmbbedingByDays::RunLearn(int end_day)
{
    string out_directory = "/Users/annasepliaraskaia/Desktop/work/data_by_days/";
    DayData dayData = read_day(out_directory + std::to_string(end_day) + ".txt");
    for (int i = 20; i < end_day; ++i)
    {
        std::cout << "Get day " << i << endl;
        OneLearningStep(out_directory + std::to_string(i) + ".txt", i);
    }


    for (int j = 0; j < 10; ++j)
    {
        counters.clear();
        for (int i = 1; i < 20; ++i)
        {
            DayData dayData = read_day(out_directory + std::to_string(i) + ".txt");
            calculate_counters(dayData, counters);
        }
        for (int i = 20; i < end_day; ++i)
        {
            std::cout << "Get day " << i << endl;
            OneLearningStep(out_directory + std::to_string(i) + ".txt", i);
        }
        PreparePairs(counters.query_user, counters.user_url, counters.query_rank, dayData);
        //GetHistogramm(learner, j);
    }
    learner.Print("/Users/annasepliaraskaia/Desktop/work/embedding/1");
}

MyLearner& EmbbedingByDays::getLearner()
{
    return learner;
}
