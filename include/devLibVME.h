/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file devLibVME.h
 * @brief API for VMEbus devices, includes some general APIs for all bus
 * types.
 * @author Marty Kraimer, Jeff Hill
 * @date    03-10-93
 * 
 * @note hpj: Why is it devLibVME.h? It includes API for generic and ISA type
 * buses
 *
 * API for VMEbus (and some general APIs for all bus types).
 *
 */

#ifndef INCdevLibh
#define INCdevLibh 1

#include "dbDefs.h"
#include "osdVME.h"
#include "errMdef.h"
#include "shareLib.h"
#include "devLib.h"

#ifdef __cplusplus
extern "C" {
#endif

//! @enum epicsAddressType Defines VMEbus address types.
//! @note devLib.c must change in unison.
typedef enum {
		atVMEA16, //!< VME short I/O.
		atVMEA24, //!< VME standard I/O.
		atVMEA32, //!< VME extended I/O.
		atISA,	//!< Memory mapped ISA access (until now only on PC).
		atVMECSR, //!< VME-64 CR/CSR address space.
		atLast	//!< atLast must be the last enum in this list.
		} epicsAddressType;

/**
 * @var epicsAddressTypeName
 * @brief Pointer to an array of strings for each of the address types defined
 * 	  in epicsAddressType.
 */
epicsShareExtern const char *epicsAddressTypeName[];

#ifdef __cplusplus
}
#endif

/*
 * To retain compatibility include everything by default
 */
#ifndef NO_DEVLIB_COMPAT
#  include  "devLibVMEImpl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Print an address map.
 * @return Success(0) or error if devLibInit() fail. 
 * @note Applies to all bus types.
 */
epicsShareFunc long devAddressMap(void);

/**
 * Translate bus addresses their local CPU address mapping.
 * @param addrType Defines address space.
 * @param busAddr Bus address to be translated.
 * @param *ppLocalAddr Set equal to the address seen by CPU.
 * @return Success(0) , error if devLibInit() fail, error on invalid bus
 * 		address, ... .
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devBusToLocalAddr (
		epicsAddressType addrType,
		size_t busAddr,
		volatile void **ppLocalAddr);
/**
 * A bus error safe "wordSize" read at the specified address.
 * @param wordSize Defines word size to read.
 * @param ptr Pointer to address to read.
 * @param pValueRead value read.
 * @return Unsuccessful status if the address could'nt read or device not present.
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devReadProbe (
    unsigned wordSize, volatile const void *ptr, void *pValueRead);

/**
 * Verifies that no devices respond at naturally aligned words
 * within the specified address range. Checks all naturally aligned
 * word sizes between char and long for the entire specified range of bytes.
 * @param addrType Defines address space.
 * @param base Address base where to start.
 * @param size range to be test.
 * @return Return success if no devices respond. Returns an error if
 *         a device does respond or if a physical address for a 
 *         naturally aligned word can't be mapped.
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devNoResponseProbe(
			epicsAddressType addrType,
			size_t base,
			size_t size
);

/**
 * A bus error safe "wordSize" write  at the specified address.
 * @param wordSize Defines word size to write.
 * @param ptr Pointer to address to write to.
 * @param pValueWritten Value to write.
 * @return Unsuccessful status if to the address could'nt write to or device not present.
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devWriteProbe (
    unsigned wordSize, volatile void *ptr, const void *pValueWritten);

/**
 * Register a (actual VME-) address. It keeps a list of all addresses
 * registered.
 * @param pOwnerName String of owner name.
 * @param addrType Defines address space.
 * @param logicalBaseAddress The address (physical?) on the bus.
 * @param size Range (bytes) to be registered.
 * @param *pPhysicalAddress Is set equal to the address as seen by the caller.
 * @return Returns an error if an attempt is made to register any addresses
 * that are already being used.
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devRegisterAddress(
			const char *pOwnerName,
			epicsAddressType addrType,
			size_t logicalBaseAddress,
			size_t size, 
			volatile void **pPhysicalAddress);

/**
 * Release addresses previously registeres by a call to devRegisterAddress.
 * @param addrType Defines address space.
 * @param logicalBaseAddress The address (physical?) on the bus.
 * @param pOwnerName String of owner name.
 * @return Returns an error if an attempt is made to unregister any unknown addresses
 * or unknown owner name.
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devUnregisterAddress(
			epicsAddressType addrType,
			size_t logicalBaseAddress,
			const char *pOwnerName);

/**
 * Allocate and register an unoccupied address block.
 * @param pOwnerName String of owner name.
 * @param addrType Defines address space.
 * @param size Range (bytes) to be allocated.
 * @param alignment Nr. of least significant bits zero in address.
 * @param *pLocalAddress Set equal to the address seen by CPU.
 * @return Returns an error if ??
 * @note Applies to all bus types (OSI routine).
 */
epicsShareFunc long devAllocAddress(
			const char *pOwnerName,
			epicsAddressType addrType,
			size_t size,
			unsigned alignment, /*n ls bits zero in addr*/
			volatile void **pLocalAddress);

/**
 * Connect ISR to a VME interrupt vector.
 * @param vectorNumber Interrupt vector number on the bus.
 * @param pFunction C function pointer to connect to.
 * @param parameter Parameter to the ISR.
 * @return Returns success or error.
 * @note Function apply only to the VME bus type.
 */
epicsShareFunc long devConnectInterruptVME(
			unsigned vectorNumber,
			void (*pFunction)(void *),
			void  *parameter);

/**
 * Disconnect ISR from a VME interrupt vector.
 * @param vectorNumber Interrupt vector number.
 * @param pFunction The parameter should be set to the C function pointer that 
 * was connected. It is used as a key to prevent a driver from inadvertently
 * removing an interrupt handler that it didn't install.
 * @return returns success or error.
 * @note Function apply only to the VME bus type.
 */
epicsShareFunc long devDisconnectInterruptVME(
			unsigned vectorNumber,
			void (*pFunction)(void *));

/**
 * Determine if a VME interrupt vector is in use.
 * @param vectorNumber Interrupt vector number.
 * @return True/false.
 * @note Function apply only to the VME bus type
 */
epicsShareFunc int devInterruptInUseVME (unsigned vectorNumber);

/**
 * Enable VME interrupt level.
 * @param level Interrupt level on the VMEbus.
 * @return Success or error, e.g. if VMEInit fail.
 * @note Function apply only to the VME bus type
 */
epicsShareFunc long devEnableInterruptLevelVME (unsigned level);

/**
 * Disable VME interrupt level.
 * @param level Interrupt level on the VMEbus.
 * @return Success or error, e.g. if VMEInit fail.
 * @note Function apply only to the VME bus type
 */
epicsShareFunc long devDisableInterruptLevelVME (unsigned level);

/**
 * Malloc like allocation routine for VME A24 memory region
 * @param size Size (bytes?) to allocate
 * @note Function apply only to the VME bus type
 */
epicsShareFunc void *devLibA24Malloc(size_t size);
/**
 * Calloc like allocation routine for VME A24 memory region
 * @note But calls devLibA24Malloc ???
 * @param size Size (bytes?) to allocate
 * @note Function apply only to the VME bus type
 */
epicsShareFunc void *devLibA24Calloc(size_t size);
/**
 * Free VME A24 memory region
 * @param pBlock Block to be released (free)
 * @note Function apply only to the VME bus type
 */
epicsShareFunc void devLibA24Free(void *pBlock);

/**
 * Connect ISR to a ISA interrupt.
 * @note !Not implemented!.
 * @param interruptLevel Bus interrupt level to connect to.
 * @param pFunction C function pointer to connect to.
 * @param parameter Parameter to the called function.
 * @return Returns success or error.
 * @note Function apply only to the ISA bus type.
 * @note API should be reviewed.
 */
epicsShareFunc long devConnectInterruptISA(
			unsigned interruptLevel,
			void (*pFunction)(void *),
			void  *parameter);

/**
 * Disconnect ISR from a ISA interrupt level.
 * @note !Not implemented!.
 * @param interruptLevel Interrupt level.
 * @param pFunction The parameter should be set to the C function pointer that 
 * was connected. It is used as a key to prevent a driver from inadvertently
 * removing an interrupt handler that it didn't install.
 * @return returns success or error.
 * @note Function apply only to the ISA bus type.
 * @note API should be reviewed.
 */
epicsShareFunc long devDisconnectInterruptISA(
			unsigned interruptLevel,
			void (*pFunction)(void *));

/**
 * Determine if an ISA interrupt level is in use
 * @note !Not implemented!.
 * @param interruptLevel Interrupt level.
 * @return Returns True/False.
 * @note Function apply only to the ISA bus type.
 */
epicsShareFunc int devInterruptLevelInUseISA (unsigned interruptLevel);

/**
 * Enable ISA interrupt level
 * @param level Interrupt level.
 * @return Returns True/False.
 * @note Function apply only to the ISA bus type.
 */
epicsShareFunc long devEnableInterruptLevelISA (unsigned level);

/**
 * Disable ISA interrupt level
 * @param level Interrupt level.
 * @return Returns True/False.
 * @note Function apply only to the ISA bus type.
 */
epicsShareFunc long devDisableInterruptLevelISA (unsigned level);

#ifndef NO_DEVLIB_OLD_INTERFACE

/**
 * @note Deprecated interface
 */
typedef enum {intVME, intVXI, intISA} epicsInterruptType;

/**
 * @note This routine has been deprecated. It exists
 * for backwards compatibility purposes only.
 * Please use one of devConnectInterruptVME, devConnectInterruptPCI,
 * devConnectInterruptISA etc. devConnectInterrupt will be removed 
 * in a future release.
 */
epicsShareFunc long devConnectInterrupt(
			epicsInterruptType intType,
			unsigned vectorNumber,
			void (*pFunction)(void *),
			void  *parameter);

/**
 * @note This routine has been deprecated. It exists
 * for backwards compatibility purposes only.
 * Please use one of devDisconnectInterruptVME, devDisconnectInterruptPCI,
 * devDisconnectInterruptISA etc. devDisconnectInterrupt will be removed 
 * in a future release.
 */
epicsShareFunc long devDisconnectInterrupt(
			epicsInterruptType      intType,
			unsigned                vectorNumber,
			void		        (*pFunction)(void *));

/**
 * @note This routine has been deprecated. It exists
 * for backwards compatibility purposes only.
 * Please use one of devEnableInterruptLevelVME, devEnableInterruptLevelPCI,
 * devEnableInterruptLevelISA etc. devEnableInterruptLevel will be removed 
 * in a future release.
 */
epicsShareFunc long devEnableInterruptLevel(
    epicsInterruptType intType, unsigned level);

/**
 * @note This routine has been deprecated. It exists
 * for backwards compatibility purposes only.
 * Please use one of devDisableInterruptLevelVME, devDisableInterruptLevelISA,
 * devDisableInterruptLevelPCI etc. devDisableInterruptLevel will be removed 
 * in a future release.
 */
epicsShareFunc long devDisableInterruptLevel (
    epicsInterruptType intType, unsigned level);

/**
 * @note This routine has been deprecated. It exists
 * for backwards compatibility purposes only.
 * Please use devNoResponseProbe(). locationProbe() will be removed 
 * in a future release.
 */
epicsShareFunc long locationProbe (epicsAddressType addrType, char *pLocation);

#endif /* NO_DEVLIB_OLD_INTERFACE */

#ifdef __cplusplus
}
#endif

#endif  /* INCdevLibh.h*/
