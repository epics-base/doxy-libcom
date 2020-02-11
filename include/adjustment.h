/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/** \file adjustment.h
 * Header file to describe adjustToWorstCaseAlignment function
 *
 */

#ifndef INCadjustmenth
#define INCadjustmenth
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/** returns a value larger or equal than `size`, that is an exact
      multiple of the worst case alignment for the architecture on
      which the routine is executed. */
epicsShareFunc size_t adjustToWorstCaseAlignment(size_t size);

#ifdef __cplusplus
}
#endif


#endif /*INCadjustmenth*/
