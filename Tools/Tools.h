/**
 * @file	Tools.h
 * @brief	header file containing common-used functions (splitting, trimming, sorting etc.)
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef TOOLS
#define TOOLS

#include "CrossPlatformHeader.h"
#include "DataFunctors.h"

#include "Containers/Alphabet.h"
#include "Containers/ContainerConverters.hpp"
#include "Containers/ContainerEraser.h"
#include "Containers/ContainerGenerators.hpp"
#include "Containers/ContainerInserter.h"
#include "Containers/ContainerSearcher.hpp"
#include "Containers/ContainerSorter.hpp"
#include "Containers/ContainerTools.h"
#include "Containers/Document.h"
#include "Containers/SparseVector.h"
#include "Containers/TypeTraits.h"

#include "Exceptions.h"

#include "IO/BinaryIO.h"
#include "IO/ConsoleTools.h"

#include "Locale/Language.h"
#include "Locale/LocaleSettings.hpp"

#include "Math/TableCalculator.hpp"

#include "OS/FixedSizeConnecter.hpp"
#include "OS/FloatSizeConnecter.hpp"
#include "OS/OS.h"
#include "OS/ProcessClient.hpp"
#include "OS/ProcessServer.hpp"
#include "OS/time-calc.h"

#include "Locale/RussianLocale.h"

#include "Strings/AhoCorasickParser.hpp"
#include "Strings/StringConvertions.h"
#include "Strings/StringTools.h"

#include "Strings/Splitter.h"

#endif // TOOLS
