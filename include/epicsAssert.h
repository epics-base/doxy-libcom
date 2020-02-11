/*************************************************************************\
* Copyright (c) 2009 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**@file epicsAssert.h
 * @author Jeffery O. Hill
 * @date 27 Feb 1995
 *
 * @brief This is a replacement for ANSI C's assert.
 *
 * To use this version just include:
 @code
 	#include "epicsAssert.h"
 @endcode
 * instead of
 @code
 	#include <assert.h>
 @endcode
 *
 * If an assert fails, it calls errlog indicating the program’s author, file name, and
 * line number. Under each OS there are specialized instructions assisting the user to
 * diagnose the problem and generate a good bug report. For instance, under vxWorks,
 * there are instructions on how to generate a stack trace, and on posix there are
 * instructions about saving the core file. After printing the message the calling thread
 * is suspended. An author may, before the above include line, optionally define a
 * preprocessor macro named epicsAssertAuthor as a string that provides their name and
 * email address if they wish to be contacted when the assertion fires.
 */

#ifndef INC_epicsAssert_H
#define INC_epicsAssert_H

#include "shareLib.h"
#include "compilerDependencies.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef epicsAssertAuthor
#   define epicsAssertAuthor 0
#endif

#undef assert

#ifdef NDEBUG
#   define assert(ignore) ((void) 0)
#else /* NDEBUG */

epicsShareFunc void epicsAssert (const char *pFile, const unsigned line,
    const char *pExp, const char *pAuthorName);

#   define assert(exp) ((exp) ? (void)0 : \
        epicsAssert(__FILE__, __LINE__, #exp, epicsAssertAuthor))

#endif  /* NDEBUG */


/* Compile-time checks */
#if __cplusplus>=201103L
#define STATIC_ASSERT(expr) static_assert(expr, #expr)
#else
#define STATIC_JOIN(x, y) STATIC_JOIN2(x, y)
#define STATIC_JOIN2(x, y) x ## y
#define STATIC_ASSERT(expr) \
    typedef int STATIC_JOIN(static_assert_failed_at_line_, __LINE__) \
    [ (expr) ? 1 : -1 ] EPICS_UNUSED
#endif

#ifdef __cplusplus
}
#endif

#endif /* INC_epicsAssert_H */
