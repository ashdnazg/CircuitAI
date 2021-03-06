/*
 * BlockingMap.cpp
 *
 *  Created on: Aug 1, 2016
 *      Author: rlcevg
 */

#include "terrain/BlockingMap.h"

namespace circuit {

SBlockingMap::StructTypes SBlockingMap::structTypes = {
	{"factory",   SBlockingMap::StructType::FACTORY},
	{"mex",       SBlockingMap::StructType::MEX},
	{"engy_low",  SBlockingMap::StructType::ENGY_LOW},
	{"engy_mid",  SBlockingMap::StructType::ENGY_MID},
	{"engy_high", SBlockingMap::StructType::ENGY_HIGH},
	{"pylon",     SBlockingMap::StructType::PYLON},
	{"def_low",   SBlockingMap::StructType::DEF_LOW},
	{"def_mid",   SBlockingMap::StructType::DEF_MID},
	{"def_high",  SBlockingMap::StructType::DEF_HIGH},
	{"special",   SBlockingMap::StructType::SPECIAL},
	{"nano",      SBlockingMap::StructType::NANO},
	{"unknown",   SBlockingMap::StructType::UNKNOWN},
};

SBlockingMap::StructMasks SBlockingMap::structMasks = {
	{"factory",   SBlockingMap::StructMask::FACTORY},
	{"mex",       SBlockingMap::StructMask::MEX},
	{"engy_low",  SBlockingMap::StructMask::ENGY_LOW},
	{"engy_mid",  SBlockingMap::StructMask::ENGY_MID},
	{"engy_high", SBlockingMap::StructMask::ENGY_HIGH},
	{"pylon",     SBlockingMap::StructMask::PYLON},
	{"def_low",   SBlockingMap::StructMask::DEF_LOW},
	{"def_mid",   SBlockingMap::StructMask::DEF_MID},
	{"def_high",  SBlockingMap::StructMask::DEF_HIGH},
	{"special",   SBlockingMap::StructMask::SPECIAL},
	{"nano",      SBlockingMap::StructMask::NANO},
	{"unknown",   SBlockingMap::StructMask::UNKNOWN},
	{"none",      SBlockingMap::StructMask::NONE},
	{"all",       SBlockingMap::StructMask::ALL},
};

} // namespace circuit
