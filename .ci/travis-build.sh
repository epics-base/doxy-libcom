#!/bin/sh
set -e -x

export EPICS_HOST_ARCH=`sh startup/EpicsHostArch`

[ -e configure/os/CONFIG_SITE.Common.linux-x86 ] || die "Wrong location: $PWD"

make

