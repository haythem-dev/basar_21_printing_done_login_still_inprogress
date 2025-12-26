#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH

cd "$(dirname "$0")"

rm -f .qmake.stash json_processing *.o
qmake -makefile json_processing.pro 2>&1 | tee go.out
gmake                              2>&1 | tee -a go.out
ldd json_processing                2>&1 | tee -a go.out
./json_processing                  2>&1 | tee -a go.out
rm -f *~ .qmake.stash *.o