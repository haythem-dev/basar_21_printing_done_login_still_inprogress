#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash crypt_sha main.o
qmake -makefile crypt_sha.pro 2>&1 | tee go.out
gmake 2>&1         | tee -a go.out
ldd crypt_sha 2>&1 | tee -a go.out
./crypt_sha 2>&1   | tee -a go.out

rm -f *~ .qmake.stash main.o
