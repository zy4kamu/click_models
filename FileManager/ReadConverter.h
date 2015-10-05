#ifndef PRINT_CONVERTER
#define PRINT_CONVERTER

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

namespace FileManager
{

template <class String, class ValueType> struct ReadConverter
{
    static ValueType Convert(String&& data)
    {
        return static_cast<ValueType>(data);
    }
};

template <class String>
struct ReadConverter<String, int>
{
    static int Convert(String&& line)
    {
        return std::stoi(line);
    }
};

template <class String>
struct ReadConverter<String, size_t>
{
    static size_t Convert(String&& line)
    {
        return std::stoi(line);
    }
};

template <class String>
struct ReadConverter<String, double>
{
    static double Convert(String&& line)
    {
        return std::stod(line);
    }
};

};

#endif // PRINT_CONVERTER
