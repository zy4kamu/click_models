/** 
 * @file	LinearHashFunction.h
 * @brief	Calculates ax + b function
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */
#ifndef LINEAR_HASH_FUNCTION
#define LINEAR_HASH_FUNCTION

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

const long long PRIME = 2000000011;
const long long UNDEFINED_NUMBER = PRIME;
const long long RANDMAX = 9000000000;
const int DEFAULT_HASH_SLOPE = 1;
const int DEFAULT_HASH_COERCIDENT = 0;

/** 
 * @class	LinearHashFunction
 * @brief	Calculates ax + b function
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */
struct LinearHashFunction
{
    int slope; /**< linear coefficient*/
    int intercept; /**< free factor*/

    /**
	 * @brief	Constructor
	 * @param[in]	possibleStateFinder - tool for finding it
	 */
    LinearHashFunction();

    /**
	 * @brief	Constructor
	 * @param[in]	slope - linear coefficient
     * @param[in]	intercept - free factor
	 */
    LinearHashFunction(int slope, int intercept);

    /**
	 * @brief	Calculates value
	 * @param[in]	input - value for which calculate hash function
	 */
    int operator ()(int input) const;
};

/**
* @brief	Creates random hash function
* @param[in]	generator - random number generator
*/
LinearHashFunction RandomizeHashFunction(
    std::default_random_engine& generator);

#endif
