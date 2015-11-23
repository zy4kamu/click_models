#include "collaborative_filtering.h"
#include "macro_parameters.h"
#include "IOptimizable.h"

namespace collaborativefiltering
{

class Learner // : public Optimization::IOptimizable
{
public:
    Learner(const string& folder, size_t dimension);
    double CalculateValue(size_t start, size_t end);
//    void CalculateGradient(const vector<double>& point,
//        vector<double>* gradient);
    void learn();


    void makeOneStep__();
    void makePositiveUpdate__(size_t query, size_t user, size_t doc, size_t rank);
    void makeNegativeUpdate__(size_t query, size_t user, size_t doc, size_t rank);
private:
//    void calculateGradient();
//    void makePositiveUpdate(size_t query, size_t user, size_t doc, size_t rank);
//    void makeNegativeUpdate(size_t query, size_t user, size_t doc, size_t rank);
//    CollaborativeFiltering gradient;
    CollaborativeFiltering model;
};

}
