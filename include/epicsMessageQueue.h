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
 * \file epicsMessageQueue.h
 * \author W. Eric Norum norume@aps.anl.gov 630-252-4793
 *
 * A C++ and a C facility for interlocked communication between threads.
 *
 * Each C function corresponds to one of the C++ methods.
 */

#ifndef epicsMessageQueueh
#define epicsMessageQueueh

#include "epicsAssert.h"
#include "shareLib.h"

typedef struct epicsMessageQueueOSD *epicsMessageQueueId;

#ifdef __cplusplus

/** Provides methods for sending messages between threads on a first in,
 *  first out basis. It is designed so that a single message queue can
 *  be used with multiple writer and reader threads.
 *
 *  An epicsMessageQueue cannot be assigned to, copy-constructed, or
 *  constructed without giving the capacity and max-imumMessageSize
 *  arguments.
 * 
 *  The C++ compiler will object to some of the statements below:
 *  \code{.cpp}
 *  epicsMessageQueue mq0();   // Error: default constructor is private
 *  epicsMessageQueue mq1(10, 20); // OK
 *  epicsMessageQueue mq2(t1); // Error: copy constructor is private
 *  epicsMessageQueue*pmq;    // OK, pointer
 *  *pmq = mq1;               // Error: assignment operator is private
 *  pmq = &mq1;               // OK, pointer assignment and address-of
 *  \endcode
 **/
class epicsShareClass epicsMessageQueue {
public:

    /**
     *  Constructor. The capacity is the maximum number of messages,
     *  each containing 0 tomaximumMessageSize bytes, that can be stored
     *  in the message queue.
     **/
    epicsMessageQueue ( unsigned int capacity,
                        unsigned int maximumMessageSize );

    /**
     *  Destructor.
     **/
    ~epicsMessageQueue ();

    /**
     *  Try to send a message. This method may be called from a
     *  vxWorks or RTEMSinterrupt handler.
     *  \returns 0 if the message was sent to a receiver or queued for
     *  future delivery.
     *  \returns -1 if no more messages can be queued or if the message
     *  is larger than the queue’s maximum message size.
     **/
    int trySend ( void *message, unsigned int messageSize );

    int send ( void *message, unsigned int messageSize );

    /**
     *  Send a message.
     *  \returns 0 if the message was sent to a receiver or queued for
     *  future delivery.
     *  \returns -1 if the timeout, if any, was reached before the
     *  message could be sent or queued, or if the message is larger
     *  than the queue’s maximum message size.
     **/
    int send ( void *message, unsigned int messageSize, double timeout );

    /**
     *  Try to receive a message. If the message queue is non-empty, the
     *  first message on the queue is copied to the specified location
     *  and the length of the message is returned.
     * 
     *  If the pending  message is larger than the specified
     *  messageBufferSize it may either return -1, or truncate the
     *  message. It is most efficient if the messageBufferSize is equal
     *  to the maximumMessageSize with which the message queue was
     *  created.
     *  \returns -1 if the message queue is empty.
     **/
    int tryReceive ( void *message, unsigned int size );
 
    int receive ( void *message, unsigned int size );
 
    /**
     *  Wait until a message is sent and store it in the specified
     *  location.
     * 
     *  If the received message is larger than the specified
     *  messageBufferSize it may either return -1, or truncate the
     *  message. It is most efficient if the messageBufferSize is equal
     *  to the maximumMessageSize with which the message queue was
     *  created.
     *  \returns Number of bytes in the message.
     *  \returns -1 if a message is not received within the timeout
     *  interval.
     **/
    int receive ( void *message, unsigned int size, double timeout );
 
    /**
     *  Displays some  information about the message queue.
     *  \param level Controls the amount of information displayed.
     **/
    void show ( unsigned int level = 0 );

    /**
     *  \returns The number of messages presently in the queue.
     **/
    unsigned int pending ();

private:
    /**
     *  Prevent compiler-generated member functions default constructor,
     *  copy constructor, assignment operator.
     **/
    epicsMessageQueue();
    epicsMessageQueue(const epicsMessageQueue &);
    epicsMessageQueue& operator=(const epicsMessageQueue &);

    epicsMessageQueueId id;
};

extern "C" {
#endif /*__cplusplus */

epicsShareFunc epicsMessageQueueId epicsShareAPI epicsMessageQueueCreate(
    unsigned int capacity,
    unsigned int maximumMessageSize);
epicsShareFunc void epicsShareAPI epicsMessageQueueDestroy(
    epicsMessageQueueId id);
epicsShareFunc int epicsShareAPI epicsMessageQueueTrySend(
    epicsMessageQueueId id,
    void *message,
    unsigned int messageSize);
epicsShareFunc int epicsShareAPI epicsMessageQueueSend(
    epicsMessageQueueId id,
    void *message,
    unsigned int messageSize);
epicsShareFunc int epicsShareAPI epicsMessageQueueSendWithTimeout(
    epicsMessageQueueId id,
    void *message,
    unsigned int messageSize,
    double timeout);
epicsShareFunc int epicsShareAPI epicsMessageQueueTryReceive(
    epicsMessageQueueId id,
    void *message,
    unsigned int size);
epicsShareFunc int epicsShareAPI epicsMessageQueueReceive(
    epicsMessageQueueId id,
    void *message,
    unsigned int size);
epicsShareFunc int epicsShareAPI epicsMessageQueueReceiveWithTimeout(
    epicsMessageQueueId id,
    void *message,
    unsigned int size,
    double timeout);
epicsShareFunc int epicsShareAPI epicsMessageQueuePending(
    epicsMessageQueueId id);
epicsShareFunc void epicsShareAPI epicsMessageQueueShow(
    epicsMessageQueueId id,
    int level);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "osdMessageQueue.h"

#endif /* epicsMessageQueueh */
