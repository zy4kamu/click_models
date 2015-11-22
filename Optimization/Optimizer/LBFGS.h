#ifndef LBFGS_H
#define LBFGS_H

#include <vector>
#include <omp.h>
#include <memory>

#include "c_lbfgs.h"
#include "IOptimizable.h"
#include "ICallable.h"

using std::shared_ptr;
using std::vector;

namespace Optimization
{

class LBFGS
{
public:
    LBFGS(void);
    ~LBFGS(void);
    double Optimize(
        const IOptimizable& function,
        ICallable* eventHandler,
        const vector<double>& initialPoint, 
        vector<double>* minPoint, int maxIterations,
        bool printInfo = true);

    static lbfgsfloatval_t Evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step);

    static int Progress(
        void *instance,
        const lbfgsfloatval_t *x,
        const lbfgsfloatval_t *g,
        const lbfgsfloatval_t fx,
        const lbfgsfloatval_t xnorm,
        const lbfgsfloatval_t gnorm,
        const lbfgsfloatval_t step,
        int n,
        int k,
        int ls);
private:
    static size_t currentIteration;
    static size_t maxIteration;
    static double currentTime;
    static ICallable* eventHandler;
    static bool shouldPrintInfo;
};

};

#endif

