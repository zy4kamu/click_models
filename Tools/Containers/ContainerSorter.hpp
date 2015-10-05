#ifndef SORTING
#define SORTING

#include <algorithm>
#include <cassert>
#include <functional>

namespace Tools
{

template<class Container>
void Sort(Container& data, bool useLessFunctor = true)
{
    if (useLessFunctor) {
        std::sort(data.begin(), data.end(), std::less<typename Container::value_type>());
    } else {
        std::sort(data.begin(), data.end(), std::greater<typename Container::value_type>());
    }
}

template <class Container1, class Container2>
void Sort(Container1& data1, Container2& data2, bool useLessFunctor = true)
{
    typedef typename Container1::value_type T1;
    typedef typename Container2::value_type T2;
    assert(data1.size() == data2.size());
    size_t size = data1.size();
    vector<std::pair<T1,T2> > data;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i)
    {
        data.push_back(std::make_pair(data1[i], data2[i]));
    }
    Sort(data, useLessFunctor);
    for (size_t i = 0; i < size; ++i)
    {
        data1[i] = data[i].first;
        data2[i] = data[i].second;
    }
}

template<class T>
struct less_second
: std::binary_function<T,T,bool>
{
   inline bool operator()(const T& lhs, const T& rhs)
   {
      return lhs.second > rhs.second;
   }
};

template<class T>
struct great_first
: std::binary_function<T,T,bool>
{
   inline bool operator()(const T& lhs, const T& rhs)
   {
      return lhs.first < rhs.first;
   }
};

}

#endif // SORTING
