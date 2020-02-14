/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file dbmf.h
 * @author Jim Kowalkowski, Marty Kraimer
 * @date  4/97
 *
 * @brief A library to manage storage that is allocated and then freed.
 *
 * Database Macro/Free describes a facility that prevents memory fragmentation
 * when memory is allocated and then freed a short time later.
 *
 * Routines whin iocCore like dbLoadDatabase() have the following attributes:
 * 	- They repeatedly call mallo() followed soon afterwards by a call to
 * 	free() the temporaryily allocated storage.
 * 	- Between those calls to malloc() and free(), an additional call to
 * 	malloc() is made that does NOT have an associated free().
 *
 * @note In some environment, e.g. vxWorks, this behavior causes severe memory
 * fragmentation.
 *
 * @note This facility should NOT be used by code that allocates storage and
 * then keeps it for a considerable period of time before releasing. Such code
 * can use the freList library.
 */
#ifndef DBMF_H
#define DBMF_H

#include <stdlib.h>
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the facility
 * @param size The maximum size request from dbmfMalloc() that will be
 * allocated from the dbmf pool (Size is always made a multiple of 8).
 * @param chunkItems Each time malloc() must be called size*chunkItems bytes
 * are allocated.
 * @return 0 on success, -1 if already initialized
 *
 * @note If dbmfInit is not called before one of the other routines then it
 * is automatically called with size=64 and chunkItems=10
 */
epicsShareFunc int dbmfInit(size_t size, int chunkItems);
/**
 * @brief Allocate memory.
 * @param bytes If bytes > size then malloc() is used to allocate the memory.
 * @return Pointer to memory, NULL on failure
 */
epicsShareFunc void * dbmfMalloc(size_t bytes);
/**
 * @brief Duplicate a string.
 * Returns a pointer to a null-terminated byte string, which is a duplicate of
 * the string pointed to by str.
 * @param str Pointer to the null-terminated byte string to duplicate.
 * @return A pointer to the newly allocated string, or a NULL pointer if an
 * error occured.
 */
epicsShareFunc char * dbmfStrdup(const char *str);
/**
 * @brief Duplicate a string (up to len bytes).
 * Returns a pointer to a null-terminated byte string, which contains copies
 * of at most len bytes from the string pointed to by str. if the null
 * terminator is not encountered in the first size bytes, it is added to the
 * duplicated string.
 * @param str Pointer to the null-terminated byte string to duplicate.
 * @param len Max number of bytes to copy from str.
 * @return A pointer to the newly allocated string, or a NULL pointer if an
 * error occured.
 */
epicsShareFunc char * dbmfStrndup(const char *str, size_t len);
/**
 * @brief Concatenate three strings.
 * Returns a pointer to a null-terminated byte string, which is the
 * concatenation of the input strings.
 * @param lhs Start string to which the others get concatenated to (left part).
 * @param mid Next string to be concatenated to the lhs (mid part).
 * @param rhs Last string to be concatenated to the lhs+mid (right part).
 * @return A pointer to the newly allocated string, or a NULL pointer if an
 * error occured.
 */
epicsShareFunc char * dbmfStrcat3(const char *lhs, const char *mid,
    const char *rhs);
/**
 * @brief Free the memory allocated by dbmfMalloc.
 * @param bytes Pointer to memory obtained by dbmfMalloc.
 */
epicsShareFunc void dbmfFree(void *bytes);
/**
 * @brief Free all chunks that have contain only free items.
 */
epicsShareFunc void dbmfFreeChunks(void);
/**
 * @brief Show the status of the dbmf memory pool.
 * @param level Detail level.
 * @return Always 0.
 */
epicsShareFunc int dbmfShow(int level);

#ifdef __cplusplus
}
#endif

#endif
