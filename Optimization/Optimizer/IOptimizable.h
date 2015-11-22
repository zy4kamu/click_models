#ifndef IOPTIMIZABLE
#define IOPTIMIZABLE

#include <vector>

using std::vector;

namespace Optimization
{

class IOptimizable
{
public:
    IOptimizable();
    IOptimizable(int dimension);
    virtual ~IOptimizable(void);
    virtual double CalculateValue(const vector<double>& point) = 0;
    virtual void CalculateGradient(
        const vector<double>& point, vector<double>* gradient) = 0;
    inline int GetDimension() const { return dimension; }
protected:
    int dimension;
};

};

#endif

