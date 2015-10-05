#ifndef NGRAMSTORAGE_H
#define NGRAMSTORAGE_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Tools.h"

namespace Tools
{

/**
 * @class	NgramStorage
 * @brief	Data structure for ngram storage
 *
 *
 * Copyright 2015 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

template<typename StringType, typename CountType>
class NgramStorage
{
public:
     NgramStorage();

private:
    std::unordered_map<StringType, size_t> m_vocabulary;

};

}

#endif // NGRAMSTORAGE_H
