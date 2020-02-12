/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/**
 * @file epicsUnitTest.h
 * @brief Unit test routines
 * @author Andrew Johnson
 *
 * The unit test routines make it easy for a test program to generate output 
 * that is compatible with the Test Anything Protocol and can thus be used with
 * Perl’s automated Test::Harness as well as generating human-readable output. 
 * The routines detect whether they are being run automatically and print a 
 * summary of the results at the end if not.
@code
void testPlan(int tests); 
int  testOk(int pass, const char ⋆fmt, ...); 
#define testOk1(cond) testOk(cond, "%s", #cond) 
void testPass(const char ⋆fmt, ...); 
void testFail(const char ⋆fmt, ...); 
int  testOkV(int pass, const char ⋆fmt, va_list pvar); 
void testSkip(int skip, const char ⋆why) 
void testTodoBegin(const char ⋆why); 
void testTodoEnd(); 
int  testDiag(const char ⋆fmt, ...); 
void testAbort(const char ⋆fmt, ...); 
int  testDone(void); 

 
typedef int (⋆TESTFUNC)(void); 
epicsShareFunc void testHarness(void); 
epicsShareFunc void runTestFunc(const char ⋆name, TESTFUNC func); 
 
#define runTest(func) runTestFunc(#func, func)
@endcode

 * A test program starts with a call to testPlan(), announcing how many tests
 * are to be conducted. If this number is not known a value of zero can be 
 * used during development, but it is recommended that the correct value be 
 * substituted after the test program has been completed.
 * 
 * Individual test results are reported using any of testOk(), testOk1(), 
 * testOkV(), testPass() or testFail(). The testOk() call takes and also 
 * returns a logical pass/fail result (zero means failure, any other value 
 * is success) and a printf-like format string and arguments which describe 
 * the test. The convenience macro testOk1() is provided which stringiﬁes its 
 * single condition argument, reducing the eﬀort needed when writing test 
 * programs. The individual testPass() and testFail() routines can be used when
 * the test program takes a diﬀerent path on success than on failure, but one 
 * or other must always be called for any particular test. The testOkV() routine
 * is a varargs form of testOk() included for internal purposes which may prove 
 * useful in some cases.
 * 
 * If some program condition or failure makes it impossible to run some tests, 
 * the testSkip() routine can be used to indicate how many tests are being omitted 
 * from the run, thus keeping the test counts correct; the constant string why is 
 * displayed as an explanation to the user (this string is not printf-like).
 * 
 * If some tests are expected to fail because functionality in the module under 
 * test has not yet been fully implemented, these tests may still be executed, 
 * wrapped between calls to testTodoBegin() and testTodoEnd(). testTodoBegin() 
 * takes a constant string indicating why these tests are not expected to 
 * succeed. This modiﬁes the counting of the results so the wrapped tests will not
 * be recorded as failures.
 * 
 * Additional information can be supplied using the testDiag() routine, which 
 * displays the relevent information as a comment in the result output. None of 
 * the printable strings passed to any testXxx() routine should contain a newline
 *  ‘∖n’ character, newlines will be added by the test routines as part of the 
 * Test Anything Protocol. For multiple lines of diagnostic output, call 
 * testDiag() as many times as necessary.
 * 
 * If at any time the test program is unable to continue for some catastrophic 
 * reason, calling testAbort() with an appropriate message will ensure that the 
 * test harness understands this. testAbort() does not return, but calls the ANSI
 *  C routine abort() to cause the program to stop immediately.
 * 
 * After all of the tests have been completed, the return value from 
 * testDone() can be used as the return status code from the program’s main() 
 * routine.
 * 
 * On vxWorks and RTEMS, an alternative test harness can be used to run a 
 * series of tests in order and summarize the results from them all at the end
 * just like the Perl harness does. The routine testHarness() is called once at
 * the beginning of the test harness program. Each test program is run by 
 * passing its main routine name to the runTest() macro which expands into a call
 * to the runTestFunc() routine. The last test program or the harness program 
 * itself must ﬁnish by calling epicsExit() which triggers the test summary 
 * mechanism to generate its result outputs (from an epicsAtExit callback 
 * routine).
 * 
 * Some tests require the context of an IOC to be run. This conﬂicts with the 
 * idea of running multiple tests within a test harness, as iocInit() is only 
 * allowed to be called once, and some parts of the full IOC (e.g. the rsrv CA 
 * server) can not be shut down cleanly. The function iocBuildIsolated() allows
 * to start an IOC without its Channel Access parts, so that it can be shutdown
 * quite cleany using iocShutdown(). This feature is only intended to be used 
 * from test programs. Do not use it on productional IOCs. After building the IOC
 * using iocBuildIsolated() or iocBuild(), it has to be started by calling 
 * iocRun(). The suggested call sequence in a test program that needs to run the 
 * IOC without Channel Access is:
@code 
#include "iocInit.h" 
 
MAIN(iocTest) 
{ 
    iocBuildIsolated() || iocRun(); 
 
    /⋆ ... test code ... ⋆/ 
 
    iocShutdown(); 
    dbFreeBase(pdbbase); 
    registryFree(); 
    pdbbase = NULL; 
    return testDone(); 
}
@endcode

 * The part from iocBuildIsolated() to iocShutdown() can be repeated to 
 * execute multiple tests within one executable or harness.
 * 
 * To make it easier to create a single test program that can be built for
 * both the embedded and workstation operating system harnesses, the header file
 * testMain.h provides a convenience macro MAIN() that adjusts the name of the 
 * test program according to the platform it is running on: main() on 
 * workstations and a regular function name on embedded systems.
 * 
 * The following is a simple example of a test program using the epicsUnitTest routines:
@code
#include <math.h> 
#include "epicsUnitTest.h" 
#include "testMain.h" 
 
MAIN(mathTest) 
{ 
    testPlan(3); 
    testOk(sin(0.0) == 0.0, "Sine␣starts"); 
    testOk(cos(0.0) == 1.0, "Cosine␣continues"); 
    if (!testOk1(M_PI == 4.0⋆atan(1.0))) 
        testDiag("4⋆atan(1)␣=␣%g", 4.0⋆atan(1.0)); 
    return testDone(); 
}
@endcode

 * The output from running the above program looks like this:
@code
1..3
ok  1 - Sine starts
ok  2 - Cosine continues
ok  3 - M_PI == 4.0⋆atan(1.0)

    Results
    =======
       Tests: 3
      Passed:  3 = 100%
@endcode
 */

#ifndef INC_epicsUnitTest_H
#define INC_epicsUnitTest_H

#include <stdarg.h>

#include "compilerDependencies.h"
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

epicsShareFunc void testPlan(int tests);
epicsShareFunc int  testOkV(int pass, const char *fmt, va_list pvar);
epicsShareFunc int  testOk(int pass, const char *fmt, ...)
						EPICS_PRINTF_STYLE(2, 3);
epicsShareFunc void testPass(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc void testFail(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc void testSkip(int skip, const char *why);
epicsShareFunc void testTodoBegin(const char *why);
epicsShareFunc void testTodoEnd(void);
epicsShareFunc int  testDiag(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc void testAbort(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc int  testDone(void);

#define testOk1(cond) testOk(cond, "%s", #cond)


typedef int (*TESTFUNC)(void);
epicsShareFunc void testHarness(void);
epicsShareFunc void testHarnessExit(void *dummy);
epicsShareFunc void runTestFunc(const char *name, TESTFUNC func);

#define runTest(func) runTestFunc(#func, func)
#define testHarnessDone() testHarnessExit(0)

#ifdef __cplusplus
}
#endif

#endif /* INC_epicsUnitTest_H */
