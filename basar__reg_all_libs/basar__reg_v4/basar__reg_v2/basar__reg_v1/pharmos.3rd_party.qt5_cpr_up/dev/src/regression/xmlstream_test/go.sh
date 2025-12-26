#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH

cd "$(dirname "$0")"

rm -f .qmake.stash xmlstream_test *.o
qmake -makefile xmlstream_test.pro 2>&1 | tee go.out
gmake                             2>&1 | tee -a go.out
ldd xmlstream_test                2>&1 | tee -a go.out
./xmlstream_test                  2>&1 | tee -a go.out
rm -f *~ .qmake.stash *.o