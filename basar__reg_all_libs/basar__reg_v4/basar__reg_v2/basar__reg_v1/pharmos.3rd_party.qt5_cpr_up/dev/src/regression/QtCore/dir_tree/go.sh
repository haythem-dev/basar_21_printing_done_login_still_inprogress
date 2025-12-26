#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash dir_tree main.o
qmake -makefile dir_tree.pro 2>&1 | tee go.out
gmake                        2>&1 | tee -a go.out
ldd dir_tree                 2>&1 | tee -a go.out
./dir_tree "${QTDIR}/lib"    2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
