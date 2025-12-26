#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash dateinfo main.o
qmake -makefile dateinfo.pro     2>&1 | tee go.out
gmake                            2>&1 | tee -a go.out
ldd dateinfo                     2>&1 | tee -a go.out
./dateinfo 1685-03-21 1750-07-28 2>&1 | tee -a go.out
./dateinfo 2024-04-22 2024-04-10 2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
