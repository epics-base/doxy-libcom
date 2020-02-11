/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/**
 * @file bucketLib.h
 * @author Jeffrey O. Hill
 * @date  Sep 1993 
 *
 * @brief describes a hash facility
 *
 * Hash facility for integers, pointers, and strings. It is used by
 * the Channel Access Server. It is currently mostly undocumented.
 *
 * @note Storage for identifier must persist until an item is deleted
 */

#ifndef INCbucketLibh
#define INCbucketLibh

#ifdef __cplusplus
extern "C" {
#endif

#include "errMdef.h"
#include "epicsTypes.h"
#include "shareLib.h"

typedef	unsigned 	BUCKETID;

/** @brief enum definition of buckTypeOfId
 *
 @code
typedef enum {bidtUnsigned, bidtPointer, bidtString} buckTypeOfId;
 @endcode
 *
 */
typedef enum {bidtUnsigned, bidtPointer, bidtString} buckTypeOfId;

/** @struct item
 *
 *  @var ITEM::pItem
 *    pointer to sruct item
 *  @var ITEM::pId
 *    pointer to id?
 *  @var ITEM::pApp
 *    pointer to app?
 *  @var ITEM::type
 *    type of bucket
 */
typedef struct item{
	struct item	*pItem;
	const void	*pId;
	const void   *pApp;
	buckTypeOfId	type;
}ITEM;

/** @struct bucket
 *
 *  @var BUCKET::pTable
 *    pointer to items
 *  @var BUCKET::freeListPVT
 *    fre list?
 *  @var BUCKET::hashIdMask
 *    no idea
 *  @var BUCKET::hashIdNBits
 *    no idea
 *  @var BUCKET::nInUse
 *    no idea
 */
typedef struct bucket{
	ITEM		**pTable;
	void		*freeListPVT;
	unsigned	hashIdMask;
	unsigned	hashIdNBits;
        unsigned        nInUse;
}BUCKET;
/**
 * creates a new bucket
 * @param nHashTableEntries tbd
 * @return  the newly created bucket.
 */
epicsShareFunc BUCKET * epicsShareAPI bucketCreate (unsigned nHashTableEntries);
/**
 * gives back a bucket (free it)
 * @param *prb pointer to an existing bucket
 * @return  the newly created bucket.
 */
epicsShareFunc int epicsShareAPI bucketFree (BUCKET *prb);
/**
 * shows a bucket
 * @param *pb pointer to a existing bucket
 * @return  S_bucket_success (0)
 */
epicsShareFunc int epicsShareAPI bucketShow (BUCKET *pb);

/**
 * @note Identifier must exist (and remain constant) at the specified address until
 * the item is deleted from the bucket 
 */
/**
 * adds an identifier (unsigned int) to the bucket
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @param *pApp hash value
 * @return S_bucket_ status, should be S_bucket_success if ok
 */
epicsShareFunc int epicsShareAPI bucketAddItemUnsignedId (BUCKET *prb, 
		const unsigned *pId, const void *pApp);
/**
 * adds an identifier (pointer) to the bucket
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @param *pApp hash value
 * @return S_bucket_ status, should be S_bucket_success if ok
 */
epicsShareFunc int epicsShareAPI bucketAddItemPointerId (BUCKET *prb, 
		void * const *pId, const void *pApp);
/**
 * adds an identifier (string) to the bucket
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @param *pApp hash value
 * @return S_bucket_ status, should be S_bucket_success if ok
 */
epicsShareFunc int epicsShareAPI bucketAddItemStringId (BUCKET *prb, 
		const char *pId, const void *pApp);
/**
 * removes an identifier (string) from the bucket
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return S_bucket_ status, should be S_bucket_success if ok
 */
epicsShareFunc int epicsShareAPI bucketRemoveItemUnsignedId (BUCKET *prb, const unsigned *pId);
/**
 * removes an identifier (pointer) from the bucket
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return S_bucket_ status, should be S_bucket_success if ok
 */
epicsShareFunc int epicsShareAPI bucketRemoveItemPointerId (BUCKET *prb, void * const *pId);
/**
 * removes an identifier (string) from the bucket
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return S_bucket_ status, should be S_bucket_success if ok
 */
epicsShareFunc int epicsShareAPI bucketRemoveItemStringId (BUCKET *prb, const char *pId);
/**
 * lookup an identifier (unsigned int)
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return hash value
 */
epicsShareFunc void * epicsShareAPI bucketLookupItemUnsignedId (BUCKET *prb, const unsigned *pId);
/**
 * lookup an identifier (pointer)
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return hash value
 */
epicsShareFunc void * epicsShareAPI bucketLookupItemPointerId (BUCKET *prb, void * const *pId);
/**
 * lookup an identifier (string)
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return hash value
 */
epicsShareFunc void * epicsShareAPI bucketLookupItemStringId (BUCKET *prb, const char *pId);

/**
 * lookup and remove an identifier (unsigned int)
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return hash value
 */
epicsShareFunc void * epicsShareAPI bucketLookupAndRemoveItemUnsignedId (BUCKET *prb, const unsigned *pId);
/**
 * lookup and remove an identifier (pointer)
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return hash value
 */
epicsShareFunc void * epicsShareAPI bucketLookupAndRemoveItemPointerId (BUCKET *prb, void * const *pId);
/**
 * lookup and remove an identifier (string)
 * @param *prb pointer to a existing bucket
 * @param *pId pointer to the identifier
 * @return hash value
 */
epicsShareFunc void * epicsShareAPI bucketLookupAndRemoveItemStringId (BUCKET *prb, const char *pId);


 /**
 * @defgroup bucketLibDefs bucketLib Macros
 * @{
 */
/**
 * @def BUCKET_SUCCESS
 * @brief defined to S_bucket_success (0)
 */
#define BUCKET_SUCCESS		S_bucket_success
/**
 * @def S_bucket_success
 * @brief defined as 0
 */
#define S_bucket_success	0
/**
 * @def S_bucket_noMemory
 * @brief Memory allocation failed
 */
#define S_bucket_noMemory	(M_bucket | 1)
/**
 * @def S_bucket_idInUse
 * @brief Identifier already in use
 */
#define S_bucket_idInUse	(M_bucket | 2)
/**
 * @def S_bucket_uknId
 * @brief Unknown identifier
 */
#define S_bucket_uknId		(M_bucket | 3)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /*INCbucketLibh*/

