#include "mergecf_ubm.h"
#include <algorithm>

R::R(const std::string& data_file)
{
    std::ifstream in(data_file);

    while (!in.eof())
    {
        std::vector<double> feature(51, 0);
        for (int i = 0; i < feature.size(); ++i)
        {
            in >> feature[i];
        }
        features.push_back(feature);
        double d;
        in >> d;
    }
    in.close();
    parameters.resize(1);
    for (int i = 0; i < parameters.size(); ++i)
    {
        parameters[i] = 0.5;
    }
}

std::vector<std::vector<double>> R::Get_features_by_rank(const std::vector<double>& features,
                                                        std::vector<bool>& truth)
{
    std::vector<std::vector<double>> res(10);
    truth.clear();
    for (int i = 0; i < 10; ++i)
    {
//        res[i].push_back(features[i]);
//        res[i].push_back(features[10 + i]);
//        res[i].push_back(features[31 + i]);
//        res[i].push_back(features[31 + i] * features[10 + i] * features[i]);
//        res[i].push_back(features[31 + i] * features[10 + i]);
//        res[i].push_back(features[31 + i] * features[i]);
//        res[i].push_back(features[10 + i] * features[i]);
        res[i].push_back(features[20 + i] * features[10 + i] * features[i]);
    }
//    for (int i = 31; i < 41; ++i)
//    {
//        res[(i-1)%10].push_back(features[i]);
//        res[(i-1)%10].push_back(std::log(features[i] + 1e-10));
//    }
    for (int i = 41; i < 51; ++i)
    {
        truth.push_back(features[i] > 0.5);
    }
    return res;

}

double R::Get_score(const std::vector<double>& feature)
{
    double res = 0;
    for (int i = 0; i < feature.size(); ++i)
    {
        res += feature[i] * parameters[i];
    }
    return res;
}

std::vector<double> R::Get_scores(const std::vector<std::vector<double>>& features_by_rank)
{

    std::vector<double> scores(10,0.);
    for (int i = 0; i < 10; ++i)
    {
        scores[i] = Get_score(features_by_rank[i]);
    }
    return scores;

}

void R::One_step(const std::vector<double>& feature)
{
    std::vector<bool> truth;
    std::vector<std::vector<double>> features_by_rank =
            Get_features_by_rank(feature, truth);
    std::vector<double> scores = Get_scores(features_by_rank);
    std::vector<double> coeffs = ranking.Get_Coeffs(scores, truth);
    for (int i = 0; i < parameters.size(); ++i)
    {
        double old_param = parameters[i];
        for (int rank = 0; rank < 10; ++rank)
        {
           parameters[i] += rate * features_by_rank[rank][i] * coeffs[rank];
        }
    }

}

void R::Learn(const std::string& train_data, const std::string& test_data)
{
    for (int i = 0; i < features.size(); ++i)
    {
        if (i%100000 == 0) std::cout << i << std::endl;
        One_step(features[i]);
    }
    for (int i = 0; i < parameters.size(); ++i)
    {
        std:: cout << parameters[i] << " ";
    }
    std::cout << std::endl;
    Test(train_data);
    Test(test_data);
}

void R::GetResult(const std::vector<bool>& truth, const std::vector<double>& evristic, Result& r)
{
    std::vector<std::pair<double, int>> res(10);
    for (int i = 0; i < 10; ++i)
    {
        res[i] = std::pair<double, int> (evristic[i], i);
    }
    std::sort(res.begin(), res.end(), [](std::pair<double, int> a, std::pair<double, int> b)->bool
                                                    {
                                                    if (std::abs(a.first -  b.first) < 1e-10)
                                                        return a.second > b.second;
                                                      if (a.first > b.first)
                                                          return true;

                                                      return false;
                                                    });
    if (truth[res[0].second])
    {
        ++r.right_answers;
        ++r.right_answers2;
        ++r.right_answers3;
    }
    else if (truth[res[1].second])
    {
        ++r.right_answers2;
        ++r.right_answers3;
    }
    else if (truth[res[2].second])
    {
        ++r.right_answers3;
    }
    ++r.wrong_answers;
}

void R::Test(const std::string& data_file)
{
    std::ifstream in(data_file);
    Result res;
    while (!in.eof())
    {
        std::vector<double> feature(51, 0);
        for (int i = 0; i < feature.size(); ++i)
        {
            in >> feature[i];
        }
        double d;
        in >> d;
        std::vector<bool> truth;
        std::vector<std::vector<double>> features_by_rank =
                Get_features_by_rank(feature, truth);
        std::vector<double> scores = Get_scores(features_by_rank);
        GetResult(truth, scores, res);


    }
    in.close();
    std::cout << res.right_answers << " " << res.right_answers2 << " "
              << res.right_answers3
              << " " << res.wrong_answers << std::endl;


}
