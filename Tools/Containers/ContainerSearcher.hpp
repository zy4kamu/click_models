#ifndef CONTAINER_SEARCHER
#define CONTAINER_SEARCHER

namespace Tools
{

/* DEFINITIONS */

template <class Container>
bool Contains(const Container& container, const typename Container::value_type& elem);

template <class Container>
bool ContainsKey(const Container& container, const typename Container::key_type& key);

template <class Container>
bool ContainsValue(const Container& container, const typename Container::mapped_type& val);

/* REALIZATION */

template <class Container>
bool Contains(const Container& container, const typename Container::value_type& elem)
{
    return std::find(container.begin(), container.end(), elem) != container.end();
}

template <class Container>
bool ContainsKey(const Container& container, const typename Container::key_type& key)
{
    return container.find(key) != container.end();
}

template <class Container>
bool ContainsValue(const Container& container, const typename Container::mapped_type& val)
{
    return std::find_if(
        container.begin(), container.end()
        , [&](const typename Container::value_type& item) { return item.second == val; }) != container.end();
}

}


#endif // CONTAINER_SEARCHER
