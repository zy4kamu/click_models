#ifndef COLLABORATIVE_FILTERING_H
#define COLLABORATIVE_FILTERING_H

#include<vector>
#include<map>
#include<numeric>
#include<random>
#include<math.h>
#include "FileWriter.h"
#include "FileReader.h"
#include "../uumap.h"
#include "../day_data.h"




class Similarity_function
{
public:
    std::function<double(const std::vector<double>& ,const std::vector<double>&)> similarity;
    std::function<std::vector<double>
    (const std::vector<double>&, const std::vector<double>&)> divSimilarity;
    Similarity_function(std::function<double(const std::vector<double>& ,const std::vector<double>&)> s,
                        std::function<std::vector<double>
                        (const std::vector<double>&, const std::vector<double>&)> div) : similarity(s), divSimilarity(div) {}
};

class Example
{
public:
    size_t user;
    size_t rang_click_document;
    Example(size_t u, size_t r): user(u), rang_click_document(r) {}
};

//probability to buy something sum(sim(u,u_i)*f) 
class collaborative_filtering
{
private:
    double rate;
    int dim;
    std::unordered_map<size_t, std::vector<double>> embedding;
    Similarity_function f;
private:
    std::vector<double> res_one_position(const std::vector<Example>& examples, size_t user) const;

public:
    collaborative_filtering(double rate_, int dim_, const string& usersFile);
    void Learn(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, DayData& dayData);
    void One_step(const std::vector<Example>& examples,
                                           const std::vector<bool>& truth,
                                           size_t user);
    void Print(const string& file) const;
};

#endif // COLLABORATIVE_FILTERING_H
