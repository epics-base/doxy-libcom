/*************************************************************************\
* Copyright (c) 2009 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file alarmString.h
 * @brief Define String named for alarm status
 * @author Unknown
 * @date xxx
 *
 * String names for alarm status and severity values
 *
 * @note This file is deprecated, use alarm.h instead.
 */

#ifndef INC_alarmString_H
#define INC_alarmString_H

#include "alarm.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def alarmSeverityString
 * @brief Defines alias for epicsAlarmSeverityStrings
 */
#define alarmSeverityString epicsAlarmSeverityStrings
/**
 * @def alarmStatusString
 * @brief Defines alias for epicsAlarmConditionStrings
 */
#define alarmStatusString epicsAlarmConditionStrings


#ifdef __cplusplus
}
#endif

#endif
