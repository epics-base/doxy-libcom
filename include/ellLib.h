/*************************************************************************\
* Copyright (c) 2010 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/
/**
 * @file ellLib.h
 * @author John Winans (ANL)
 * @author Andrew Johnson (ANL)
 *
 * @brief A doubly-linked list library
 *
 * It provides functionality similar to the vxWorks lstLib library.
 *
 * Supports the creation and maintenance of a doubly-linked list. The user
 * supplies a list descriptor (type ELLLIST) that will contain pointers to
 * the first and last nodes in the list, and a count of the number of
 * nodes in the list. The nodes in the list can be any user-defined structure,
 * but they must reserve space for two pointers as their first elements.
 * Both the forward and backward chains are terminated with a NULL pointer.
 */

#ifndef INC_ellLib_H
#define INC_ellLib_H

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @struct ELLNODE
 *  @brief List node item
 *  @var ELLNODE::next
 *    Pointer to next ELLNODE in the list
 *  @var ELLNODE::previous
 *    Pointer to previous ELLNODE in the list
 */
typedef struct ELLNODE {
    struct ELLNODE *next;
    struct ELLNODE *previous;
} ELLNODE;

/**
 * @defgroup ellMacroDefs ell Macros
 * @{
 */
/**
 * @def ELLNODE_INIT
 * @brief Creates a "NULL" Node
 */
#define ELLNODE_INIT {NULL, NULL}
/** @} */

/** @struct ELLLIST
 *  @brief List descriptor
 *  @var ELLLIST::node
 *    pointer to ELLNODE which contains next and previous pointer
 *  @var ELLLIST::count
 *    The number of nodes in the list
 */
typedef struct ELLLIST {
    ELLNODE node;
    int     count;
} ELLLIST;

/**
 * @defgroup ellMacroDefs ell Macros
 * @{
 */
/**
  * @def ELLLIST_INIT
  * @brief Creates an empty list
  */
#define ELLLIST_INIT {ELLNODE_INIT, 0}
/** @} */

typedef void (*FREEFUNC)(void *);

/**
 * @defgroup ellMacroDefs ell Macros
 * @{
 */
/**
  * @def ellInit
  * @brief Initialize a list descriptor
  * @param PLIST Pointer to list descriptor to be initialized
  */
#define ellInit(PLIST) {\
    (PLIST)->node.next = (PLIST)->node.previous = NULL;\
    (PLIST)->count = 0;\
}
/**
  * @def ellCount
  * @brief Report the number of nodes in a list
  * @param PLIST Pointer to list descriptor
  */
#define ellCount(PLIST)    ((PLIST)->count)
/**
  * @def ellFirst
  * @brief Find the first node in list
  * @param PLIST Pointer to list descriptor
  */
#define ellFirst(PLIST)    ((PLIST)->node.next)
/**
  * @def ellLast
  * @brief Find the last node in list
  * @param PLIST Pointer to list descriptor
  */
#define ellLast(PLIST)     ((PLIST)->node.previous)
/**
  * @def ellNext
  * @brief Find the next node in list
  * @param PNODE Pointer to node whose successor is to be found
  */
#define ellNext(PNODE)     ((PNODE)->next)
/**
  * @def ellPrevious
  * @brief Find the previous node in list
  * @param PNODE Pointer to node whose predecessor is to be found
  */
#define ellPrevious(PNODE) ((PNODE)->previous)
/**
  * @def ellFree
  * @brief Free up the list
  * @param PLIST List for which to free all nodes
  */
#define ellFree(PLIST)     ellFree2(PLIST, free)
/** @} */

/**
 * @brief Adds a node to the end of a list
 * @param pList Pointer to list descriptor
 * @param pNode Pointer to node to be added
 */
epicsShareFunc void ellAdd (ELLLIST *pList, ELLNODE *pNode);
/**
 * @brief Concatenates a list to the end of another list.
 * The list to be added is left empty. Either list (or both)
 * can be empty at the beginning of the operation.
 * @param pDstList Destination list
 * @param pAddList List to be added to dstList
 */
epicsShareFunc void ellConcat (ELLLIST *pDstList, ELLLIST *pAddList);
/**
 * @brief Deletes a node from a list.
 * @param pList Pointer to list descriptor
 * @param pNode Pointer to node to be deleted
 */
epicsShareFunc void ellDelete (ELLLIST *pList, ELLNODE *pNode);
/**
 * @brief Extract a sublist from a list.
 * @param pSrcList Pointer to source list
 * @param pStartNode First node in sublist to be extracted
 * @param pEndNode Last node in sublist to be extracted
 * @param pDstList Pointer to list where to put extracted list
 */
epicsShareFunc void ellExtract (ELLLIST *pSrcList, ELLNODE *pStartNode, ELLNODE *pEndNode, ELLLIST *pDstList);
/**
 * @brief Delete and returns the first node from a list
 * @param pList Pointer to list from which to get node
 * @return If the list is empty, NULL will be returned.
 */
epicsShareFunc ELLNODE * ellGet (ELLLIST *pList);
/**
 * @brief Delete and returns the last node from the list.
 * @param pList Pointer to list from which to get node
 * @return Returns the last node in the specified list. If
 * the list is empty, NULL will be returned.
 */
epicsShareFunc ELLNODE * ellPop (ELLLIST *pList);
/**
 * @brief Insert a node in a list after a specified node
 * Delete and returns the last node from the list.
 * @param plist Pointer to list into which to insert node
 * @param pPrev Pointer to node after which to insert
 * @param pNode Pointer to node to be inserted
 * @note If pPrev is NULL, then pNode will be inserted at the head of the list
 */
epicsShareFunc void ellInsert (ELLLIST *plist, ELLNODE *pPrev, ELLNODE *pNode);
/**
 * @brief Find the Nth node in a list
 * @param pList Pointer to list from which to get node
 * @param nodeNum N'th node
 * @return Returns the nodeNum'th element in pList. If
 * there is no nodeNum'th node in the list, NULL will be returned.
 */
epicsShareFunc ELLNODE * ellNth (ELLLIST *pList, int nodeNum);
/**
 * @brief Find a list node nStep steps away from a specified node
 * @param pNode The known node
 * @param nStep Number of steps away to find
 * @return Returns the node, nStep nodes forward from pNode. If
 * there is no node that many steps from pNode, NULL will be returned.
 */
epicsShareFunc ELLNODE * ellNStep (ELLNODE *pNode, int nStep);
/**
 * @brief Find a node in a list
 * @param pList Pointer to list in which to search
 * @param pNode Pointer to node to search for
 * @return The node number, or -1 if the node is not found.
 * @note The first node is 1.
 */
epicsShareFunc int  ellFind (ELLLIST *pList, ELLNODE *pNode);
/**
 * @author Michael Davidsaver
 * @date 2016
 *
 * @brief Stable (MergeSort) to given list.
 * @param pList Pointer to list to be sorted
 * @param pListCmp Compare function to be used
 * @note The comparison function cmp(A,B) is expected
 * to return -1 for A<B, 0 for A==B, and 1 for A>B.
 *
 * @note Use of mergesort algorithm based on analysis by
 * http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html
 */
typedef int (*pListCmp)(const ELLNODE* A, const ELLNODE* B);
epicsShareFunc void ellSortStable(ELLLIST *pList, pListCmp);
/**
 * @brief Frees the nodes in a list
 * It makes the list into an empty list, and calls
 * freeFunc() for all the nodes that are (were) in that list.
 * @param pList List for which to free all nodes
 * @param freeFunc Free function to be called
 * @note The nodes in the list are free()'d on the assumption that the node
 * structures were malloc()'d one-at-a-time and that the ELLNODE structure is
 * the first member of the parent structure.
 */
epicsShareFunc void ellFree2 (ELLLIST *pList, FREEFUNC freeFunc);
/**
 * @brief Verifies that the list is consistent
 * @param pList List to be verified
 */
epicsShareFunc void ellVerify (ELLLIST *pList);

#ifdef __cplusplus
}
#endif

#endif /* INC_ellLib_H */
