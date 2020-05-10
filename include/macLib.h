/*************************************************************************\
* Copyright (c) 2007 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/**
 * @file macLib.h
 * @author William Lupton, W. M. Keck Observatory
 *
 * Describes a general purpose macro substitution library. 
 * It is used for all macro substitution in base.
 *
 * @note The directory \<base\>/src/libCom/macLib contains two ﬁles 
 * macLibNOTES and macLibREADME that explain this library.
 *
 * @note Error handling on routines conforms 0 (=OK) for success,
 * -1 (=ERROR) for failure, and small positive values for extra info.
 *
 * "authors note": I contravened this for macGetValue() and macExpandString()
 * because I felt that it was worth returning information both on
 * success / failure and on value length. Errors and warnings are 
 * reported using errlogPrintf().
 */

#ifndef INCmacLibH
#define INCmacLibH

#include "ellLib.h"
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
 * @defgroup macLibDefs macLib Macros
 * @{
 */
/**
 * @def MAC_SIZE 
 * @brief Maximum size of macro name or value string (simpler to make fixed)
 */
#define MAC_SIZE 256
/** @} */

/*
 * Macro substitution context. One of these contexts is allocated each time
 * macCreateHandle() is called
 */
 /** @struct MAC_HANDLE
 *  @brief Macro substitution context
 *  One of these contexts is allocated each time macCreateHandle() is called
 */
typedef struct {
    long        magic;          /**< @brief magic number (used for authentication) */
    int         dirty;          /**< @brief values need expanding from raw values? */
    int         level;          /**< @brief scoping level */
    int         debug;          /**< @brief debugging level */
    ELLLIST     list;           /**< @brief macro name / value list */
    int         flags;          /**< @brief operating mode flags */
} MAC_HANDLE;

/**
 * @brief Creates a new macro substitution context
 * @return 0 = OK; <0 = ERROR
 */
epicsShareFunc long
epicsShareAPI macCreateHandle(
    MAC_HANDLE  **handle,       /**< address of variable to receive pointer
                                  to new macro substitution context */

    const char * pairs[]        /**< pointer to NULL-terminated array of
                                  {name,value} pair strings; a NULL
                                  value implies undefined; a NULL
                                  argument implies no macros */
);
/**
 * @brief Can be called to suppress the marning message
 * 
 * when macExpandString cant expand a macro. A non zero value will
 * suppress the messages.
 */
epicsShareFunc void
epicsShareAPI macSuppressWarning(
    MAC_HANDLE  *handle,        /**< opaque handle */

    int         falseTrue       /**< 0 means issue, 1 means suppress*/
);

/**
 * @brief Operates on a string which may contain macro references
 * @return Returns the length of the destination string, <0 if any macro are
 * undefined
 *
 * This operates on a string which may contain macro references. It
 * parses the string looking for such references and passes them to
 * macGetValue() for translation.
 *
 * @note The function value is similar to that of macGetValue(). Its absolute
 * value is the number of characters copied. If negative, at least one
 * undefined macro has been left unexpanded.
 */
epicsShareFunc long
epicsShareAPI macExpandString(
    MAC_HANDLE  *handle,        /**< opaque handle */

    const char  *src,           /**< source string */

    char        *dest,          /**< destination string */

    long        capacity        /**< capacity of destination buffer (dest) */
);

/**
 * @brief Sets the value of a macro
 * @return Returns the length of the value string
 * @note If "value" is NULL, it undefines
 * all instances of "name" at all scoping levels (it's not an error 
 * if "name" is not defined in this case). Macros referenced by
 * "value" need not be defined at this point.
 */
epicsShareFunc long
epicsShareAPI macPutValue(
    MAC_HANDLE  *handle,        /**< opaque handle */

    const char  *name,          /**< macro name */

    const char  *value          /**< macro value */
);

/**
 * @brief Returns the value of a macro
 * @return Returns the length of the value string, <0 if undefined
 *
 * "value" will be zero-terminated if the length of the value is less than
 * maxlen. The function value will be the number of characters copied to
 * "value" (see below for behavior if the macro is undefined). If maxlen
 * is zero, no characters will be copied to "value" (which can be NULL)
 * and the call can be used to check whether the macro is defined.
 *
 * @note Truncation of the value is not reported. Is this a problem?
 *
 * If the macro is undefined, the macro reference will be returned in
 * the value string (if permitted by maxlen) and the function value will
 * be _minus_ the number of characters copied. Note that treatment of
 * maxlen is intended to be consistent with what people are used to with
 * strncpy().
 *
 * If the value contains macro references then the references will be
 * expanded recursively. This expansion will detect a direct or indirect
 * self reference.
 *
 * Macro references begin with a "$" immediately followed by either a
 * "(" or a "{" character. The macro name comes next, and may optionally
 * be succeeded by an "=" and a default value, which will be returned if
 * the named macro is undefined at the moment of expansion. The reference
 * is terminated by the matching ")" or "}" character.
 */
epicsShareFunc long
epicsShareAPI macGetValue(
    MAC_HANDLE  *handle,        /**< opaque handle */

    const char  *name,          /**< macro name or reference */

    char        *value,         /**< string to receive macro value or name
                                argument if macro is undefined */

    long        capacity        /**< capacity of destination buffer (value) */
);
/**
 * @brief Marks a handle invalid, and frees all storage associated with it
 * @return 0 = OK; <0 = ERROR
 * @note Note that this does not free any strings into which macro values have
 *  been returned. Macro values are always returned into strings which
 *  were pre-allocated by the caller.
 */
epicsShareFunc long
epicsShareAPI macDeleteHandle(
    MAC_HANDLE  *handle         /**< opaque handle */
);
/**
 * @brief Marks the start of a new scoping level
 * @return 0 = OK; <0 = ERROR 
 * Marks such that all definitions made
 * up until the next macPopScope() call will be lost on macPopScope()
 * and those current at macPushScope() will be re-instated.
 */
epicsShareFunc long
epicsShareAPI macPushScope(
    MAC_HANDLE  *handle         /**< opaque handle */
);
/**
 * @brief Retrieve the last pushed scope (like stack operations)
 * @return 0 = OK; <0 = ERROR 
 * see macPushScope()
 */
epicsShareFunc long 
epicsShareAPI macPopScope(
    MAC_HANDLE  *handle         /**< opaque handle */
);
/**
 * @brief Reports details of current definitions
 * @return 0 = OK; <0 = ERROR  
 * This reports details of current definitions to standard output,
 * and is intended purely for debugging purposes.
 */
epicsShareFunc long
epicsShareAPI macReportMacros(
    MAC_HANDLE  *handle         /**< opaque handle */
);

/**
 * @brief Converts macro definitions into an array of pointers
 * @return nr of defns encountered; <0 = ERROR
 *
 * This takes a set of macro definitions in "a=xxx,b=yyy" format and
 * converts them into an array of pointers to character strings which
 * are, in order, "first name", "first value", "second name", "second
 * value" etc. The array is terminated with two NULL pointers and all
 * storage is allocated contiguously so that it can be freed with a
 * single call to free().

 * This routine is independent of any handle and provides a generally
 * useful service which may be used elsewhere. Any macro references in
 * values are not expanded at this point since the referenced macros may
 * be defined or redefined before the macro actually has to be
 * translated.
 *
 * Shell-style escapes and quotes are supported, as are things like
 * "A=B,B=$(C$(A)),CA=CA,CB=CB" (sets B to "CB"). White space is
 * significant within values but ignored elsewhere (i.e. surrounding "="
 * and "," characters).
 *
 * Probably noone will ever want to, but the special meanings of "$",
 * "{", "}", "(", ")", "=" and "," can all be changed via macPutXxxx()
 * calls.  This routine does not have a handle argument, so they must be
 * changed globally for it to use the new definitions. Should it have a
 * handle argument? This makes it more of a pain to use but guarantees
 * that there will be no conflicts. I think it should really.
 *
 * The function value is the number of definitions encountered, or -1 if
 * the supplied string is invalid.
 */
epicsShareFunc long
epicsShareAPI macParseDefns(
    MAC_HANDLE  *handle,        /**< opaque handle; can be NULL if default
                                special characters are to be used */

    const char  *defns,         /**< macro definitions in "a=xxx,b=yyy"
                                format */

    char        **pairs[]       /**< address of variable to receive pointer
                                to NULL-terminated array of {name,
                                value} pair strings; all storage is
                                allocated contiguously */
);

/**
 * @brief Installalls pairs as definitions
 * @return nr of marcros defined; <0 = ERROR 
 *
 * This takes an array of pairs as defined above and installs them as
 * definitions by calling macPutValue(). The pairs array is terminated
 * by a NULL pointer.
 */
epicsShareFunc long
epicsShareAPI macInstallMacros(
    MAC_HANDLE  *handle,        /**< opaque handle */

    char        *pairs[]        /**< pointer to NULL-terminated array of
                                {name,value} pair strings; a NULL
                                value implies undefined; a NULL
                                argument implies no macros */
);

/**
 * @brief Parse macro strings and passes them to macGetValue
 * @return expanded string; NULL if any undefined macros
 *
 * This operates on a string which may contain macros defined by
 * environment variables. It parses the string looking for such
 * references and passes them to macGetValue() for translation. It uses
 * malloc() to allocate space for the expanded string and returns a
 * pointer to this null-terminated string. It returns NULL if the source
 * string contains any undefined references.
 */
epicsShareFunc char *
epicsShareAPI macEnvExpand(
    const char *str             /**< string to be expanded */
);

/**
 * @brief Expands set of macro definitions
 * @return expanded string; NULL if any undefined macros
 *
 * This operates in the same manner as macEnvExpand, but takes an
 * optional macro handle that can contain a set of macro definitions.
 * These macros are appended to the set of macros from environment
 * variables when expanding the string.
 */
epicsShareFunc char *
epicsShareAPI macDefExpand(
    const char *str,            /**< string to be expanded */
    MAC_HANDLE *macros          /**< opaque handle; can be NULL if default
                                special characters are to be used */
);

#ifdef __cplusplus
}
#endif

#endif /*INCmacLibH*/
