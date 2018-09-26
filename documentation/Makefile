#*************************************************************************
# Copyright (c) 2017 UChicago Argonne LLC, as Operator of Argonne
#     National Laboratory.
# EPICS BASE is distributed subject to a Software License Agreement found
# in file LICENSE that is included with this distribution.
#*************************************************************************

TOP = ..
include $(TOP)/configure/CONFIG

EXPAND = Doxyfile
# Generate Doxyfile from Doxyfile@ with these values:
EXPAND_VARS += PROJECT_NUMBER=$(EPICS_SHORT_VERSION)
EXPAND_VARS += INCLUDE_PATH=$(INSTALL_INCLUDE)
EXPAND_VARS += HTML_OUTPUT=$(INSTALL_HTML)/doxygen

# Run doxygen to generate API documentation from source-files.
# Requires doxygen, latex, pdflatex and Graphviz (for dot)
TARGETS += doxygen
# Clean doxygen's working directory:
CLEANDIRS += O.latex

include $(TOP)/configure/RULES.documentation

Doxyfile: Makefile
