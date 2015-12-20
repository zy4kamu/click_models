#ifndef DAY_DATA
#define DAY_DATA
#include "utils.h"
#include "uumap.h"

struct Query
{
    size_t id;
    size_t person;
    size_t session;
    vector<size_t> urls;
    vector<int> type;
    vector<int> domains;
    vector<int> terms;

    Query() : urls(10, -1), type(10, -1), domains(10, -1), terms(5, 0) {}
};

typedef unordered_map<size_t, unordered_map<size_t, Query> > DayData;

void separate_by_day(const string& file, const string& outFolder);
DayData read_day(const string& file);


#endif
