/* Generated file epicsVersion.h */

#ifndef INC_epicsVersion_H
#define INC_epicsVersion_H

#define EPICS_VERSION        7
#define EPICS_REVISION       0
#define EPICS_MODIFICATION   1
#define EPICS_PATCH_LEVEL    2
#define EPICS_DEV_SNAPSHOT   "-DOXYGEN"
#define EPICS_SITE_VERSION   ""

#define EPICS_VERSION_SHORT  "7.0.1.2"
#define EPICS_VERSION_FULL   "7.0.1.2-DOXYGEN"
#define EPICS_VERSION_STRING "EPICS 7.0.1.2-DOXYGEN"
#define epicsReleaseVersion  "EPICS R7.0.1.2-DOXYGEN"

#ifndef VERSION_INT
#  define VERSION_INT(V,R,M,P) ( ((V)<<24) | ((R)<<16) | ((M)<<8) | (P))
#endif
#define EPICS_VERSION_INT VERSION_INT(7, 0, 1, 2)

#endif /* INC_epicsVersion_H */
