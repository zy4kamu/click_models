#ifndef CONTAINER_GENERATORS
#define CONTAINER_GENERATORS

namespace Tools
{

template<class T>
vector<T> Range(T start, T end, T step = 1)
{
    size_t size = (end - start) / step;
    vector<T> vec(size);
    T toInsert = start - step;
    std::generate(vec.begin(), vec.end(),
        [&]() { return toInsert += step; });
    return vec;
}

}

#endif // CONTAINER_GENERATORS
