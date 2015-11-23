#include<vector>
#include "pairwaislearning.h"
#include "filters.h"
#include<fstream>

class R
{
private:
    std::vector<std::vector<double>> features;
    std::vector<double> parameters;
    pairwaisLearning ranking;
private:
    std::vector<std::vector<double>> Get_features_by_rank(const std::vector<double>& features,
                                                            std::vector<bool>& truth);
    double Get_score(const std::vector<double>& feature);
 public:
    double rate;
 public:
    std::vector<double> Get_scores(const std::vector<std::vector<double>>& features_by_rank);
    void One_step(const std::vector<double>& feature);
    R(const std::string& data_file);
    void Learn(const std::string& train_data, const std::string& test_data);
    void GetResult(const std::vector<bool>& truth, const std::vector<double>& evristic, Result& r);
    void Test(const std::string& data_file);



};
