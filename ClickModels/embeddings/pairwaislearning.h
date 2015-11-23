#ifndef PAIRWAISLEARNING_H
#define PAIRWAISLEARNING_H
#include<vector>

class pairwaisLearning
{
public:
    double Cost(double x, double y);
    double DivCost(double x, double y);
    std::vector<double> Get_Coeffs(const std::vector<double>& scores,
                                        const std::vector<bool>& truth);
};

#endif // PAIRWAISLEARNING_H
