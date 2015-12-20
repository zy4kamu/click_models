#ifndef BASELINE_H
#define BASELINE_H

#include<vector>
#include<unordered_map>
#include<string>

#include "../day_data.h"

struct Url_type
{
    int missed;
    int skipped;
    int clicked;
    int clicked_1;
    int clicked_2;
};

struct Features
{
    int position;
    int query_id;
    std::vector<int> terms;
    int url_id;
    int domain;
    int url_query;
    int url_positoin;
    int domain_positoin;
    std::vector<std::string> url_terms;
    std::vector<std::string> domain_term;
};

struct UserData
{
    std::unordered_map<int , std::unordered_map<int, Url_type>> data;
    Url_type Get(int person, int url);
};

class BaseLine
{
private:
   UserData users;
public:
    BaseLine();
    void Write_Example(int enumerator, int rank, const std::unordered_map<int, Url_type>& local,
                       const Query& serp, ofstream* file);
};

#endif // BASELINE_H
