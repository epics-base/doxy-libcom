/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/**
 * @file asTrapWrite.h
 * @brief Trapping channel access writes 
 * @author  Marty Kraimer
 * @date    07NOV2000
 *
 * Access security provides a facility asTrapWrite that can trap
 * write requests and pass them to any facility that registers a
 * listener.
 */

#ifndef INCasTrapWriteh
#define INCasTrapWriteh

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @struct asTrapWriteMessage
 *
 * @note  starting with V4.4 the count (no_elements)
 * field is used (abused) to store the minor version number of the client.
 */
typedef struct asTrapWriteMessage {
    const char *userid; /**< @brief Userid of whoever orginated the request. */
    const char *hostid; /**< @brief Hostid of whoever orginated the request. */
    void *serverSpecific; /**< @brief The meaning of this field is server
			    specific. If the listerner uses this field it must
			    know what type of server is supplying the messages.
			    It is the value the server provides to asTrapWriteBefore.
                            */
    void *userPvt; /**<  @brief This field is for use by the asTrapWriteListener.
		     When the listener is called before the write, it has the 
		     value 0. The listener can give it any value it desires
		     and it will have the same value when the listener gets
		     called after the write. */
    int dbrType; /**< @brief Data type from ca/db_access.h, NOT dbFldTypes.h */
    int no_elements; /**< @brief See note, stores minor version number of the client */
    void *data;     /**< @brief Might be NULL if no data is available */
} asTrapWriteMessage;

/**
 * @typedef *asTrapWriteId
 * @brief Defines Id as void pointer.
 */
typedef void *asTrapWriteId;

/**
 * @typedef *asTrapWriteListener
 * @brief Define *asTrapWriteListener as asTrapWriteMessage
 *
 * asTrapWriteListener is called before and after the write is performed.
 * The listener can set userPvt on the before call and retrieve it after
 * after = (0,1) (before,after) the put.
 *
 * Each asTrapWriteMessage can change or may be deleted after the user's
 * asTrapWriteListener returns
 *
 * asTrapWriteListener delays the associated server thread so it must not
 * do anything that causes to to block.
*/
typedef void(*asTrapWriteListener)(asTrapWriteMessage *pmessage,int after);

/**
 * @brief Register function to be called on asTrapWriteListener.
 * @param func Funktion to be registered.
 * @return Pointer to listener Id.
 */
epicsShareFunc asTrapWriteId epicsShareAPI asTrapWriteRegisterListener(
    asTrapWriteListener func);
/**
 * @brief Unregister asTrapWriteListener.
 * @param id Listener id (obtained by asTrapWriteRegisterListener).
 */
epicsShareFunc void epicsShareAPI asTrapWriteUnregisterListener(
    asTrapWriteId id);

#ifdef __cplusplus
}
#endif

#endif /*INCasTrapWriteh*/
