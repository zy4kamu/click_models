#include "filters.h"

bool  Filter1(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
{
    size_t query = history.id;
    size_t user = history.person;
    int url = -1;

    const vector<double>& rank0 = queryRank.watch(query, 0);
    const vector<double>& rank1 = queryRank.watch(query, 1);
    if (rank0.size() > 0 && (rank1.size() == 0 || rank0[0] >= 2 * rank1[0]))
    {
     return false;
    }
    return true;
}

bool  Filter2(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
{
    size_t query = history.id;
    size_t user = history.person;
    int url = -1;

    for (size_t i = 0; i < 10; ++i)
    {
     const vector<double>& found = userUrl.watch(user, history.urls[i]);
     if (found.size() > 0 && found[0] > 1 - 1e-5)
     {
         return false;
     }
    }
    return true;
}

bool Filter3(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
{
    size_t query = history.id;
    if (queryUser.watch(query).size() <  2) return false;
    return true;
}

bool Filter3Mine(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history, int min_number, int max_number)
{
    size_t query = history.id;
    if (queryUser.watch(query).size() <  min_number) return false;
    if (queryUser.watch(query).size() >  max_number) return false;
    return true;
}

bool Filter4(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
{
    bool found = false;
    for (size_t i = 0; i < 10; ++i)
    {
     if(history.type[i] == 2)
     {
         found = true;
         break;
     }
    }
    if (!found) return false;
    return true;
}

bool Filter5(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
{
    bool found = false;
    for (size_t i = 0; i < 2; ++i)
    {
     if(history.type[i] == 2)
     {
         found = true;
         break;
     }
    }
    if (found) return false;
    return true;
}

bool GetFilter(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history)
{
    typedef bool(*filter) (const uumap&, const uumap&, const uumap&, const Query&);
    vector<filter> filters = {Filter1, Filter2, Filter3, Filter4, Filter5};
    for (size_t i = 0; i < filters.size(); ++i)
    {
        if (!filters[i](queryUser, userUrl, queryRank, history)) return false;
    }
    return true;
}

bool GetFilterForTest(const uumap& queryUser, const uumap& userUrl, const uumap& queryRank, const Query& history, int min_number, int max_number)
{
    typedef bool(*filter) (const uumap&, const uumap&, const uumap&, const Query&);
    vector<filter> filters = {Filter1, Filter2, Filter4};
    for (size_t i = 0; i < filters.size(); ++i)
    {
        if (!filters[i](queryUser, userUrl, queryRank, history)) return false;
    }
    if (!Filter3Mine(queryUser, userUrl, queryRank, history, min_number, max_number)) return false;
    return true;
}
