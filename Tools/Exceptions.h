#ifndef ___TOOLS_EXCEPTIONS
#define ___TOOLS_EXCEPTIONS

#include <stdexcept>

namespace Tools
{

#ifndef MSVC
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

class LanguageNotFoundException : public std::runtime_error
{
public:
    LanguageNotFoundException();
};

class ProcessClientConnectionException : public std::runtime_error
{
public:
    ProcessClientConnectionException();
};

class ProcessClientCreationException : public std::runtime_error
{
public:
    ProcessClientCreationException();
};

class ProcessServerConnectionException : public std::runtime_error
{
public:
    ProcessServerConnectionException();
};

class ProcessServerCreationException : public std::runtime_error
{
public:
    ProcessServerCreationException();
};


}

#endif // ___TOOLS_EXCEPTIONS
