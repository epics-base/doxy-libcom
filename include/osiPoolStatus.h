/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/**@Author Jeff Hill
 * 
 * @brief Functions which interrogate the state of the system wide pool
 *
 **/

#ifndef INC_osiPoolStatus_H
#define INC_osiPoolStatus_H

#include <stdlib.h>
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Tests to see if there is sufficent space for a block of the requested size
 * along with whatever additional free space is necessary to keep the system running 
 * reliably
 * 
 * @note this routine is called quite frequently so an efficent implementation is important
 *
 * @A vxWorks version returns (True, False) if memFindMax returns (>100000, <=10000) bytes.
 *
 * @param contiguousBlockSize Block size to check.
 * @return (True, False) if there is sufficient memory.
 */
epicsShareFunc int epicsShareAPI osiSufficentSpaceInPool ( size_t contiguousBlockSize );

#ifdef __cplusplus
}
#endif

#include "osdPoolStatus.h"

#endif /* INC_osiPoolStatus_H */
