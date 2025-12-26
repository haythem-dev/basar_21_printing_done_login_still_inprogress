#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash main.o commandline_example
qmake -makefile commandline_example.pro 2>&1 | tee go.out
gmake 2>&1                             | tee -a go.out
ldd commandline_example 2>&1           | tee -a go.out
./commandline_example --help 2>&1      | tee -a go.out
./commandline_example --name dude 2>&1 | tee -a go.out

rm -f *~ .qmake.stash main.o
