#include "embbedingbydays.h"
//"/Users/annasepliaraskaia/Desktop/work/kaggle_yandex/big_data/users"
EmbbedingByDays::EmbbedingByDays(const string& usersFile,
                                 const string& pairsfile_,  const string& embeddingFile_,
                                 const string& daysDirectory_, const string& outDirectory_,
                                 std::function<double(double)> probFunctor,
                                 std::function<double(double)> divLogFunctor):
    pairsfile(pairsfile_), embeddingFile(embeddingFile_), daysDirectory(daysDirectory_),
    outDirectory(outDirectory_),
    learner(usersFile, probFunctor, divLogFunctor, 90)
{
    string out_directory = daysDirectory;
    for (int i = 1; i < 20; ++i)
    {
        DayData dayData = read_day(out_directory + std::to_string(i) + ".txt");
        calculate_counters(dayData, counters);
    }
}

void EmbbedingByDays::OneLearningStep(const string& pathToDayData, int day)
{
    DayData dayData = read_day(pathToDayData);
    PreparePairs(pairsfile, counters.query_user, counters.user_url, counters.query_rank, dayData);
   // GetHistogramm(learner, day);
    Learn(learner, outDirectory, pairsfile, day);
    calculate_counters(dayData, counters);
}

void EmbbedingByDays::RunLearn(int end_day)
{
    string out_directory = daysDirectory;
    for (int i = 20; i < end_day; ++i)
    {
        std::cout << "Get day " << i << endl;
        OneLearningStep(out_directory + std::to_string(i) + ".txt", i);
    }


    /*for (int j = 0; j < 10; ++j)
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
        const string outFile = "/tmp/xxx";
        PreparePairs(outFile, counters.query_user, counters.user_url, counters.query_rank, dayData);
        //GetHistogramm(learner, j);
    }*/
    this->counters.clear();
    learner.Print(embeddingFile);
}

MyLearner& EmbbedingByDays::getLearner()
{
    return learner;
}
