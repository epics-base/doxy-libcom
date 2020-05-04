/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2008 Diamond Light Source Ltd
* Copyright (c) 2004 Oak Ridge National Laboratory
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/**@file epicsGeneralTime.h
 *
 * @brief The generalTime framework provides a mechanism for several time providers to be
 * present within the system.
 *
 * There are two types of provider, one type for the current time and one type for the
 * current time and one type for providing Time Event times. Each time provider has a
 * time priority, and installed providers are queried in priority order whenever a time
 * is requested, until one returns successfully. Thus there is a fallback from higher
 * priority providers (smaller value of priority) to lower priority providers (larger
 * value of priority) if te higher priority ones fail. Each architecture has a "last
 * resort" provider, installed at priority 999, usually based on the system clock, which
 * is used in the absence of any other provider.
 *
 * Targets running vxWorks and RTEMS have an NTP provider installed at priority 100.
 *
 * Registered providers may also add an interrupt-safe routine that will be called from
 * the epicsTimeGetCurrentInt() or epicsTimeGetEventInt() API routines. These interfaces
 * do not check the priority queue, and will only succeed if the last-used provider has
 * registered a suitable routine.
 *
 * @note There are two interfaces to this framework, epicsGeneralTime.h for consumers
 * that wish to get a time and query the framework, and generalTimeSup.h for providers
 * that supply timestamps.
 **/

#ifndef INC_epicsGeneralTime_H
#define INC_epicsGeneralTime_H

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@def NUM_TIME_EVENTS
 * @brief The number of timed events that are validated.
 *
 * Time Events numbered 0 through (NUM_TIME_EVENTS-1) are validated by code in
 * epicsGeneralTime.c that tests for advancing timestamps and enforces that
 * restriction.
 *
 * @note Event numbers greater than or equal to NUM_TIME_EVENTS are now allowed
 * if supported by a custom time provider which should provide its own advancing
 * timestamp validation.
 **/
#define NUM_TIME_EVENTS 256

/**@brief Initialise the framework. This is called automatically by any function
 * that requires the framework.
 *
 * @note It does not need to be called explicitly.
 **/
epicsShareFunc void generalTime_Init(void);

/**@brief Install a Time Event time provider that returns the current time for any
 * Time event number.
 *
 * @note This is optional as it is site policy whether the last resort for a Time
 * Event time in the absence of any working provider should be a failure, or the
 * current time.
 **/
epicsShareFunc int  installLastResortEventProvider(void);

/**@brief Reset the internal counter of the number of times the time returned was
 * earlier than when previously requested.
 *
 * Used by device support for binary out record
 * with:
 @code
     DTYP = "General Time"
     OUT = "@RSTERRCNT"
 @endcode
 **/
epicsShareFunc void generalTimeResetErrorCounts(void);

/**@brief Return the internal counter of the number of times the time returned was
 * earlier than when previously requested.
 *
 * Used by device support for longin record
 * with:
 @code
     DTYP = "General Time"
     INP = "@GETERRCNT"
 @endcode
 *
 * @return Reset error counts
 **/
epicsShareFunc int  generalTimeGetErrorCounts(void);

/**@brief Return the nume of the provider that last returned a valid current time, or
 * NULL if none.
 *
 * Used by stringin device support with:
 @code
 	DTYP = "General Time"
    INP = "@BESTTCP"
 @endcode
 *
 * @return Provider name
 **/
epicsShareFunc const char * generalTimeCurrentProviderName(void);

/**@brief Return the name of the provider that last returned a valid Time Event time, or
 * NULL of none.
 *
 * Used by stringin device support with:
 @code
     DTYP = "General Time"
     INP = "@BESTTEP"
 @endcode
 *
 * @return Provider name
 **/
epicsShareFunc const char * generalTimeEventProviderName(void);

/**@brief Return the name of the registered current time provider that has the highest
 * priority.
 *
 * Used by stringin device support with:
 @code
 	DTYP = "General Time"
    INP = "@TOPTCP"
 @endcode
 *
 * @return Provider name
 **/
epicsShareFunc const char * generalTimeHighestCurrentName(void);

/* Original names, for compatibility */
#define generalTimeCurrentTpName generalTimeCurrentProviderName
#define generalTimeEventTpName generalTimeEventProviderName

/**@brief Provide information about the installed providers and their current best times.
 *
 * @param interst Desired interest level to report
 **/
epicsShareFunc long generalTimeReport(int interest);

#ifdef __cplusplus
}
#endif

#endif /* INC_epicsGeneralTime_H */
