#include "our_similarity.h"
#include <cmath>

namespace our_similarity
{
double similarity(const std::vector<double>& x,const std::vector<double>& y,
                  double min_value, double max_value)
{
    double res = 0;
    bool cout_ = false;
    for (size_t i = 0; i < x.size(); ++i)
    {
        res += (x[i] - y[i]) * (x[i] - y[i]);
        if (x[i] != x[i]) cout_ = true;
        if (y[i] != y[i]) cout_ = true;
    }
    res = exp(-sqrt(res));
    double a = min_value / (max_value - min_value);
    double b = 1./ (max_value - min_value);
    return (res + a) / b;

}

std::vector<double> divSimilarity(const std::vector<double>& x,const std::vector<double>& y,
                                  double min_value, double max_value)
{
    double coef = 0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        coef += (x[i] - y[i]) * (x[i] - y[i]);
    }
    coef = sqrt(coef);
    coef = exp(-coef) * (-1./coef);
    coef /= (max_value - min_value);
    //if (std::abs(coef) > 1) coef = coef / std::abs(coef);
    std::vector<double> res_v(x.size(), 0.);
    for (size_t i = 0; i < x.size(); ++i)
    {
        res_v[i] = (x[i] - y[i]) * coef;

    }
    return res_v;
}

}

namespace Examination
{
double Value(double x)
{
    double res = 1./ (1 + std::exp(-x));
    res += 1./9;
    return res /= (1 + 1./9);
}
double DivValue(double x)
{
    double res = 1./ (1 + std::exp(-x));
    res *= res;
    res *= std::exp(-x);
    res /= (1. + 1./9);
    return res;

}
double Value(double x, double min_v, double max_v)
{
    double res = 1./ (1 + std::exp(-x));
    double a = min_v / (max_v - min_v);
    res += a;
    res /= (1 + a);
    res *= max_v;
    return res;
}

double DivValue(double x, double min_v, double max_v)
{
    double res = 1./ (1 + std::exp(-x));
    res *= res;
    res *= std::exp(-x);
    double a = min_v / (max_v - min_v);
    res /= (1. + a);
    res *= max_v;
    return res;
}
}
namespace Cos
{
double similarity(const std::vector<double>& x,const std::vector<double>& y)
{
    double res = 0;
    double x_norm = 1e-10;
    double y_norm = 1e-10;
    for (size_t i = 0; i < x.size(); ++i)
    {
        res += (x[i]) * (y[i]);
        x_norm += x[i] * x[i];
        y_norm += y[i] * y[i];
    }
//    if (std::abs(res) < 1e-10)
//    {
//        res = (res / (std::abs(res) + 1e-10)) + 1e-10;
//    }
//    return res / sqrt(x_norm * y_norm);
    return res;

}

std::vector<double> divSimilarity(const std::vector<double>& x,const std::vector<double>& y)
{

    std::vector<double> res_v(x.size(), 0.);
    for (size_t i = 0; i < x.size(); ++i)
    {
        res_v[i] = y[i];
    }
    return res_v;
}

void normalize(double* ar, int dim)
{
    double sum = 1e-10;
    double min_value = ar[0];
    for (size_t i = 0; i < dim;++i)
    {
        sum += ar[i] * ar[i];
    }
    for (size_t i = 0; i < dim;++i)
    {
        ar[i] /= sqrt(sum);
    }
}

void normalize(std::vector<double>& d)
{
    double sum = 1e-10;
    double min_value = d[0];
    int dim = d.size();
    for (size_t i = 0; i < dim;++i)
    {
        sum += d[i] * d[i];
    }
    for (size_t i = 0; i < dim;++i)
    {
        d[i] /= sqrt(sum);
    }
}

void sum(double* ar, double* ar1, int dim, double rate)
{
    double sum = 0;
    double min_value = ar[0];
    for (size_t i = 0; i < dim;++i)
    {
        ar[i] += ar1[i]*rate;
    }
}
}

