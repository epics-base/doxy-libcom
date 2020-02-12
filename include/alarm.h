/*************************************************************************\
* Copyright (c) 2009 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file alarm.h
 * @brief Setup alarm definitions
 * @author Bob Dalesio and Marty Kraimer
 *
 * Alarm definitions 
 * @note Must match menuAlarmSevr.dbd and menuAlarmStat.dbd
 */

#ifndef INC_alarm_H
#define INC_alarm_H

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @def NO_ALARM
 * @brief defines NO_ALARM as 0
 */
#define NO_ALARM            0

//! @enum epicsAlarmSeverity assigns alarm severities
//!  @note must match menuAlarmSevr.dbd
typedef enum {
    epicsSevNone = NO_ALARM, //!< no alarm
    epicsSevMinor,           //!< minor severity
    epicsSevMajor,           //!< major severity
    epicsSevInvalid,         //!< invalid severity
    ALARM_NSEV               //!< Nr. of alarm severities
} epicsAlarmSeverity;

/**
 * @defgroup alarmDefs alarm 
 * @{
 */
//! @def alarm definitions
#define firstEpicsAlarmSev  epicsSevNone
#define MINOR_ALARM         epicsSevMinor
#define MAJOR_ALARM         epicsSevMajor
#define INVALID_ALARM       epicsSevInvalid
#define lastEpicsAlarmSev   epicsSevInvalid

//! @enum epicsAlarmCondition assigns alarm conditions
//!  @note must match menuAlarmStat.dbd
typedef enum {
    epicsAlarmNone = NO_ALARM, //!< no alarm
    epicsAlarmRead,            //!< read alarm (hw read?)
    epicsAlarmWrite,           //!< write alarm (hw write?)
    epicsAlarmHiHi,            //!< High high limit alarm
    epicsAlarmHigh,            //!< High limit alarm
    epicsAlarmLoLo,            //!< Low low limit alarm
    epicsAlarmLow,             //!< Low limit alarm
    epicsAlarmState,           //!< State alarm (e.g. off/on)
    epicsAlarmCos,             //!< Change of state alarm
    epicsAlarmComm,            //!< Communication alarm
    epicsAlarmTimeout,         //!< Timeout alarm
    epicsAlarmHwLimit,         //!< Hardware limit alarm
    epicsAlarmCalc,            //!< Alarms from calc/calcout record
    epicsAlarmScan,            //!< Scan alarm, e.g. record not processed (10 times) or not in desired scan list
    epicsAlarmLink,            //!< Link alarm
    epicsAlarmSoft,            //!< Soft alarm, e.g. in sub record if subroutine gives error
    epicsAlarmBadSub,          //!< Bad subroutine alarm, e.g. in sub record subroutine not defined
    epicsAlarmUDF,             //!< Undefined alarm state, e.g. record never processed
    epicsAlarmDisable,         //!< Alarm disabled
    epicsAlarmSimm,            //!< Alarm simulation
    epicsAlarmReadAccess,      //!< Alarm on read access (?AlarmRead)
    epicsAlarmWriteAccess,     //!< Alarm on write access (?AlarmWrite)
    ALARM_NSTATUS              //!< Nr. of alarm conditions
} epicsAlarmCondition;

#define firstEpicsAlarmCond epicsAlarmNone
#define READ_ALARM          epicsAlarmRead
#define WRITE_ALARM         epicsAlarmWrite
#define HIHI_ALARM          epicsAlarmHiHi
#define HIGH_ALARM          epicsAlarmHigh
#define LOLO_ALARM          epicsAlarmLoLo
#define LOW_ALARM           epicsAlarmLow
#define STATE_ALARM         epicsAlarmState
#define COS_ALARM           epicsAlarmCos
#define COMM_ALARM          epicsAlarmComm
#define TIMEOUT_ALARM       epicsAlarmTimeout
#define HW_LIMIT_ALARM      epicsAlarmHwLimit
#define CALC_ALARM          epicsAlarmCalc
#define SCAN_ALARM          epicsAlarmScan
#define LINK_ALARM          epicsAlarmLink
#define SOFT_ALARM          epicsAlarmSoft
#define BAD_SUB_ALARM       epicsAlarmBadSub
#define UDF_ALARM           epicsAlarmUDF
#define DISABLE_ALARM       epicsAlarmDisable
#define SIMM_ALARM          epicsAlarmSimm
#define READ_ACCESS_ALARM   epicsAlarmReadAccess
#define WRITE_ACCESS_ALARM  epicsAlarmWriteAccess
#define lastEpicsAlarmCond  epicsAlarmWriteAccess
/** @} */

/**
 * @var Name string arrays
 */
epicsShareExtern const char *epicsAlarmSeverityStrings [ALARM_NSEV];
epicsShareExtern const char *epicsAlarmConditionStrings [ALARM_NSTATUS];


#ifdef __cplusplus
}
#endif

#endif /* INC_alarm_H */
