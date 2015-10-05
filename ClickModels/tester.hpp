#ifndef TESTER
#define TESTER
#include <string>
#include <vector>
#include <iostream>
#include "day_data.h"

using std::string;
using std::vector;

template<class Strategy>
class Tester
{
public:
    Tester(const string& file):
        initial_statistics(0), got_statistics(0)
    {
        std::cout << "Tester: reading data ..." << std::endl;
        data = read_day(file);
    }

    void calculate(Strategy& strategy)
    {
        size_t enumerator = 0;
        const vector<int> defaultRanging = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        for(const auto& item0 : data)
        {
            for (const auto& item1 : item0.second)
            {
                const Query& query = item1.second;
                vector<int> ranging = strategy.calculate(query);
                if (ranging.size() > 0)
                {
                    update_statistics(&initial_statistics, query, defaultRanging);
                    update_statistics(&got_statistics, query, ranging);
                }
                if (enumerator++ % 100000 == 0)
                {
                    std::cout << "Tester: " << enumerator << " ..." << std::endl;
                }
            }
        }
    }

    void print()
    {
        std::cout << "Total number of queries: " << data.size() << std::endl;
        std::cout << "Basic ranker: " << initial_statistics << std::endl;
        std::cout << "Result ranker: " << got_statistics << std::endl;
        std::cout << "Fraction: " << 100 * (got_statistics / initial_statistics - 1) << "%s" << std::endl;
    }
private:
    void update_statistics(double* cumulative_stat, const Query& query,
        const vector<int> ranging)
    {
        if (ranging.size() > 0 && query.type[ranging[0]] == 2)
        {
            *cumulative_stat += 1;
        }
    }

    double initial_statistics;
    double got_statistics;
    DayData data;
};

#endif
