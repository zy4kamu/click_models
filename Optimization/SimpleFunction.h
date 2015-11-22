#ifndef SIMPLE_FUNCTION
#define SIMPLE_FUNCTION

#include "IOptimizable.h"

namespace Optimization
{

class SimpleFunction : public IOptimizable
{
public:
    SimpleFunction(int dimension);
    ~SimpleFunction(void);
    double CalculateValue(const vector<double>& point);
    void CalculateGradient(const vector<double>& point, vector<double>* gradient);
};

};

#endif

