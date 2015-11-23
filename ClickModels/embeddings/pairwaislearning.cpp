#include "pairwaislearning.h"
#include <math.h>


double pairwaisLearning::Cost(double x, double y)
{
    double del = 1./ (1 + exp(-(x-y)));
    if (del < 1e-5) del = 1e-5;
    return log(del);
}

double pairwaisLearning::DivCost(double x, double y)
{
    return -1./ (1 + exp(-(x-y)));
}

std::vector<double> pairwaisLearning::Get_Coeffs(const std::vector<double>& scores,
                                    const std::vector<bool>& truth)
{
    std::vector<double> coeffs(10, 0);
    for (int rang = 0; rang < 10; ++rang)
    {
        int rang_truth = truth[rang];
        for (int rang1 = rang+1; rang1 < 10; ++rang1)
        {
            int rang1_truth = truth[rang1];
            if (rang_truth != rang1_truth)
            {
                double d_cost = DivCost(scores[rang], scores[rang1]);
                if (rang_truth > rang1_truth)
                {
                    coeffs[rang] += d_cost;
                    coeffs[rang1] -= d_cost;
                }
                else
                {
                    coeffs[rang] -= d_cost;
                    coeffs[rang1] += d_cost;
                }

            }
        }
    }
    return coeffs;

}

