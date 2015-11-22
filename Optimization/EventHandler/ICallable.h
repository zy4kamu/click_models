#ifndef I_CALLABLE
#define I_CALLABLE

#include <iostream>

namespace Optimization
{

class ICallable
{
public:
    virtual bool Call(int currentIteration
                      , const double* parameters
                      , int dimension)
    {
        return false;
    }
};

};

#endif // I_CALLABLE
