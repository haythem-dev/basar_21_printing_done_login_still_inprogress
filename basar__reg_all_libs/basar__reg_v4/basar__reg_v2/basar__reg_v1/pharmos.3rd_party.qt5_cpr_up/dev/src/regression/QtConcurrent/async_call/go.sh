#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash async_call main.o
qmake -makefile async_call.pro 2>&1 | tee go.out
gmake             2>&1 | tee -a go.out
ldd async_call    2>&1 | tee -a go.out
./async_call      2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
