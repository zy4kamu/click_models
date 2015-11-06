#ifndef COUNTERS
#define COUNTERS
#include "uumap.h"
#include "day_data.h"

struct Counters
{
    uumap query_url;
    uumap user_url;
    uumap user_rank;
    uumap query_user;
    uumap query_rank;

    Counters();
    void clear();
    void save(size_t start_day, size_t finish_day);
    void Set_query_url(uumap& query_url_);
    void Set_user_url(uumap& user_url_);
    void Set_user_rank(uumap& user_rank_);
    void Set_query_user(uumap& query_user_);
    void Set_query_rank(uumap& query_rank_);
};

extern Counters counters;

void calculate_counters(const DayData& data, Counters& counters1);
void calculate_counters(const string& folder, size_t start_day, size_t finish_day);

#endif
