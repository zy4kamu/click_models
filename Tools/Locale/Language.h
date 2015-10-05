#ifndef LANGUAGE_MACROS
#define LANGUAGE_MACROS

#include <string>

namespace Tools
{

enum Language
{
    RU,
    EN,
    EN_FAST
};

std::string LanguageToString(Language language);
std::string LanguageToLocaleString(Language language);

}

#endif // LANGUAGE_MACROS
