#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash fwatcher_example main.o
qmake -makefile fwatcher_example.pro 2>&1 | tee go.out
gmake 2>&1 | tee -a go.out
ldd fwatcher_example 2>&1 | tee -a go.out
./fwatcher_example /proc/sys/disk/lbp/* >> go.out 2>&1 &

thePID=$!
sleep 5
kill -HUP "$thePID"
cat -n go.out
