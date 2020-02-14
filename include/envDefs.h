/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/**
 * @file envDefs.h
 * @author Roger A. Cole
 * @date   07-20-91
 *
 * @brief Definitions for environment get/set routines.
 *
 * This file defines the environment parameters for EPICS. These 
 * ENV_PARAM's are created in envData.c by bldEnvData for use by
 * EPICS programs running under UNIX and VxWorks.
 * User programs can define their own environment parameters for their
 * own use--the only caveat is that such parameters aren't automatically
 * setup by EPICS.
 *
 * @note bldEnvData.pl looks for "epicsShareExtern const ENV_PARAM <name>;"
 */
/****************************************************************************

#ifndef envDefsH
#define envDefsH

#ifdef __cplusplus
extern "C" {
#endif

#include "shareLib.h"

/**
 * @struct ENV_PARAM
 * @brief Defines parameter structure
 */
typedef struct envParam {
    char	*name;	  /**< @brief text name of the parameter */
    char	*pdflt;
} ENV_PARAM;

epicsShareExtern const ENV_PARAM EPICS_CA_ADDR_LIST;
epicsShareExtern const ENV_PARAM EPICS_CA_CONN_TMO;
epicsShareExtern const ENV_PARAM EPICS_CA_AUTO_ADDR_LIST;
epicsShareExtern const ENV_PARAM EPICS_CA_REPEATER_PORT;
epicsShareExtern const ENV_PARAM EPICS_CA_SERVER_PORT;
epicsShareExtern const ENV_PARAM EPICS_CA_MAX_ARRAY_BYTES;
epicsShareExtern const ENV_PARAM EPICS_CA_AUTO_ARRAY_BYTES;
epicsShareExtern const ENV_PARAM EPICS_CA_MAX_SEARCH_PERIOD;
epicsShareExtern const ENV_PARAM EPICS_CA_NAME_SERVERS;
epicsShareExtern const ENV_PARAM EPICS_CA_MCAST_TTL;
epicsShareExtern const ENV_PARAM EPICS_CAS_INTF_ADDR_LIST;
epicsShareExtern const ENV_PARAM EPICS_CAS_IGNORE_ADDR_LIST;
epicsShareExtern const ENV_PARAM EPICS_CAS_AUTO_BEACON_ADDR_LIST;
epicsShareExtern const ENV_PARAM EPICS_CAS_BEACON_ADDR_LIST;
epicsShareExtern const ENV_PARAM EPICS_CAS_SERVER_PORT;
epicsShareExtern const ENV_PARAM EPICS_CA_BEACON_PERIOD; /**< @brief deprecated */
epicsShareExtern const ENV_PARAM EPICS_CAS_BEACON_PERIOD;
epicsShareExtern const ENV_PARAM EPICS_CAS_BEACON_PORT;
epicsShareExtern const ENV_PARAM EPICS_BUILD_COMPILER_CLASS;
epicsShareExtern const ENV_PARAM EPICS_BUILD_OS_CLASS;
epicsShareExtern const ENV_PARAM EPICS_BUILD_TARGET_ARCH;
epicsShareExtern const ENV_PARAM EPICS_TIMEZONE;
epicsShareExtern const ENV_PARAM EPICS_TS_NTP_INET;
epicsShareExtern const ENV_PARAM EPICS_IOC_IGNORE_SERVERS;
epicsShareExtern const ENV_PARAM EPICS_IOC_LOG_PORT;
epicsShareExtern const ENV_PARAM EPICS_IOC_LOG_INET;
epicsShareExtern const ENV_PARAM EPICS_IOC_LOG_FILE_LIMIT;
epicsShareExtern const ENV_PARAM EPICS_IOC_LOG_FILE_NAME;
epicsShareExtern const ENV_PARAM EPICS_IOC_LOG_FILE_COMMAND;
epicsShareExtern const ENV_PARAM EPICS_CMD_PROTO_PORT;
epicsShareExtern const ENV_PARAM EPICS_AR_PORT;
epicsShareExtern const ENV_PARAM IOCSH_PS1;
epicsShareExtern const ENV_PARAM IOCSH_HISTSIZE;
epicsShareExtern const ENV_PARAM IOCSH_HISTEDIT_DISABLE;
epicsShareExtern const ENV_PARAM *env_param_list[];

struct in_addr;

/**
 * @brief Get value of a configuration parameter 
 * 
 * Gets the value of a configuration parameter and copies it
 * into the caller's buffer.  If the configuration parameter
 * isn't found in the environment, then the default value for
 * the parameter is copied.  If no parameter is found and there
 * is no default, then '\0' is copied and NULL is returned.
 *
 * @param pParam Pointer to config param structure.
 * @param bufDim Dimension of parameter buffer
 * @param pBuf Pointer to parameter buffer
 * @return Pointer to the environment variable value string, 
 * or NULL if no parameter value and no default value was found.
 */
epicsShareFunc char * epicsShareAPI 
	envGetConfigParam(const ENV_PARAM *pParam, int bufDim, char *pBuf);

/**
 * @brief Returns a pointer to the configuration parameter value string  
 * 
 * @param pParam Pointer to config param structure.
 * @return Pointer to the environment variable value string, 
 * or NULL if no parameter value and no default value was found.
 */
epicsShareFunc const char * epicsShareAPI 
	envGetConfigParamPtr(const ENV_PARAM *pParam);

/**
 * @brief Print value of a configuration parameter.  
 * 
 * @param pParam Pointer to config param structure.
 * @return 0 
 */
epicsShareFunc long epicsShareAPI 
	envPrtConfigParam(const ENV_PARAM *pParam);

/**
 * @brief Get value of an inet addr config parameter.  
 *
 * Gets the value of a configuration parameter and copies it into
 * the caller's (struct in_addr) buffer. If the configuration parameter
 * isn't found in the environment, then the default value for
 * the parameter is copied. 
 *
 * If no parameter is found and there is no default, then -1 is
 * returned and the callers buffer is unmodified.
 * 
 * @param pParam Pointer to config param structure.
 * @param pAddr Pointer to struct to receive inet addr. 
 * @return 0, or -1 if an error is encountered
 */
epicsShareFunc long epicsShareAPI 
	envGetInetAddrConfigParam(const ENV_PARAM *pParam, struct in_addr *pAddr);

/**
 * @brief Get value of a double configuration parameter.
 *
 * Gets the value of a configuration parameter and copies it into
 * the caller's real (double) buffer. If the configuration parameter
 * isn't found in the environment, then the default value for
 * the parameter is copied. 
 *
 * If no parameter is found and there is no default, then -1 is
 * returned and the callers buffer is unmodified.
 * 
 * @param pParam Pointer to config param structure.
 * @param pDouble Pointer to place to store value.
 * @return 0, or -1 if an error is encountered
 */
epicsShareFunc long epicsShareAPI 
	envGetDoubleConfigParam(const ENV_PARAM *pParam, double *pDouble);

/**
 * @brief Get value of a long configuration parameter.
 *
 * Gets the value of a configuration parameter and copies it into
 * the caller's real (long) buffer. If the configuration parameter
 * isn't found in the environment, then the default value for
 * the parameter is copied. 
 *
 * If no parameter is found and there is no default, then -1 is
 * returned and the callers buffer is unmodified.
 * 
 * @param pParam Pointer to config param structure.
 * @param pLong Pointer to place to store value.
 * @return 0, or -1 if an error is encountered
 */
epicsShareFunc long epicsShareAPI 
	envGetLongConfigParam(const ENV_PARAM *pParam, long *pLong);

/**
 * @brief Get environment port number.
 *
 * @param pEnv Pointer to config param structure.
 * @param defaultPort Default Port to be used if environment settings invalid.
 * @return PortNumber.
 */
epicsShareFunc unsigned short epicsShareAPI envGetInetPortConfigParam 
        (const ENV_PARAM *pEnv, unsigned short defaultPort);
/**
 * @brief Get value of a boolean configuration parameter.
 *
 * Gets the value of a configuration parameter and copies it into
 * the caller's real (boolean) buffer. If the configuration parameter
 * isn't found in the environment, then the default value for
 * the parameter is copied. 
 *
 * If no parameter is found and there is no default, then -1 is
 * returned and the callers buffer is unmodified.
 * 
 * @param pParam Pointer to config param structure.
 * @param pBool Pointer to place to store value.
 * @return 0, or -1 if an error is encountered
 */
epicsShareFunc long epicsShareAPI
    envGetBoolConfigParam(const ENV_PARAM *pParam, int *pBool);

/**
 * @brief Prints all configuration parameters and their current value.
 *
 * @return 0
 */
epicsShareFunc long epicsShareAPI epicsPrtEnvParams(void);

epicsShareFunc void epicsShareAPI epicsEnvSet (const char *name, const char *value);
epicsShareFunc void epicsShareAPI epicsEnvUnset (const char *name);
epicsShareFunc void epicsShareAPI epicsEnvShow (const char *name);

#ifdef __cplusplus
}
#endif

#endif /*envDefsH*/

