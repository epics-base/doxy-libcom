#!/bin/sh
set -e -x

export EPICS_HOST_ARCH=`perl startup/EpicsHostArch.pl`

[ -e configure/os/CONFIG_SITE.Common.linux-x86 ] || die "Wrong location: $PWD"

make

# Upload the results from html/doxygen to ...
# Use $TRAVIS_PULL_REQUEST to distinguish jobs.


