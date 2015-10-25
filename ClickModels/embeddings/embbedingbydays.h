#ifndef EMBBEDINGBYDAYS_H
#define EMBBEDINGBYDAYS_H
#include "pairspreperation.h"


class EmbbedingByDays
{
private:
     string pairsfile;
     string daysDirectory;
     string embeddingFile;
     string outDirectory;
     Counters counters;
     MyLearner learner;

public:
    EmbbedingByDays(const string& usersFile,
                     const string& pairsfile_,  const string& embeddingFile_,
                     const string& daysDirectory_, const string& outDirectory_,
                     std::function<double(double)> probFunctor,
                     std::function<double(double)> divLogFunctor);
    void OneLearningStep(const string& pathToDayData, int day);
    void RunLearn(int end_day);
    MyLearner& getLearner();

};

#endif // EMBBEDINGBYDAYS_H
/*1.Count counters
2.Create Paires
3.Learn*/

