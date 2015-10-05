/**
* @file	time-calc.h
* @brief	header file for cross-platform handling gettimeofday function
*
* Copyright 2013 by Samsung Electronics, Inc.,
*
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung.
*/

#ifndef _TIME_CALC_
#define _TIME_CALC_

#ifdef MSVC
#include <time.h>
#include <windows.h> 

#define DELTA_EPOCH_IN_MICROSECS  116444736000000000Ui64

/* FILETIME of Jan 1 1970 00:00:00, the PostgreSQL epoch */
static const unsigned __int64 epoch = 116444736000000000Ui64;

/*
* FILETIME represents the number of 100-nanosecond intervals since
* January 1, 1601 (UTC).
*/
#define FILETIME_UNITS_PER_SEC	10000000L
#define FILETIME_UNITS_PER_USEC	10

int gettimeofday(struct timeval * tp, struct timezone * tzp);

#else

#include <sys/time.h>

#endif // MSVC


#endif //_TIME_