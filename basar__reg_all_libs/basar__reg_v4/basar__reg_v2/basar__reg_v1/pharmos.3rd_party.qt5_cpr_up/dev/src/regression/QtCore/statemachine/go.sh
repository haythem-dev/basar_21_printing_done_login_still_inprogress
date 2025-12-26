#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash statemachine_example main.o
qmake -makefile statemachine_example.pro 2>&1 | tee go.out
gmake 2>&1                    | tee -a go.out
ldd statemachine_example 2>&1 | tee -a go.out
./statemachine_example 2>&1   | tee -a go.out
rm -f *~ .qmake.stash main.o

