/*************************************************************************\
* Copyright (c) 2011 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file cantProceed.h
 * @brief declares routines that are provided for code that can’t proceed when an error occurs. 
 *
 *cantProceed.h declares routines that are provided for code that can’t proceed when an error occurs.
@code
void cantProceed(const char ⋆errorMessage, ...); 
void ⋆callocMustSucceed(size_t count, size_t size,const char ⋆errorMessage); 
void ⋆mallocMustSucceed(size_t size, const char ⋆errorMessage);
@endcode

 * cantProceed() accepts a printf format string and variable number of 
 * arguments; it displays the error message and suspends the current task. 
 * It will never return. callocMustSucceed() and mallocMustSucceed() can be 
 * used in place of calloc() and malloc(). If size or count are zero, or the 
 * memory allocation fails, they output a message and call cantProceed(). 
 */


#ifndef INCcantProceedh
#define INCcantProceedh

#include <stdlib.h>

#include "compilerDependencies.h"
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

epicsShareFunc void cantProceed(const char *errorMessage, ...) EPICS_PRINTF_STYLE(1,2);
epicsShareFunc void * callocMustSucceed(size_t count, size_t size, const char *errorMessage);
epicsShareFunc void * mallocMustSucceed(size_t size, const char *errorMessage);

#ifdef __cplusplus
}
#endif

#endif /* cantProceedh */
