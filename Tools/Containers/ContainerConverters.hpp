#ifndef CONTAINER_CONVERTERS
#define CONTAINER_CONVERTERS

#include <algorithm>
#include <iterator>
#include <map>
#include <unordered_map>
#include <vector>

#include "TypeTraits.h"

namespace  Tools
{

using std::map;
using std::unordered_map;
using std::vector;

/***************** DEFINITION ******************/

// TO VECTOR
template<class Container>
vector<typename ConstRemover<typename Container::value_type>::Type > ToVector(
    const Container& container);

// TO MAP
template<class Container>
std::map<
    typename Container::value_type::first_type
    , typename Container::value_type::second_type> ToMap(
    const Container& container);

// TO UNORDERED MAP
template<class Container>
std::unordered_map<
    typename Container::value_type::first_type
    , typename Container::value_type::second_type
    > ToUMap(const Container& container);

// KEYS
template<class Container>
vector<typename std::remove_const<typename Container::key_type>::type > Keys(
    const Container& container);

// VALUES
template<class Container>
vector<typename Container::mapped_type> Values(
    const Container& container);


/***************** REALIZATION ******************/

template<class Container>
vector<typename ConstRemover<typename Container::value_type>::Type > ToVector(
    const Container& container)
{
    vector<typename ConstRemover<typename Container::value_type>::Type> converted;
    std::copy(container.begin(), container.end(), std::back_inserter(converted));
    return converted;
}

template<class Container>
std::map<
    typename Container::value_type::first_type
    , typename Container::value_type::second_type> ToMap(
    const Container& container)
{
    return std::map<
        typename Container::value_type::first_type
        , typename Container::value_type::second_type>(
        container.begin(), container.end());
}

template<class Container>
std::unordered_map<
    typename Container::value_type::first_type
    , typename Container::value_type::second_type
    > ToUMap(const Container& container)
{
    return std::unordered_map<
        typename Container::value_type::first_type
        , typename Container::value_type::second_type>(
        container.begin(), container.end());
}

template<class Container>
vector<typename std::remove_const<typename Container::key_type>::type > Keys(
    const Container& container)
{
    typedef typename std::remove_const<typename Container::key_type>::type KEY_TYPE;
    typedef typename Container::value_type PAIR_TYPE;
    vector<KEY_TYPE> keys;
    std::transform(
        container.begin(), container.end(), std::back_inserter(keys)
        , [](const PAIR_TYPE& p) { return p.first; });
    return keys;
}

template<class Container>
vector<typename Container::mapped_type> Values(
    const Container& container)
{
    typedef typename Container::mapped_type VALUE_TYPE;
    typedef typename Container::value_type PAIR_TYPE;
    vector<VALUE_TYPE> values;
    std::transform(
        container.begin(), container.end(), std::back_inserter(values)
        , [](const PAIR_TYPE& p) { return p.second; });
    return values;
}

}


#endif // CONTAINER_CONVERTERS
