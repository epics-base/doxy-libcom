/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* Copyright (c) 2013 ITER Organization.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/**
 * \file epicsThread.h
 *
 * \brief C++ and C descriptions for a thread.
 *
 * The epicsThread API is meant as a somewhat minimal interface for
 * multithreaded applications. It can be implementedon a wide variety of
 * systems with the restriction that the system MUST support a
 * multithreaded environment.
 * A POSIX pthreads version is provided.
 *
 * The interface provides the following thread facilities,
 * with restrictions as noted:
 * - Life cycle: a thread starts life as a result of a call to
 *   epicsThreadCreate. It terminates when the thread function returns.
 *   It should not return until it has released all resources it uses.
 *   If a thread is expected to terminate as a natural part of its life
 *   cycle then the thread function must return.
 * - epicsThreadOnce: this provides the ability to have an
 *   initialization function that is guaranteed to be called exactly
 *   once.
 * - main: if a main routine finishes its work but wants to leave other
 *   threads running it can call epicsThreadExitMain, which should be
 *   the last statement in main.
 * - Priorities: ranges between 0 and 99 with a higher number meaning
 *   higher priority. A number of constants are defined for iocCore
 *   specific threads. The underlying implementation may collapse the
 *   range 0 to 99 into a smaller range; even a single priority. User
 *   code should never rely on the existence of multiple thread
 *   priorities to guarantee correct behavior.
 * - Stack Size: epicsThreadCreate accepts a stack size parameter. Three
 *   generic sizes are available: small, medium,and large. Portable code
 *   should always use one of the generic sizes. Some implementation may
 *   ignore the stack size request and use a system default instead.
 *   Virtual memory systems providing generous stack sizes can be
 *   expected to use the system default.
 * - epicsThreadId: every epicsThread has an Id which gets returned by
 *   epicsThreadCreate and is valid as long as that thread still exists.
 *   A value of 0 always means no thread. If a threadId is used after
 *   the thread has terminated,the results are not defined (but will
 *   normally lead to bad things happening). Thus code that looks after
 *   other threads MUST be aware of threads terminating.
 */

#ifndef epicsThreadh
#define epicsThreadh

#include <stddef.h>

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*EPICSTHREADFUNC)(void *parm);

#define epicsThreadPriorityMax          99
#define epicsThreadPriorityMin           0

/* some generic values */
#define epicsThreadPriorityLow          10
#define epicsThreadPriorityMedium       50
#define epicsThreadPriorityHigh         90

/* some iocCore specific values */
#define epicsThreadPriorityCAServerLow  20
#define epicsThreadPriorityCAServerHigh 40
#define epicsThreadPriorityScanLow      60
#define epicsThreadPriorityScanHigh     70
#define epicsThreadPriorityIocsh        91
#define epicsThreadPriorityBaseMax      91

/** Stack sizes for each stackSizeClass are implementation and CPU dependent. */
typedef enum {
    epicsThreadStackSmall, epicsThreadStackMedium, epicsThreadStackBig
} epicsThreadStackSizeClass;

typedef enum {
    epicsThreadBooleanStatusFail, epicsThreadBooleanStatusSuccess
} epicsThreadBooleanStatus;

/**
 * Get a stack size value that can be given to epicsThreadCreate().
 * \param size one of the values epicsThreadStackSmall,
 * epicsThreadStackMedium or epicsThreadStackBig.
 **/
epicsShareFunc unsigned int epicsShareAPI epicsThreadGetStackSize(
    epicsThreadStackSizeClass size);

/** (epicsThreadId)0 is guaranteed to be an invalid thread id. */
typedef struct epicsThreadOSD *epicsThreadId;

typedef epicsThreadId epicsThreadOnceId;
#define EPICS_THREAD_ONCE_INIT 0

/**
 * This is used as follows:
 * \code{.cpp}
 *   void myInitFunc(void*arg) {...}
 *   epicsThreadOnceId onceFlag=EPICS_THREAD_ONCE_INIT;
 *   ...
 *   epicsThreadOnce(&onceFlag, myInitFunc,(void*)myParm);
 * \endcode
 * For each unique epicsThreadOnceId, epicsThreadOnce guarantees that
 * -# myInitFunc will only be called only once.
 * -# myInitFunc will have re-turned before any other epicsThreadOnce
 *   call returns.
 *
 * Note that myInitFunc must not call epicsThreadOnce with the same
 * onceId.
 **/
epicsShareFunc void epicsShareAPI epicsThreadOnce(
    epicsThreadOnceId *id, EPICSTHREADFUNC, void *arg);

/**
 * When real-time scheduling is active, attempt any post-init operations
 * that preserve real-time performance. For POSIX targets this locks the
 * process into RAM, preventing swap-related VM faults.
 **/
epicsShareFunc void epicsThreadRealtimeLock(void);

/**
 * If the main routine is done but wants to let other threads run it can
 * call this routine. This should be the last call in main, except the
 * final return. On most systems epicsThreadExitMain never returns.This
 * must only be called by the main thread.
 **/
epicsShareFunc void epicsShareAPI epicsThreadExitMain(void);

/**
 * Create a new thread. The use made of the priority, and stackSize
 * arguments is implementation dependent. Some implementations may
 * ignore one or other of these, but for portability appropriate values
 * should be given for both.  A thread terminates when funptr returns.
 * \param name
 * \param priority Implementation dependent.
 * \param stackSize Implementation dependent; should be obtained by
 *        calling epicsThreadGetStackSize().
 * \param funptr Function that implements the thread.
 * \param parm single argument passed to funptr.
 * \return thread id or zero on failure.
 **/
epicsShareFunc epicsThreadId epicsShareAPI epicsThreadCreate (
    const char * name, unsigned int priority, unsigned int stackSize,
    EPICSTHREADFUNC funptr,void * parm );

/**
 * Create a new thread, see epicsThreadCreate() for details. Does not
 * return if the thread could not be created.
 **/
epicsShareFunc epicsThreadId epicsShareAPI epicsThreadMustCreate (
    const char * name, unsigned int priority, unsigned int stackSize,
    EPICSTHREADFUNC funptr,void * parm );

/**
 * This causes the calling thread to suspend. The only way it can resume
 * is for another thread to call epicsThreadResume().
 **/
epicsShareFunc void epicsShareAPI epicsThreadSuspendSelf(void);

/**
 * Resume a suspended thread. Only do this if you know that it is safe
 * to resume a suspended thread.
 **/
epicsShareFunc void epicsShareAPI epicsThreadResume(epicsThreadId id);

/**
 * Get the priority of the specified thread.
 **/
epicsShareFunc unsigned int epicsShareAPI epicsThreadGetPriority(
    epicsThreadId id);

/**
 * Get the priority of this thread.
 **/
epicsShareFunc unsigned int epicsShareAPI epicsThreadGetPrioritySelf(void);

/**
 * Set a new priority for the specified thread. The result is
 * implementation dependent.
 **/
epicsShareFunc void epicsShareAPI epicsThreadSetPriority(
    epicsThreadId id,unsigned int priority);

/**
 * Get a priority that is just lower than the specified priority.
 **/
epicsShareFunc epicsThreadBooleanStatus epicsShareAPI
    epicsThreadHighestPriorityLevelBelow (
        unsigned int priority, unsigned *pPriorityJustBelow);

/**
 * Get a priority that is just above the specified priority.
 **/
epicsShareFunc epicsThreadBooleanStatus epicsShareAPI
    epicsThreadLowestPriorityLevelAbove (
        unsigned int priority, unsigned *pPriorityJustAbove);

/**
 * Compares two threadIds.
 * \return 0 if they are not the same
 * \return 1 if they are the same
 **/
epicsShareFunc int epicsShareAPI epicsThreadIsEqual(
    epicsThreadId id1, epicsThreadId id2);

/**
 * How and why a thread can be suspended is implementation dependent. A
 * thread calling epicsThreadSuspendSelf() should result in this routine
 * returning true for that thread, but a thread may also be suspended
 * for other reasons.
 **/
epicsShareFunc int epicsShareAPI epicsThreadIsSuspended(epicsThreadId id);

/**
 * Sleep for the specified period of time, i.e. sleep without using the
 * cpu.
 *
 * If delay is >0 then the thread will sleep at least until the next
 * clock tick. The exact time is determined by the underlying
 * architecture.
 *
 * If delay is <=0 then a delay of 0 is requested of the underlying
 * architecture. What happens is architecture dependent but often it
 * allows other threads of the same priority to run.
 **/
epicsShareFunc void epicsShareAPI epicsThreadSleep(double seconds);

/**
 * This function returns the minimum slumber interval obtainable with
 * epicsThreadSleep() in seconds. On most OS there is a system scheduler
 * interrupt interval which determines the value of this parameter.
 * Knowledge of this parameter is used by the various components of
 * EPICS to improve scheduling of software tasks intime when the
 * reduction of average time scheduling errors is important. If this
 * parameter is unknown or is unpredictable for a particular OS then it
 * is safe to return zero.
 **/
epicsShareFunc double epicsShareAPI epicsThreadSleepQuantum(void);

/**
 * Get the threadId of the calling thread.
 **/
epicsShareFunc epicsThreadId epicsShareAPI epicsThreadGetIdSelf(void);

/**
 * Get the threadId of the specified thread.
 * \return 0 means that no thread was found with the specified name.
 **/
epicsShareFunc epicsThreadId epicsShareAPI epicsThreadGetId(const char *name);

/**
 * Get the number of CPUs (logical cores) available to the IOC. On
 * systems that provide Hyper-Threading, this may be more the number of
 * physical CPU cores.
 **/
epicsShareFunc int epicsThreadGetCPUs(void);

/**
 * Get the name of the calling thread.
 **/
epicsShareFunc const char * epicsShareAPI epicsThreadGetNameSelf(void);

/**
 * Get the name of the specified thread. The value is copied to a caller
 * specified buffer so that if the thread terminates the caller is not
 * left with a pointer to something that may no longer exist.
 * \param id
 * \param name guaranteed to be null terminated.
 * \param size size of buffer to hold name (including terminator).
 * \return Failure results in an empty string stored in name.
 **/
epicsShareFunc void epicsShareAPI epicsThreadGetName(
    epicsThreadId id, char *name, size_t size);

/**
 * Is it OK for a thread to block? This can be called by support code
 * that does not know if it is called in a thread that should not block.
 * For example the errlog system calls this to decide when messages
 * should be displayed on the console.
 **/
epicsShareFunc int epicsShareAPI epicsThreadIsOkToBlock(void);

/**
 * When a thread is started the default is that it is not allowed to
 * block. This method can be called to change the state. For example
 * iocsh calls this to specify that it is OK to block.
 **/
epicsShareFunc void epicsShareAPI epicsThreadSetOkToBlock(int isOkToBlock);

/**
 * Display info about all threads.
 **/
epicsShareFunc void epicsShareAPI epicsThreadShowAll(unsigned int level);

/**
 * Display info about the specified thread.
 **/
epicsShareFunc void epicsShareAPI epicsThreadShow(
    epicsThreadId id,unsigned int level);

/**
 * Hooks called when a thread starts, map function called once for every thread.
 **/
typedef void (*EPICS_THREAD_HOOK_ROUTINE)(epicsThreadId id);

/**
 * Register a routine to be called by every new thread before the thread
 * function gets run. Hook routines will often register a thread exit
 * routine with epicsAtThreadExit() to release thread-specific resources
 * they have allocated.
 */
epicsShareFunc int epicsThreadHookAdd(EPICS_THREAD_HOOK_ROUTINE hook);

/**
 * Remove routine from the list of hooks run at thread creation time.
 **/
epicsShareFunc int epicsThreadHookDelete(EPICS_THREAD_HOOK_ROUTINE hook);

/**
 * Print the current list of hook function pointers.
 **/
epicsShareFunc void epicsThreadHooksShow(void);

/**
 * Call func once for every known thread.
 **/
epicsShareFunc void epicsThreadMap(EPICS_THREAD_HOOK_ROUTINE func);

typedef struct epicsThreadPrivateOSD * epicsThreadPrivateId;

/**
 * Thread private variables are intended for use by legacy libraries
 * written for a single threaded environment and which used a global
 * variable to store private data. The only code in base that currently
 * needs this facility is channel access. A library that needs a private
 * variable should make exactly one call to epicsThreadPrivateCreate()
 * and store the index returned. Each thread can later call
 * epicsThreadPrivateGet() and epicsThreadPrivateSet() with that index
 * to access a thread-specific pointer store.
 **/
epicsShareFunc epicsThreadPrivateId epicsShareAPI epicsThreadPrivateCreate(void);

/**
 * Delete a thread private variable.
 **/
epicsShareFunc void epicsShareAPI epicsThreadPrivateDelete(epicsThreadPrivateId id);

/**
 * Set the value for a thread private pointer.
 **/
epicsShareFunc void epicsShareAPI epicsThreadPrivateSet(epicsThreadPrivateId,void *);

/**
 * Get the value of a thread private pointer. The value returned is the
 * last value given to epicsThreadPrivateSet() by the same thread. If
 * called before epicsThreadPrivateSet() the pointer’s value is NULL.
 **/
epicsShareFunc void * epicsShareAPI epicsThreadPrivateGet(epicsThreadPrivateId);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include "epicsEvent.h"
#include "epicsMutex.h"

/**
 * Code using the C++ API must provide a class that derives from
 * epicsThreadRunable.
 *
 * For example:
 * \code{.cpp}
 * class myThread: public epicsThreadRunable {
 *   public:
 *     myThread(int arg,const char*name);
 *     virtual ~myThread();
 *     virtual void run();
 *     epicsThread thread;
 * }
 *
 * myThread::myThread(int arg,const char*name) :
 *   thread(*this,name,epicsThreadGetStackSize(epicsThreadStackSmall),50) {
 *     thread.start();
 * }
 *
 * myThread::~myThread() {}
 *
 * void myThread::run() {
 *   // ...
 * }
 * \endcode
 **/

class epicsShareClass epicsThreadRunable {
public:
    virtual ~epicsThreadRunable () = 0;
    virtual void run () = 0;
    virtual void show ( unsigned int level ) const;
};

extern "C" void epicsThreadCallEntryPoint ( void * );

/**
 * The C++ interface is a wrapper around the C interface.
 *
 * Two differences are the method start and the class
 * epicsThreadRunable.
 *
 * The method start must not be called until after
 * the epicsThread constructor has returned. Calling the start method
 * allows the run method of the epicsThreadRunableobject to be executed
 * in the context of the new thread.
 **/
class epicsShareClass epicsThread {
public:
    epicsThread ( epicsThreadRunable &,const char *name, unsigned int stackSize,
        unsigned int priority=epicsThreadPriorityLow );
    ~epicsThread () throw ();
    void start () throw ();
    void exitWait () throw ();
    bool exitWait ( const double delay ) throw ();
    static void exit ();
    void resume () throw ();
    void getName ( char * name, size_t size ) const throw ();
    epicsThreadId getId () const throw ();
    unsigned int getPriority () const throw ();
    void setPriority ( unsigned int ) throw ();
    bool priorityIsEqual ( const epicsThread & ) const throw ();
    bool isSuspended () const throw ();
    bool isCurrentThread () const throw ();
    bool operator == ( const epicsThread & ) const throw ();
    void show ( unsigned level ) const throw ();

    /* these operate on the current thread */
    static void suspendSelf () throw ();
    static void sleep (double seconds) throw ();
    static const char * getNameSelf () throw ();
    static bool isOkToBlock () throw ();
    static void setOkToBlock ( bool isOkToBlock ) throw ();

    /* exceptions */
    class unableToCreateThread;
private:
    epicsThreadRunable & runable;
    epicsThreadId id;
    epicsMutex mutex;
    epicsEvent event;
    epicsEvent exitEvent;
    bool * pThreadDestroyed;
    bool begin;
    bool cancel;
    bool terminated;

    bool beginWait () throw ();
    epicsThread ( const epicsThread & );
    epicsThread & operator = ( const epicsThread & );
    friend void epicsThreadCallEntryPoint ( void * );
    void printLastChanceExceptionMessage (
        const char * pExceptionTypeName,
        const char * pExceptionContext );
    /* exceptions */
    class exitException {};
};

class epicsShareClass epicsThreadPrivateBase {
public:
    class unableToCreateThreadPrivate {}; /* exception */
protected:
    static void throwUnableToCreateThreadPrivate ();
};

template < class T >
class epicsThreadPrivate :
    private epicsThreadPrivateBase {
public:
    epicsThreadPrivate ();
    ~epicsThreadPrivate () throw ();
    T * get () const throw ();
    void set (T *) throw ();
private:
    epicsThreadPrivateId id;
};

#endif /* __cplusplus */

#include "osdThread.h"

#ifdef __cplusplus

template <class T>
inline epicsThreadPrivate<T>::epicsThreadPrivate ()
{
    this->id = epicsThreadPrivateCreate ();
    if ( this->id == 0 ) {
        epicsThreadPrivateBase::throwUnableToCreateThreadPrivate ();
    }
}

template <class T>
inline epicsThreadPrivate<T>::~epicsThreadPrivate () throw ()
{
    epicsThreadPrivateDelete ( this->id );
}

template <class T>
inline T *epicsThreadPrivate<T>::get () const throw ()
{
    return static_cast<T *> ( epicsThreadPrivateGet (this->id) );
}

template <class T>
inline void epicsThreadPrivate<T>::set (T *pIn) throw ()
{
    epicsThreadPrivateSet ( this->id, static_cast<void *> (pIn) );
}

#endif /* ifdef __cplusplus */

#endif /* epicsThreadh */
