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
 * @file epicsRingPointer.h
 * @author Marty Kraimer, Ralph Lange
 * @date 15. Jul 1999
 *
 * EpicsRingPointer provides methods for creating and using ring buﬀers
 * (ﬁrst in ﬁrst out circular buﬀers) that store pointers. The unlocked
 * variant is designed so that one writer thread and one reader thread can
 * access the ring simultaneously without requiring mutual exclusion. The
 * locked variant uses an epicsSpinLock, and works with any numbers of writer
 * and reader threads.
 * @note Here the terms push and pop are used, which have become familiar
 * for stack operations (LIFO). It would be better to use the terms 
 * enqueue and dequeue for FIFO operations.
 * @note If there is only one writer it is not necessary to lock push. 
 * If there is a single reader it is not necessary to lock pop. 
 * epicsRingPointerLocked uses a spinlock.
 *
 * @note Algorithm note:
 *  Space is allocated for one additional element.
 *  A put request is rejected if the the ring buffer is full.
 *  The algorithm does not require locking puts for a single writer
 *      or locking of gets for a single reader
 */

#ifndef INCepicsRingPointerh
#define INCepicsRingPointerh


#include "epicsSpin.h"
#include "shareLib.h"

#ifdef __cplusplus
/**
 * @brief Provides methods for creating and using ring buﬀers 
 * (ﬁrst in ﬁrst out circular buﬀers) that store pointers.
 */
template <class T>
class epicsRingPointer {
public: /* Functions */
    /**@brief Constructor
     * @param size Maximum number of elements (pointers) that can be stored in the ring
     * @param locked If locked is true, the spin lock secured variant is created
     */
    epicsRingPointer(int size, bool locked); 
    /**@brief Destructor
     */
    ~epicsRingPointer();
    /**@brief Push a new entry on the ring
     * @return It returns (false,true) is (failure, success).
     * Failure means the ring was full
     */
    bool push(T *p);
    /**@brief Take an element oﬀ the ring
     * @return It returns 0 (null) if the ring was empty
     */
    T* pop();
    /**@brief Remove all elements from the ring.
     * If this operation is performed on a ring buﬀer of the 
     * unsecured variant, all access to the ring should be locked.
     */
    void flush();
    /**@brief Get amount of empty space in the ring
     * @return Return the amount of empty space in the ring,
     * i.e. how many additional elements it can hold.
     */
    int getFree() const;
    /**@brief Get the number of elements stored on the ring
     * @return Return the number of elements stored on the ring
     */
    int getUsed() const;
    /**@brief Get the size of the ring
     * @return Return the size of the ring, 
     * i.e. the value of size speciﬁed when the ring was created.
     */
    int getSize() const;
    /**@brief Test on empty
     * @return Returns true if the ring is empty, else false.
     */
    bool isEmpty() const;
    /**@brief Test on full 
     * @return Returns true if the ring is full, else false.
     */
    bool isFull() const;
    /**@brief Read high water mark 
     * @return Returns the highest number of elements the ring
     * buffer contained since the water mark has been reset. A new
     * ring buffer starts with a water mark of 0.
     */
    int getHighWaterMark() const;
    /**@brief Reset high water mark 
     * High water mark will be set to the current usage
     */
    void resetHighWaterMark();

private: /* Prevent compiler-generated member functions */
    /* default constructor, copy constructor, assignment operator */
    epicsRingPointer();
    epicsRingPointer(const epicsRingPointer &);
    epicsRingPointer& operator=(const epicsRingPointer &);
    int getUsedNoLock() const;

private: /* Data */
    epicsSpinId lock;
    volatile int nextPush;
    volatile int nextPop;
    int size;
    int highWaterMark;
    T  * volatile * buffer;
};

extern "C" {
#endif /*__cplusplus */

typedef void *epicsRingPointerId;
typedef void const *epicsRingPointerIdConst;
/**
 * @brief Create a new ring buﬀer
 * @param size Size of ring buffer to create
 * @return epicsRingPointerId or NULL on failure
 */
epicsShareFunc epicsRingPointerId  epicsShareAPI epicsRingPointerCreate(int size);
/**
 * @brief Create a new ring buﬀer, secured by a spinlock
 * @param size Size of ring buffer to create
 * @return epicsRingPointerId or NULL on non success
 */
epicsShareFunc epicsRingPointerId  epicsShareAPI epicsRingPointerLockedCreate(int size);
/**
 * @brief Delete the ring buﬀer and free any associated memory
 * @param id epicsRingPointerId returned by a former epicsRingPointerCreate 
 */
epicsShareFunc void epicsShareAPI epicsRingPointerDelete(epicsRingPointerId id);
/**
 * @brief Push pointer to the ring
 * @param id epicsRingPointerId returned by a former epicsRingPointerCreate
 * @param p Pointer to be pushed to the ring
 * @return Returns (0,1) if p (was not, was) put on ring
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerPush(epicsRingPointerId id,void *p);
/**
 * @brief Take a element oﬀ the ring
 * @param id epicsRingPointerId returned by a former epicsRingPointerCreate
 * @return Returns 0 (null) if the ring was empty
 */
epicsShareFunc void* epicsShareAPI epicsRingPointerPop(epicsRingPointerId id) ;
/**
 * @brief Remove all elements from the ring
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate 
 * @note If this operation is performed on a ring buﬀer of the unsecured
 * variant, all access to the ring should be locked.
 */
epicsShareFunc void epicsShareAPI epicsRingPointerFlush(epicsRingPointerId id);
/**
 * @brief Return the amount of empty space in the ring
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 * @return Return the amount of empty space in the ring, 
 * i.e. how many additional elements it can hold.
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerGetFree(epicsRingPointerId id);
/**
 * @brief Return the number of elements stored on the ring
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 * @return Return the amount of elements stored on the ring 
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerGetUsed(epicsRingPointerId id);
/**
 * @brief Return the size of the ring
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 * @return Return the size of the ring, 
 * i.e. the value of size speciﬁed when the ring was created.
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerGetSize(epicsRingPointerId id);
/**
 * @brief Returns 1 if the ring is empty, else 0
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 * @return Returns 1 if the ring is empty, else 0
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerIsEmpty(epicsRingPointerId id);
/**
 * @brief Returns 1 if the ring is full, else 0
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 * @return Returns 1 if the ring is full, else 0
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerIsFull(epicsRingPointerId id);
/**
 * @brief Get the Highwater mark of the ring buffer
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 * @return Returns the highest number of elements the ring buffer contained
 * since the water mark has been reset. A new ring buffer starts with the
 * water mark of 0.
 */
epicsShareFunc int  epicsShareAPI epicsRingPointerGetHighWaterMark(epicsRingPointerIdConst id);
/**
 * @brief Reset the Highwater mark of the ring buffer
 * The Highwater mark will be set to the current usage
 * @param id epicsRingPointerID returned by a former epicsRingPointerCreate
 */
epicsShareFunc void epicsShareAPI epicsRingPointerResetHighWaterMark(epicsRingPointerId id);

/* This routine was incorrectly named in previous releases */
#define epicsRingPointerSize epicsRingPointerGetSize

#ifdef __cplusplus
}
#endif
/* END OF DECLARATIONS */

/* INLINE FUNCTIONS */

#ifdef __cplusplus

template <class T>
inline epicsRingPointer<T>::epicsRingPointer(int sz, bool locked) :
    lock(0), nextPush(0), nextPop(0), size(sz+1), highWaterMark(0),
    buffer(new T* [sz+1])
{
    if (locked)
        lock = epicsSpinCreate();
}

template <class T>
inline epicsRingPointer<T>::~epicsRingPointer()
{
    if (lock) epicsSpinDestroy(lock);
    delete [] buffer;
}

template <class T>
inline bool epicsRingPointer<T>::push(T *p)
{
    if (lock) epicsSpinLock(lock);
    int next = nextPush;
    int newNext = next + 1;
    if(newNext>=size) newNext=0;
    if (newNext == nextPop) {
        if (lock) epicsSpinUnlock(lock);
        return(false);
    }
    buffer[next] = p;
    nextPush = newNext;
    int used = getUsedNoLock();
    if (used > highWaterMark) highWaterMark = used;
    if (lock) epicsSpinUnlock(lock);
    return(true);
}

template <class T>
inline T* epicsRingPointer<T>::pop()
{
    if (lock) epicsSpinLock(lock);
    int next = nextPop;
    if (next == nextPush) {
        if (lock) epicsSpinUnlock(lock);
        return(0);
    }
    T*p  = buffer[next];
    ++next;
    if(next >=size) next = 0;
    nextPop = next;
    if (lock) epicsSpinUnlock(lock);
    return(p);
}

template <class T>
inline void epicsRingPointer<T>::flush()
{
    if (lock) epicsSpinLock(lock);
    nextPop = 0;
    nextPush = 0;
    if (lock) epicsSpinUnlock(lock);
}

template <class T>
inline int epicsRingPointer<T>::getFree() const
{
    if (lock) epicsSpinLock(lock);
    int n = nextPop - nextPush - 1;
    if (n < 0) n += size;
    if (lock) epicsSpinUnlock(lock);
    return n;
}

template <class T>
inline int epicsRingPointer<T>::getUsedNoLock() const
{
    int n = nextPush - nextPop;
    if (n < 0) n += size;
    return n;
}

template <class T>
inline int epicsRingPointer<T>::getUsed() const
{
    if (lock) epicsSpinLock(lock);
    int n = getUsedNoLock();
    if (lock) epicsSpinUnlock(lock);
    return n;
}

template <class T>
inline int epicsRingPointer<T>::getSize() const
{
    return(size-1);
}

template <class T>
inline bool epicsRingPointer<T>::isEmpty() const
{
    bool isEmpty;
    if (lock) epicsSpinLock(lock);
    isEmpty = (nextPush == nextPop);
    if (lock) epicsSpinUnlock(lock);
    return isEmpty;
}

template <class T>
inline bool epicsRingPointer<T>::isFull() const
{
    if (lock) epicsSpinLock(lock);
    int count = nextPush - nextPop +1;
    if (lock) epicsSpinUnlock(lock);
    return((count == 0) || (count == size));
}

template <class T>
inline int epicsRingPointer<T>::getHighWaterMark() const
{
    return highWaterMark;
}

template <class T>
inline void epicsRingPointer<T>::resetHighWaterMark()
{
    if (lock) epicsSpinLock(lock);
    highWaterMark = getUsedNoLock();
    if (lock) epicsSpinUnlock(lock);
}

#endif /* __cplusplus */

#endif /* INCepicsRingPointerh */
