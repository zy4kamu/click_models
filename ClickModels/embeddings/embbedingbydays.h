#ifndef EMBBEDINGBYDAYS_H
#define EMBBEDINGBYDAYS_H
#include "pairspreperation.h"


class EmbbedingByDays
{
private:
     Counters counters;
     MyLearner learner;

public:
    EmbbedingByDays(const string& usersFile,
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

