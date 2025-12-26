#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash sysinfo main.o
qmake -makefile sysinfo.pro 2>&1 | tee go.out
gmake                       2>&1 | tee -a go.out
sysinfo                     2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
