#ifndef PERSONALIZEDCLICKMODEL_H
#define PERSONALIZEDCLICKMODEL_H
#include<unordered_map>
#include<vector>
#include <random>
#include "../day_data.h"

class PersonalizedClickModel
{
private:
    int dim;
    double sigma;
    double sigma_u;
    double sigma_q;
    double sigma_d;
    std::vector<double> examination;
    std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, double>>> attractivness_u_q_d;
    std::unordered_map<size_t, std::vector<double>> queries;
    std::unordered_map<size_t, std::vector<double>> documents;
    std::unordered_map<size_t, std::vector<double>> users;

    int end_day;
public:
    PersonalizedClickModel(int d, int e_day);
    double Attractivness(size_t user, size_t document, size_t query);
    double Probability_irrelevant(size_t user, size_t document, size_t query, size_t rank);
    void Update(const std::string& pathToDayData, double step);
    void Learn(double step);
    void Test();
};

#endif // PERSONALIZEDCLICKMODEL_H
