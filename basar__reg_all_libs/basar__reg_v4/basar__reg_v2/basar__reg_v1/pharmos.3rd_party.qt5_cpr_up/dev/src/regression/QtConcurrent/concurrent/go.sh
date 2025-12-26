#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash main.o qt_concurrent_example

qmake -makefile qt_concurrent_example.pro | tee go.out
gmake 2>&1                     | tee -a go.out
ldd qt_concurrent_example 2>&1 | tee -a go.out
./qt_concurrent_example 2>&1   | tee -a go.out

rm -f *~ .qmake.stash main.o
