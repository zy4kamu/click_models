#ifndef TRAINING_AND_TEST_DATA_FUNCTORS
#define TRAINING_AND_TEST_DATA_FUNCTORS

#include <cstdlib>

namespace Tools
{

class DataFunctor
{
public:
    virtual bool operator()(size_t index) = 0;
};

template<size_t modNumber>
class ModFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index % modNumber == 0;
    }
};

template<size_t modNumber, size_t firstMod>
class ModFunctorOneParam : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        size_t mod = index % modNumber;
        return (mod == firstMod);
    }
};

template<size_t modNumber>
class NotModFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index % modNumber != 0;
    }
};

template<size_t modNumber, size_t firstMod>
class NotModFunctorOneParam : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        size_t mod = index % modNumber;
        return (mod != firstMod);
    }
};

template<size_t modNumber, size_t firstMod, size_t secondMod>
class NotModFunctorTwoParams : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        size_t mod = index % modNumber;
        return (mod != firstMod) && (mod != secondMod);
    }
};

template<size_t maxIndex>
class LessFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index < maxIndex;
    }
};

template<size_t maxIndex>
class MoreFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index >= maxIndex;
    }
};

template<size_t lowerBound, size_t upperBound>
class SegmentFunctor : public DataFunctor
{
public:
    virtual bool operator()(size_t index)
    {
        return index >= lowerBound && index <= upperBound;
    }
};

}

#endif // TRAINING_AND_TEST_DATA_FUNCTORS
