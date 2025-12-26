#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH

cd "$(dirname "$0")"

rm -f .qmake.stash threading_advanced *.o
qmake -makefile threading_advanced.pro 2>&1 | tee go.out
gmake                               2>&1 | tee -a go.out
ldd threading_advanced              2>&1 | tee -a go.out
./threading_advanced                2>&1 | tee -a go.out
rm -f *~ .qmake.stash *.o