#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash dom_example main.o
qmake -makefile dom_example.pro 2>&1 | tee go.out
gmake                           2>&1 | tee -a go.out
dump -X64 -Hv dom_example       2>&1 | tee -a go.out
./dom_example                   2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
