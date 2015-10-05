#ifndef LOCALE_SETTINGS
#define LOCALE_SETTINGS

#include <fstream>
#include <locale>
#include <string>
#ifdef MSVC
#include <codecvt>
#endif // MSVC


#include "Language.h"

namespace Tools
{

/************ STREAM CHAR IDENTIFIER **************/

template<class Stream>
struct StreamCharIdentifier;

template<>
struct StreamCharIdentifier<std::ifstream> { typedef char Char; };

template<>
struct StreamCharIdentifier<std::fstream> { typedef char Char; };

template<>
struct StreamCharIdentifier<std::ofstream> { typedef char Char; };

template<>
struct StreamCharIdentifier<std::wifstream> { typedef wchar_t Char; };

template<>
struct StreamCharIdentifier<std::wofstream> { typedef wchar_t Char; };

template<>
struct StreamCharIdentifier<std::wfstream> { typedef wchar_t Char; };

/*********** SET STREAM LOCALE FUNCTION ******************/

/**
* @brief	Makes thousands separator equal 0
*/
template<class Char>
struct no_separator : std::numpunct<Char>
{
protected:
    virtual std::string do_grouping() const
    {
        return "\000";
    }
};

template<class Stream>
void SetLocale(Stream& stream, Tools::Language language = Tools::Language::RU)
{
    typedef typename StreamCharIdentifier<Stream>::Char Char;
#ifdef MSVC
    stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<Char>));
#else
    const std::string& localeString = Tools::LanguageToLocaleString(language);
    stream.imbue(
        std::locale(
            std::locale(localeString.c_str()),
            new no_separator<Char>));
#endif
}

/*********** SET CONSOLE STREAM FUNCTION ******************/

void PrepareConsole(Tools::Language language = Tools::Language::RU);

}

#endif // LOCALE_SETTINGS
