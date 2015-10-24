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
};

extern Counters counters;

void calculate_counters(const DayData& data, Counters& counters1);
void calculate_counters(const string& folder, size_t start_day, size_t finish_day);

#endif
