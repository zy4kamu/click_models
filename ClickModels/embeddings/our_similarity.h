#ifndef OUR_SIMILARITY_H
#define OUR_SIMILARITY_H

#include <vector>
#include <math.h>
namespace our_similarity
{
double similarity(const std::vector<double>& x,const std::vector<double>& y);
std::vector<double> divSimilarity(const std::vector<double>& x,const std::vector<double>& y);

}
namespace Similarity
{
    double similarity(const std::vector<double>& x,const std::vector<double>& y);
    std::vector<double> divSimilarity(const std::vector<double>& x,const std::vector<double>& y);
}

namespace Cos
{
double similarity(const std::vector<double>& x,const std::vector<double>& y);
std::vector<double> divSimilarity(const std::vector<double>& x,const std::vector<double>& y);
void normalize(double* d, int dim);
void sum(double* ar, double* ar1, int dim, double rate);
}

namespace Examination
{
double Value(double x);
double Value(double x, double min_v, double max_v);
double DivValue(double x);
double DivValue(double x, double min_v, double max_v);
}
#endif // OUR_SIMILARITY_H
