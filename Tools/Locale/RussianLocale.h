#ifndef RUSSIAN_LOCALE
#define RUSSIAN_LOCALE

#include <string>

namespace Tools
{

/**
 * @class	RussianLocale
 * @brief	Class for supporting switching system locales.
 *
 * @remark	Work principle: sets choosen locale in constructor, and turns it back in destructor
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */
class RussianLocale {
public:
    RussianLocale();
    ~RussianLocale();

private:
    std::string externalLocale;
};

}

#endif // RUSSIAN_LOCALE
