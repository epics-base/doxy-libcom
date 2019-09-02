/*************************************************************************\
* Copyright (c) 2010 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/**
 * @file postfix.h
 * @author Bob Dalesio
 * @date  21 Sep 1988
 *
 * @brief deﬁnes macros and routines used by the calculation record
 *
 * Deﬁnes several macros and the routines used by the calculation record
 * type calcRecord, access security, and other code, to compile and evaluate
 * mathematical expressions.
 */

#ifndef INCpostfixh
#define INCpostfixh

#include "shareLib.h"

#define CALCPERFORM_NARGS 12
#define CALCPERFORM_STACK 80

/**
 * @defgroup macroDefs
 * @{
 */
/**
 * @def INFIX_TO_POSTFIX_SIZE
 *
 * The above expression is an estimate of the maximum postfix buffer
 * size needed for a given infix expression buffer (the argument must count
 * the trailing nil byte in the input expression string). The actual size
 * needed is never larger than this value, although it is actually a
 * few bytes smaller for some sizes.
 *
 * The maximum expansion from infix to postfix is for the sub-expression
 *   - .1?.1:
 * which is 6 characters long and results in 21 bytes of postfix:
 *     - .1 => LITERAL_DOUBLE + 8 byte value
 *     - ?  => COND_IF
 *     - .1 => LITERAL_DOUBLE + 8 byte value
 *     - :  => COND_ELSE
 *     - ...
 *     -    => COND_END
 *     .
 * For other short expressions the factor 21/6 always gives a big enough
 * postfix buffer (proven by hand, look at '1+' and '.1+' as well).
 */
#define INFIX_TO_POSTFIX_SIZE(n) ((n)*21/6)

/**
 * @def MAX_INFIX_SIZE
 *
 * These are not hard limits, just default sizes for the database
 */
#define MAX_INFIX_SIZE 100
/**
 * @def MAX_POSTFIX_SIZE
 *
 * These are not hard limits, just default sizes for the database
 */
#define MAX_POSTFIX_SIZE INFIX_TO_POSTFIX_SIZE(MAX_INFIX_SIZE)


/** @def CALC_ERR_NONE 
 * No error
 */
#define CALC_ERR_NONE            0 
/** @def CALC_ERR_TOOMANY
 * Too many results returned 
 */
#define CALC_ERR_TOOMANY         1
/** @def CALC_ERR_BAD_LITERAL
 *  Bad numeric literal
 */
#define CALC_ERR_BAD_LITERAL     2
/** @def CALC_ERR_BAD_ASSIGNMENT
 * Bad assignment target
 */
#define CALC_ERR_BAD_ASSIGNMENT  3
/** @def CALC_ERR_BAD_SEPERATOR
 * Comma without parentheses
 */
#define CALC_ERR_BAD_SEPERATOR   4
/** @def CALC_ERR_PAREN_NOT_OPEN
 * Close parenthesis without open
 */
#define CALC_ERR_PAREN_NOT_OPEN  5
/** @def CALC_ERR_PAREN_OPEN
 * Open parenthesis at end of expression 
 */
#define CALC_ERR_PAREN_OPEN      6
/** @def CALC_ERR_CONDITIONAL
 * Unbalanced conditional ?: operators
 */
#define CALC_ERR_CONDITIONAL     7
/** @def CALC_ERR_INCOMPLETE
 * Incomplete expression, operand missing
 */
#define CALC_ERR_INCOMPLETE      8
/** @def CALC_ERR_UNDERFLOW
 * Runtime stack would underflow
 */
#define CALC_ERR_UNDERFLOW       9
/** @def CALC_ERR_OVERFLOW
 * Runtime stack would overflow
 */
#define CALC_ERR_OVERFLOW       10
/** @def CALC_ERR_SYNTAX
 * Syntax error
 */
#define CALC_ERR_SYNTAX         11
/** @def CALC_ERR_NULL_ARG
 * NULL or empty input argument
 */
#define CALC_ERR_NULL_ARG       12
/** @def CALC_ERR_INTERNAL
 * Internal error, bad element type
 */
#define CALC_ERR_INTERNAL       13

/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts an expression from inﬁx to postﬁx notation
 *
 * @param pinfix pointer to the infix string
 * @param ppostfix pointer to the postfix notation
 * @param perror place for an error code
 * @return non-zero value in case of error 
 *
 * It is the callers’s responsibility to ensure that ppostﬁx points 
 * to suﬃcient storage to hold the postﬁx expression. The macro 
 * INFIX_TO_POSTFIX_SIZE(n) can be used to calculate an appropriate
 * postﬁx buﬀer size from the length of the inﬁx buﬀer.
 *
 * @note "n" must count the terminating nil byte too.
 *
 * -# The **inﬁx expressions** that can be used are very similar
 * to the C expression syntax, but with some additions and subtle
 * diﬀerences in operator meaning and precedence. The string may
 * contain a series of expressions separated by a semi-colon character ‘;’
 * any one of which may actually provide the calculation result; however
 * all of the other expressions included must assign their result to
 * a variable. All alphabetic elements described below are case independent,
 * so upper and lower case letters may be used and mixed in the variable
 * and function names as desired. Spaces may be used anywhere within an
 * expression except between the characters that make up a single expression element.
 *
 * -# ***Numeric Literals***
 *  The simplest expression element is a numeric literal, any (positive)
 *  number expressed using the standard ﬂoating point syntax that can be stored
 *  as a double precision value. This now includes the values Infinity and
 *  NaN (not a number). Note that negative numbers will be encoded as a
 *  positive literal to which the unary negate operator is applied.
 *
 *  - Examples: 
 *    - 1
 *    - 2.718281828459
 *    - Inf
 *
 * -# ***Constants***
 *  There are three trigonometric constants available to any expression
 *  which return a value: 
 *    - pi returns the value of the mathematical constant π.
 *    - D2R evaluates to π/180 which, when used as a multiplier, 
 *    converts an angle from degrees to radians.
 *    - R2D evaluates to 180/π which as a multiplier converts an angle
 *    from radians to degrees.
 *
 * -# ***Variables***
 *  Variables are used to provide inputs to an expression, and are named
 *  using the single letters A through L inclusive or the keyword VAL which
 *  refers to the previous result of this calculation. The software that
 *  makes use of the expression evaluation code should document how the
 *  individual variables are given values; for the calc record type the input
 *  links INPA through INPL can be used to obtain these from other record ﬁelds,
 *  and VAL refers to the the VAL ﬁeld (which can be overwritten from outside
 *  the record via Channel Access or a database link).
 *
 * -# ***Variable Assignment Operator***
 *  Recently added is the ability to assign the result of a sub-expression to
 *  any of the single letter variables, which can then be used in another
 *  sub-expression. The variable assignment operator is the character pair
 *  := and must immediately follow the name of the variable to receive the
 *  expression value. Since the inﬁx string must return exactly one value, every
 *  expression string must have exactly one sub-expression that is not an
 *  assignment, which can appear anywhere in the string. Sub-expressions within
 *  the string are separated by a semi-colon character.
 *
 *    - Examples: 
 *      - B; B:=A
 *      - i:=i+1; a⋆sin(i⋆D2R)
 *
 * -# ***Arithmetic Operators***
 *  The usual binary arithmetic operators are provided: + - ⋆ and / with their
 *  usual relative precedence and left-to-right associativity, and - may also
 *  be used as a unary negate operator where it has a higher precedence and
 *  associates from right to left. There is no unary plus operator, so numeric
 *  literals cannot begin with a + sign.
 *
 *    - Examples: 
 *      - a⋆b + c
 *      - a/-4 - b
 *
 *  Three other binary operators are also provided: % is the integer modulo operator,
 *  while the synonymous operators ⋆⋆ and ̂ raise their left operand to the power of
 *  the right operand. % has the same precedence and associativity as ⋆ and /, while
 *  the power operators associate left-to-right and have a precedence in between ⋆ and
 *  unary minus.
 *
 *    - Examples: 
 *      - e:=a%10;
 *      - d:=a/10%10;
 *      - c:=a/100%10;
 *      - b:=a/1000%10;
 *      - b⋆4096+c⋆256+d⋆16+e
 *      - sqrt(a⋆⋆2 + b⋆⋆2)
 *     
 * -# ***Algebraic Functions***
 *  Various algebraic functions are available which take parameters inside
 *  parentheses. The parameter seperator is a comma.
 *
 *    - Absolute value: abs(a)
 *    - Exponential ea: exp(a)
 *    - Logarithm, base 10: log(a)
 *    - Natural logarithm (base e): ln(a) or loge(a)
 *    - n parameter maximum value: max(a, b, ...)
 *    - n parameter minimum value: min(a, b, ...)
 *    - Square root: sqr(a) or sqrt(a)
 *
 * -# ***Trigonometric Functions***
 *  Standard circular trigonometric functions, with angles expressed in radians:
 *    - Sine: sin(a)
 *    - Cosine: cos(a)
 *    - Tangent: tan(a)
 *    - Arcsine: asin(a)
 *    - Arccosine: acos(a)
 *    - Arctangent: atan(a)
 *    - 2 parameter arctangent: atan2(a, b)
 *  @note  Note that these arguments are the reverse of the ANSI C function,
 *  so while C would return arctan(a/b) the calc expression engine returns arctan(b/a)
 *
 * -# ***Hyperbolic Trigonometry***
 *  The basic hyperbolic functions are provided, but no inverse functions
 *  (which are not provided by the ANSI C math library either).
 *     - Hyperbolic sine: sinh(a)
 *     - Hyperbolic cosine: cosh(a)
 *     - Hyperbolic tangent: tanh(a)
 *
 * -# ***Numeric Functions***
 *  The numeric functions perform operations related to the ﬂoating point
 *  numeric representation and truncation or rounding.
 *    - Round up to next integer: ceil(a)
 *    - Round down to next integer: floor(a)
 *    - Round to nearest integer: nint(a)
 *    - Test for inﬁnite result: isinf(a)
 *    - Test for any non-numeric values: isnan(a, ...)
 *    - Test for all ﬁnite, numeric values: finite(a, ...)
 *    - Random number between 0 and 1: rndm
 *
 * -# ***Boolean Operators***
 *  These operators regard their arguments as true or false, where 0.0 is
 *  false and any other value is true.
 *
 *    - Boolean and: a && b
 *    - Boolean or: a || b
 *    - Boolean not: !a
 *
 * -# ***Bitwise Operators***
 * The bitwise operators convert their arguments to an integer (by truncation),
 * perform the appropriate bitwise operation and convert back to a ﬂoating point
 * value. Unlike in C though, ̂ is not a bitwise exclusive-or operator.
 *
 *    - Bitwise and: a & b or a and b
 *    - Bitwise or: a | b or a or b
 *    - Bitwise exclusive or: a xor b
 *    - Bitwise not (ones complement): ~a or not a
 *    - Bitwise left shift: a << b
 *    - Bitwise right shift: a >> b
 *
 * -# ***Relational Operators***
 *  Standard numeric comparisons between two values:
 *
 *    - Less than: a < b
 *    - Less than or equal to: a <= b
 *    - Equal to: a = b or a == b
 *    - Greater than or equal to: a >= b
 *    - Greater than: a > b
 *    - Not equal to: a != b or a # b
 *
 * -# ***Conditional Operator***
 *  Expressions can use the C conditional operator, which has a lower
 *  precedence than all of the other operators except for the assignment operator.
 *
 *    - condition ? true result : false result
 *      - Example:
 *        - a < 360 ? a+1 : 0
 *        
 * -# ***Parentheses***
 * Sub-expressions can be placed within parentheses to override operator precence rules.
 * Parentheses can be nested to any depth, but the intermediate value stack used by
 * the expression evaluation engine is limited to 80 results (which require an
 * expression at least 321 characters long to reach).
 */
epicsShareFunc long
    postfix(const char *pinfix, char *ppostfix, short *perror);

/**
 * Evaluates the postfix expression
 *
 * @param parg Pointer to an array of double values for the arguments A-L
 * that can appear in the expression. Note that the argument values may be
 * modiﬁed if the expression uses the assignment operator.
 * @param presult Where to put the calculated result, which may be a NaN or Inﬁnity.
 * @param ppostfix The postﬁx expression created by postﬁx().
 * @return Status value 0 for OK, or non-zero if an error is discovered
 * during the evaluation process.
 */
epicsShareFunc long
    calcPerform(double *parg, double *presult, const char *ppostfix);

/**
 * Discover used or modified expression arguments
 *
 * Software using the calc subsystem may need to know what expression arguments are
 * used and/or modiﬁed by a particular expression. It can discover this from the
 * postﬁx string by calling calcArgUsage(), which takes two pointers pinputs
 * and pstores to a pair of unsigned long bitmaps which return that information
 * to the caller. Passing a NULL value for either of these pointers is legal
 * if only the other is needed.
 * @param ppostfix The postﬁx expression created by postﬁx().
 * @param pinputs Bitmap pointer
 * The least signﬁcant bit (bit 0) of the bitmap at *pinputs will be set if the
 * expression depends on the argument A, and so on through bit 11 for the argument L.
 * An argument that is not used until after a value has been assigned
 * to it will not be set in the pinputs bitmap, thus the bits can be used to determine
 * whether a value needs to be supplied for their associated argument or not for the
 * purposes of evaluating the expression.
 * @param pstores Bitmap pointer.
 * Bit 0 of the bitmap at *pstores will be set if the expression assigns a value to
 * the argument A. 
 * @return The return value will be non-zero if the ppostﬁx expression was
 * illegal, otherwise 0.
 */
epicsShareFunc long
    calcArgUsage(const char *ppostfix, unsigned long *pinputs, unsigned long *pstores);

/**
 * Gives back a string representation of the error codes
 * @param error error code
 * @return  string representation of the error code
 *
 * The error codes deﬁned above as a series of macros with names starting CALC_ERR_.
 * @note Changes in the errors must also be made in calcErrorStr()
 */
epicsShareFunc const char *
    calcErrorStr(short error);

/**
 * Disassemble the given postfix instructions to stdout
 * @param pinst postfix instructions
 *
 */
epicsShareFunc void
    calcExprDump(const char *pinst);

#ifdef __cplusplus
}
#endif

#endif /* INCpostfixh */
