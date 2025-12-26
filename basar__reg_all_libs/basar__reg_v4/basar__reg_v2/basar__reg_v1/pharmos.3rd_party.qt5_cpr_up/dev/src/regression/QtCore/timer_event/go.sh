#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash timer_event main.o
qmake -makefile timer_event.pro 2>&1 | tee go.out
gmake            2>&1 | tee -a go.out
ldd timer_event  2>&1 | tee -a go.out
./timer_event    2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
