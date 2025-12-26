#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash hello_qt5 main.o
qmake -makefile hello.pro 2>&1 | tee go.out
gmake            2>&1 | tee -a go.out
ldd hello_qt5    2>&1 | tee -a go.out
./hello_qt5      2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
