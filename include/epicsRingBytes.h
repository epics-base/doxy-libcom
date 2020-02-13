/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* Copyright (c) 2012 ITER Organization.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/
/**
 * @file epicsRingBytes.h
 * @author Marty Kraimer, Eric Norum, Ralph Lange
 * @date 15. Jul 1999
 *
 * EpicsRingBytes provides methods for creating and using ring buﬀers
 * (ﬁrst in ﬁrst out circular buﬀers) that store bytes. The unlocked
 * variant is designed so that one writer thread and one reader thread
 * can access the ring simultaneously without requiring mutual exclusion.
 * The locked variant uses an epicsSpinLock, and works with any numbers of
 * writer and reader threads.
 * @note If there is only one writer it is not necessary to lock for puts
 * If there is a single reader it is not necessary to lock for pops 
 * epicsRingBytesLocked uses a spinlock.
 */

#ifndef INCepicsRingBytesh
#define INCepicsRingBytesh

#ifdef __cplusplus
extern "C" {
#endif

#include "shareLib.h"

typedef void *epicsRingBytesId;
typedef void const *epicsRingBytesIdConst;

/**
 * @brief Create a new ring buﬀer
 * @param nbytes Size of ring buffer to create
 * @return Ring buffer Id or NULL on failure
 */
epicsShareFunc epicsRingBytesId  epicsShareAPI epicsRingBytesCreate(int nbytes);
/**
 * @brief Create a new ring buﬀer, secured by a spinlock
 * @param nbytes Size of ring buffer to create
 * @return Ring buffer Id or NULL on failure
 */
epicsShareFunc epicsRingBytesId  epicsShareAPI epicsRingBytesLockedCreate(int nbytes);
/**
 * @brief Delete the ring buﬀer and free any associated memory
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 */
epicsShareFunc void epicsShareAPI epicsRingBytesDelete(epicsRingBytesId id);
/**
 * @brief Read data out of the ring buffer
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @param value Up to nbytes get moved from the ring buffer to value 
 * @param nbytes Get nbytes from ring buffer
 * @return The number of bytes actually moved is returned.
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesGet(
    epicsRingBytesId id, char *value,int nbytes);
/**
 * @brief Move data to the ring buffer
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @param value Move nbytes from value to the ring buﬀer if there is enough
 * free space available to hold them. 
 * @param nbytes Move nbytes to the ring buffer
 * @return The number of bytes actually moved is returned, which will
 * be zero if insuﬃcient space exists.
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesPut(
    epicsRingBytesId id, char *value,int nbytes);
/**
 * @brief Make the ring buﬀer empty
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @note Should only be used when both gets and puts are locked out.
 */
epicsShareFunc void epicsShareAPI epicsRingBytesFlush(epicsRingBytesId id);
/**
 * @brief Return the number of free bytes in the ring buﬀer
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @return The number of free bytes in the ring buﬀer
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesFreeBytes(epicsRingBytesId id);
/**
 * @brief Return the number of bytes currently stored in the ring buﬀer
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @return The number of bytes currently stored in the ring buﬀer
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesUsedBytes(epicsRingBytesId id);
/**
 * @brief Return the size of the ring buﬀer
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @return Return the size of the ring buﬀer, i.e., nbytes speciﬁed in
 * the call to epicsRingBytesCreate().
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesSize(epicsRingBytesId id);
/**
 * @brief Test if the ring buﬀer is currently empty.
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @return 1, 0
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesIsEmpty(epicsRingBytesId id);
/**
 * @brief Test if the ring buﬀer is currently full.
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @return 1, 0
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesIsFull(epicsRingBytesId id);
/**
 * @brief Returns the highest number of bytes the ring buffer contained
 * since the water mark has beeb reset. A new ring buffer starts with a water
 * mark of 0.
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 * @return Actual Highwater mark
 */
epicsShareFunc int  epicsShareAPI epicsRingBytesHighWaterMark(epicsRingBytesIdConst id);
/**
 * @brief Reset the Highwater mark of the ring buffer
 * The Highwater mark will be set to the current usage 
 * @param id RingbufferID returned by a former epicsRingBytesCreate 
 */
epicsShareFunc void epicsShareAPI epicsRingBytesResetHighWaterMark(epicsRingBytesId id);

#ifdef __cplusplus
}
#endif


#endif /* INCepicsRingBytesh */
