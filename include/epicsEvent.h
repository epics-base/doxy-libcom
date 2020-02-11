/*************************************************************************\
* Copyright (c) 2011 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/** @file epicsEvent.h **/

#ifndef epicsEventh
#define epicsEventh

#include "shareLib.h"

/** @brief Defines the C++ and C API's for a simple binary semaphore. If multiple threads are
 *  waiting on the same event, only one of them will be woken when the event is signalled.
 **/

typedef struct epicsEventOSD *epicsEventId;

typedef enum {
    epicsEventOK = 0,
    epicsEventWaitTimeout,
    epicsEventError
} epicsEventStatus;

/* Backwards compatibility */
#define epicsEventWaitStatus epicsEventStatus
#define epicsEventWaitOK epicsEventOK
#define epicsEventWaitError epicsEventError

typedef enum {
    epicsEventEmpty,
    epicsEventFull
} epicsEventInitialState;

#ifdef __cplusplus

/** @brief Create an epicsEvent.
 *
 *  The primary use of an event semaphore is for thread synchronization. An example of using an
 *  event semaphore is a consumer thread that processes requests from one or more producer threads.
 *  For example:
 *
 *  When creating the consumer thread also create an epicsEvent.
 @code
     epicsEvent *pevent = new epicsEvent;
 @endcode
 *  The consumer thread has code containing:
 @code
       while(1) {
           pevent->wait();
           while( {more work} ) {
               {process work}
           }
       }
 @endcode
 *  Producers create requests and issue the statement:
 @code
       pevent->signal();
 @endcode
 **/
class epicsShareClass epicsEvent {
public:
    /**@brief An epicsEvent can be created empty or full.
     * @note If it is created empty then a wait is issued before a signal will block.
     * @note If created full then the first wait will always succeed.
     * @note Multiple signals may be issues between waits but have the same effect as a single signal.
     **/
    epicsEvent ( epicsEventInitialState initial = epicsEventEmpty );
    /** @brief Remove the event and any resources it uses. Any further use of the semaphore result
     *  in unknown (most certainly bad) behaviour. No outstanding take can be active when this call
     *  is made.
     **/
    ~epicsEvent ();
    void trigger ();
    /** @brief Signal the event i.e. ensures the next or current call to wait completes. This method
     *  may be called from a vxWorks or RTEMS interrupt handler.
     **/
    void signal () { this->trigger(); }
    /** @breif Wait for the event.
        @note Blocks until full,
     **/
    void wait ();
    /** @brief Wait for the event or until the specified timeout.
     *  @param timeOut A double to wait for timeout. 
     *  @return True if the event was signalled, False if timed out.
     **/
    bool wait ( double timeOut );
    /** @brief Similar to wait except that if the event does not happen the call completes
     *  immediately.
     *  @return True if an unused event has already been signalled, False if not.
     **/
    bool tryWait ();                /* false if empty */
    /** @brief Display information about the semaphore.
     *  @note The information displayed is architecture dependant.
     *  @param level An unsigned int for the level of information to be displayed.
     **/
    void show ( unsigned level ) const;

    class invalidSemaphore;         /* exception payload */
private:
    epicsEvent ( const epicsEvent & );
    epicsEvent & operator = ( const epicsEvent & );
    epicsEventId id;
};

extern "C" {
#endif /*__cplusplus */

epicsShareFunc epicsEventId epicsEventCreate(
    epicsEventInitialState initialState);
epicsShareFunc epicsEventId epicsEventMustCreate (
    epicsEventInitialState initialState);
epicsShareFunc void epicsEventDestroy(epicsEventId id);
epicsShareFunc epicsEventStatus epicsEventTrigger(
    epicsEventId id);
epicsShareFunc void epicsEventMustTrigger(epicsEventId id);
#define epicsEventSignal(ID) epicsEventMustTrigger(ID)
epicsShareFunc epicsEventStatus epicsEventWait(
    epicsEventId id);
epicsShareFunc void epicsEventMustWait(epicsEventId id);
epicsShareFunc epicsEventStatus epicsEventWaitWithTimeout(
    epicsEventId id, double timeOut);
epicsShareFunc epicsEventStatus epicsEventTryWait(
    epicsEventId id);
epicsShareFunc void epicsEventShow(
    epicsEventId id, unsigned int level);

#ifdef __cplusplus
}
#endif /*__cplusplus */

#include "osdEvent.h"

#endif /* epicsEventh */
