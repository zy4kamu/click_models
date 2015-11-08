#ifndef UUMAP
#define UUMAP
#include "utils.h"

class uumap
{
public:
    uumap();
    uumap(const string& file);
    void save(const string& file);
    vector<double>& get(size_t key1, size_t key2, size_t default_size);
    const vector<double>& watch(size_t key1, size_t key2) const;
    const unordered_map<size_t, vector<double> >& watch(size_t key1) const;
    void clear();
public:
    typedef unordered_map<size_t, unordered_map<size_t, vector<double> > > UUmap;
    UUmap data;
};


#endif
