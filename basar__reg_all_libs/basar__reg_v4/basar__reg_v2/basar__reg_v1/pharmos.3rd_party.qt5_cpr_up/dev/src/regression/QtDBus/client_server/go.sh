#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash client_server main.o
qmake -makefile client_server.pro 2>&1 | tee go.out
gmake                    2>&1 | tee -a go.out
ldd client_server        2>&1 | tee -a go.out
./client_server --help   2>&1 | tee -a go.out
./client_server --server 2>&1 | tee -a go.out
./client_server --client 2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o
