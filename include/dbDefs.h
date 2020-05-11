/*************************************************************************\
* Copyright (c) 2009 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file dbDefs.h
 * @author Marty Kraimer
 * @date  6-1-90
 *
 * @brief Miscellaneous database related functions
 *
 * This file containes a number of database related definitions.
 */

#ifndef INC_dbDefs_H
#define INC_dbDefs_H

#include <stddef.h>

#ifdef TRUE
#   undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#   undef FALSE
#endif
#define FALSE 0

#ifndef LOCAL
/**
 * @def LOCAL
 * @brief Deprecated, use static
 */
#   define LOCAL static
#endif

/**
 * @def NELEMENTS(array)
 * @brief Number of elements in an array
 */
#ifndef NELEMENTS
#   define NELEMENTS(array) (sizeof (array) / sizeof ((array) [0]))
#endif

/**
 * @def OFFSET(structure, member)
 * @brief Byte offset of member in structure
 * @note Deprecated, use offsetof
 */
#ifndef OFFSET
#   define OFFSET(structure, member) offsetof(structure, member)
#endif

/**
 * @def CONTAINER(ptr, structure, member)
 * @brief Subtract member byte offset, returning pointer to parent object
 */
#ifndef CONTAINER
# ifdef __GNUC__
#   define CONTAINER(ptr, structure, member) ({                     \
        const __typeof(((structure*)0)->member) *_ptr = (ptr);      \
        (structure*)((char*)_ptr - offsetof(structure, member));    \
    })
# else
#   define CONTAINER(ptr, structure, member) \
        ((structure*)((char*)(ptr) - offsetof(structure, member)))
# endif
#endif

/**
 * @def PVNAME_SZ (PVNAME_STRINGSZ - 1)
 * @brief Process Variable Name Size
 * @note PVNAME_STRINGSZ includes the nil terminator
 */
#define PVNAME_STRINGSZ 61
#define PVNAME_SZ (PVNAME_STRINGSZ - 1)

/**
 * @def PVLINK_STRINGSZ
 * @brief Buffer size for the string representation of a DBF_*LINK field
 */
#define PVLINK_STRINGSZ 1024

/**
 * @def DB_MAX_CHOICES
 * @brief dbAccess enums/menus can have up to this many choices
 */
#define DB_MAX_CHOICES 30

#endif /* INC_dbDefs_H */
