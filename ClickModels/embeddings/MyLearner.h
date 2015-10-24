#ifndef MYLEARNER
#define MYLEARNER
#include <random>
#include <thread>
#include <mutex>
#include "utils.h"
#include "Token.h"
#include "Tools.h"

class MyLearner
{
public:
    MyLearner(
        const string& usersFile,
        std::function<double(double)> probFunctor,
        std::function<double(double)> divLogFunctor,
        size_t dimension);

    //MyLearner(const string& file);
    void Print(const string& file);
    vector<std::pair<size_t, double> > GetNearest(size_t token, size_t number);
    vector<std::pair<size_t, double> > GetNearest(size_t index,
        size_t number, const unordered_set<size_t>& userList);
    void MakeOnePositiveStep(size_t first, size_t second, double rate);
    void MakeOneNegativeStep(size_t first, size_t second, double rate);
    double Distance(size_t first, size_t second);
private:
    Tools::TableCalculator<double, double> probFunctorCalculator;
    Tools::TableCalculator<double, double> divLogFunctorCalculator;
    size_t dimension;
    vector<Token> tokens;
    unordered_map<size_t, size_t> tokenToIndex;

    std::default_random_engine generator;
    std::discrete_distribution<size_t> distribution;
    std::discrete_distribution<size_t> documentDistribution;
    std::uniform_real_distribution<double> uniformDistribution;
};


#endif
