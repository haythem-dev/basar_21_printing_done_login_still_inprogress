#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash main.o

qmake -makefile clientserver.pro  2>&1 | tee go.out
gmake                             2>&1 | tee -a go.out
./clientserver                    2>&1 | tee -a go.out
./clientserver --server >> go.out 2>&1 &
sleep 2
./clientserver --client >> go.out 2>&1 &

thePID=$!
sleep 2
kill -HUP "${thePID}"             2>&1 | tee -a go.out
echo "clientserver: test done."   2>&1 | tee -a go.out

rm -f *~ .qmake.stash main.o
