#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash qrep main.o qrep_*
qmake -makefile qrep.pro                   2>&1 | tee go.out
gmake                                      2>&1 | tee -a go.out
./qrep '(file|File|CON)' main.cpp qrep.pro 2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
