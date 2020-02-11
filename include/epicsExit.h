/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/** 
 * @file epicsExit.h
 *
 * @brief This is an extended replacement for the Posix exit and atexit
 * routines, which also provides a pointer argument to pass to the exit
 * handlers.
 * 
 * This facility was created because of problems on vxWorks and windows
 * with the implementation of atexit, i.e. neither of these systems
 * implement exit and atexit according to the POSIX standard.
 **/

#ifndef epicsExith
#define epicsExith
#include <shareLib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*epicsExitFunc)(void *arg);

/** This calls epicsExitCallAtExits() and then passes status on to exit.
 *  @param status An int passed on to exit. 
 **/
epicsShareFunc void epicsExit(int status);

epicsShareFunc void epicsExitLater(int status);

/** This calls each of the functions registered by prior calls to
 *  epicsAtExit(), in reverse order of their registration. Most
 *  applications will not call this routine directly. 
 **/
epicsShareFunc void epicsExitCallAtExits(void);

/** Register a function and an associated context parameter, to be
 *  called with the given parameter when epicsExitCallAtExits() is
 *  invoked. 
 *  @param func Function to be called back.
 *  @param arg Parameter context.
 *  @param name Parameter context.
 **/
epicsShareFunc int epicsAtExit3(epicsExitFunc func, void *arg, const char* name);

/** Shortcut to epicsAtExit3(). 
 **/
#define epicsAtExit(F,A) epicsAtExit3(F,A,#F)

/** This calls each of the functions that were registered by the current
 *  thread calling epicsAtThreadExit(), in reverse order of the function
 *  registration. This routine is called automatically when an
 *  epicsThread’s main entry method returns, but will not be run if the
 *  thread is stopped by other means. 
 **/
epicsShareFunc void epicsExitCallAtThreadExits(void);

/** Register a function and an associated context parameter. The
 *  function will be called with the given parameter when
 *  epicsExitCallAtThreadExits() is invoked by the current thread ending
 *  normally, i.e. when the thread function returns. 
 *  @param func Function to be called back.
 *  @param arg Parameter context.
 **/
epicsShareFunc int epicsAtThreadExit(epicsExitFunc func, void *arg);


#ifdef __cplusplus
}
#endif

#endif /*epicsExith*/
