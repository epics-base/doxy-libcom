/*************************************************************************\
* Copyright (c) 2010 Brookhaven Science Associates, as Operator of
*     Brookhaven National Laboratory.
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/**
 * @file devLib.h
 * @brief Support macros for allocation of common device resources
 * @author Marty Kraimer and Jeff Hill
 * @date 03-10-93
 *
 * Support for allocation of common device resources
 */
#ifndef EPICSDEVLIB_H
#define EPICSDEVLIB_H

/**
 * @defgroup nmlFuntions normalizeAndConvertFunctions
 * @{
 */
/**
 * @def devCreateMask(NBITS)
 * @brief Creates bit mask for nr. of requested bits
 */
#define devCreateMask(NBITS)	((1<<(NBITS))-1)
/**
 * @def devDigToNml(DIGITAL,NBITS)
 * @brief Normalize digital value and convert it to type double 
 */
#define devDigToNml(DIGITAL,NBITS) \
	(((double)(DIGITAL))/devCreateMask(NBITS))
/**
 * @def devNmlToDig(NORMAL,NBITS)
 * @brief Convert normalize value to digital 
 */
#define devNmlToDig(NORMAL,NBITS) \
	(((long)(NORMAL)) * devCreateMask(NBITS))
 /** @} */

/**
 * @defgroup alignFuntions alingnmentFunctions
 * @{
 */
/**
 * @def devCreateAlignmentMask(CTYPE)
 * @brief Creates alignment mask for CTYPE
 */
#define devCreateAlignmentMask(CTYPE)\
(sizeof(CTYPE)>sizeof(double)?sizeof(double)-1:sizeof(CTYPE)-1)

/**
 * @def devPtrAlignTest(PTR)
 * @brief Pointer aligned test, returns true if the pointer
 *        is on the worst case alignemnt boundary for its type
 */
#define devPtrAlignTest(PTR) (!(devCreateAlignmentMask(*PTR)&(long)(PTR)))
/** @} */

/*
 * error codes (and messages) associated with devLib.c
 */
/**
 * @defgroup errorCodes errorCodes
 * @{
 */
#define S_dev_success 0                 //!< no error, success
#define S_dev_vectorInUse (M_devLib| 1) //!< interrupt vector in use
#define S_dev_vecInstlFail (M_devLib| 2) //!< interrupt vector install failed
#define S_dev_uknIntType (M_devLib| 3) //!< Unrecognized interrupt type 
#define S_dev_vectorNotInUse (M_devLib| 4) //!< Interrupt vector not in use by caller
#define S_dev_badA16 (M_devLib| 5) //!< Invalid VME A16 address
#define S_dev_badA24 (M_devLib| 6) //!< Invalid VME A24 address
#define S_dev_badA32 (M_devLib| 7) //!< Invalid VME A32 address
#define S_dev_uknAddrType (M_devLib| 8) //!< Unrecognized address space type
#define S_dev_addressOverlap (M_devLib| 9) //!< Specified device address overlaps another device 
#define S_dev_identifyOverlap (M_devLib| 10) //!< This device already owns the address range 
#define S_dev_addrMapFail (M_devLib| 11) //!< Unable to map address 
#define S_dev_intDisconnect (M_devLib| 12) //!< Interrupt at vector disconnected from an EPICS device
#define S_dev_internal (M_devLib| 13) //!< Internal failure 
#define S_dev_intEnFail (M_devLib| 14) //!< Unable to enable interrupt level 
#define S_dev_intDissFail (M_devLib| 15) //!< Unable to disable interrupt level 
#define S_dev_noMemory (M_devLib| 16) //!< Memory allocation failed 
#define S_dev_addressNotFound (M_devLib| 17) //!< Specified device address unregistered 
#define S_dev_noDevice (M_devLib| 18) //!< No device at specified address
#define S_dev_wrongDevice (M_devLib| 19) //!< Wrong device type found at specified address
#define S_dev_badSignalNumber (M_devLib| 20) //!< Signal number (offset) to large
#define S_dev_badSignalCount (M_devLib| 21) //!< Signal count to large
#define S_dev_badRequest (M_devLib| 22) //!< Device does not support requested operation
#define S_dev_highValue (M_devLib| 23) //!< Parameter to high
#define S_dev_lowValue (M_devLib| 24) //!< Parameter to low
#define S_dev_multDevice (M_devLib| 25) //!< Specified address is ambiguous (more than one device responds)
#define S_dev_badSelfTest (M_devLib| 26) //!< Device self test failed
#define S_dev_badInit (M_devLib| 27) //!< Device failed during initialization
#define S_dev_hdwLimit (M_devLib| 28) //!< Input exceeds Hardware Limit
#define S_dev_deviceDoesNotFit (M_devLib| 29) //!< Unable to locate address space for device
#define S_dev_deviceTMO (M_devLib| 30) //!< Device timed out
#define S_dev_badFunction (M_devLib| 31) //!< Bad function pointer
#define S_dev_badVector (M_devLib| 32) //!< Bad interrupt vector
#define S_dev_badArgument (M_devLib| 33) //!< Bad function argument
#define S_dev_badISA (M_devLib| 34) //!< Invalid ISA address
#define S_dev_badCRCSR (M_devLib| 35) //!< Invalid VME CR/CSR address
#define S_dev_vxWorksIntEnFail S_dev_intEnFail //!< vxWorks interrupt enable fail

/** @} */

#endif /* EPICSDEVLIB_H */

/*
 * Retain compatibility by including VME by default
 */
#ifndef NO_DEVLIB_COMPAT
#  include "devLibVME.h"
#endif
