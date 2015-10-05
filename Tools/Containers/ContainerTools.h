#ifndef CONTAINER_TOOLS
#define CONTAINER_TOOLS

#include <algorithm>
#include <set>
#include <unordered_set>
#include <cassert>
#include <functional>

#include "ContainerInserter.h"
#include "TypeTraits.h"

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

using std::set;
using std::unordered_set;
using std::pair;

namespace Tools
{

/*************** GET VALUE OR DEFAULT *********************/

template <class Container>
typename Container::mapped_type GetValueOrDefault(const Container& container
    , const typename Container::key_type& key
    , const typename Container::mapped_type& defaultValue)
{
    auto found = container.find(key);
    if (found == container.end()) {
        return defaultValue;
    } else {
        return found->second;
    }
}

template<class Container>
typename Container::key_type
searchByValue(const Container& i_map, const typename Container::value_type& value)
{
    for (auto it = i_map.begin(); it != i_map.end(); ++it )
        if (it->second == value)
            return it->first;
    return Container::key_type();
}


/*********************** FIND MAX INDEX **************************/

template <class Container>
size_t FindMaxIndex(const Container& container)
{
    return std::distance(
        std::begin(container), std::max_element(
            std::begin(container), std::end(container)
            )
        );
}


}

#endif // CONTAINER_TOOLS
