#ifndef FILTERS_H
#define FILTERS_H
#include "../uumap.h"
#include "../counters.h"

struct Result
{
    size_t corect_pairs;
    size_t right_answers;
    size_t right_answers2;
    size_t right_answers3;
    size_t wrong_answers;
    Result(): corect_pairs(0), right_answers(0),
                               right_answers2(0),
                               right_answers3(0),
        wrong_answers(0) {}
};

bool Filter1(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
bool Filter2(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
bool Filter3(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
bool Filter4(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
bool Filter5(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
bool Filter3Mine(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history, int min_number, int max_number);
bool GetFilter(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
bool GetFilterForTest(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history);
#endif // FILTERS_H
