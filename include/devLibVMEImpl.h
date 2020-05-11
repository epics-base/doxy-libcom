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
 * @file devLibVMEImpl.h
 * @note Should it not be devLibImpl.h ?
 * @author Marty Kraimer, Jeff Hill
 * @date   03-10-93
 *
 * Virtual OS layer for devLib.c
 *
 */

#ifndef INCdevLibImplh
#define INCdevLibImplh 1

#include "dbDefs.h"
#include "shareLib.h"
#include "devLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct devLibVME
 *
 * The global virtual OS table pdevLibVME controls
 * the behaviour of the functions defined in devLib.h.
 * All of which call into the functions found in this table
 * to perform system specific tasks.
 */
typedef struct devLibVME {
	long (*pDevMapAddr) (epicsAddressType addrType, unsigned options,
			size_t logicalAddress, size_t size, volatile void **ppPhysicalAddress); //!< Maps logical address to physical address, but does not detect two device drivers that are using the same address range.

	long (*pDevReadProbe) (unsigned wordSize, volatile const void *ptr, void *pValueRead); //!< A bus error safe "wordSize" read at the specified address which returns unsuccessful status if the device isnt present.

	long (*pDevWriteProbe) (unsigned wordSize, volatile void *ptr, const void *pValueWritten); //!< A bus error safe "wordSize" write at the specified address which returns unsuccessful status if the device isnt present.

	long (*pDevConnectInterruptVME) (unsigned vectorNumber, 
						void (*pFunction)(void *), void  *parameter); //!< Connect ISR to a VME interrupt vector. (required for backwards compatibility)

	long (*pDevDisconnectInterruptVME) (unsigned vectorNumber,
						void (*pFunction)(void *)); //!< Disconnect ISR from a VME interrupt vector. (required for backwards compatibility)
	long (*pDevEnableInterruptLevelVME) (unsigned level); //!< Enable VME interrupt level.

	long (*pDevDisableInterruptLevelVME) (unsigned level); //!< Disable VME interrupt level.
        void *(*pDevA24Malloc)(size_t nbytes); //!< Malloc/free A24 address space.
        void (*pDevA24Free)(void *pBlock); //!< Release (free) block in A24 adress space.
        long (*pDevInit)(void); //!< Init devLib
	int (*pDevInterruptInUseVME) (unsigned vectorNumber); //!< Test if VME interrupt has an ISR connectd.
}devLibVME;

epicsShareExtern devLibVME *pdevLibVME;

#ifndef NO_DEVLIB_COMPAT
/**
 * @def pdevLibVirtualOS
 * @brief Alias to pdevLibVME
 */
#  define pdevLibVirtualOS pdevLibVME
/**
 * @var typedef devLibVME
 * @brief A type definition for devLibVME
 */
typedef devLibVME devLibVirtualOS;
#endif


#ifdef __cplusplus
}
#endif

#endif /* INCdevLibImplh */
