#ifndef UTILS
#define UTILS

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <memory>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::shared_ptr;
using std::unique_ptr;

template<class Key, class Value>
Value& take(unordered_map<Key, Value>& data, const Key& key, Value def = Value())
{
    if (data.find(key) == data.end())
    {
        data[key] = def;
    }
    return data[key];
}

#endif
