#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash tablemodel main.o input.html
qmake -makefile tablemodel.pro 2>&1 | tee go.out
gmake                          2>&1 | tee -a go.out
ldd tablemodel                 2>&1 | tee -a go.out
./tablemodel                   2>&1 | tee -a go.out

cat > input.html <<EOF
<table>
<tr><th>Name</th><th>Age</th></tr>
<tr><td>Alice</td><td>30</td></tr>
<tr><td>Bob</td><td>25</td></tr>
<tr><td>Charlie</td><td>35</td></tr>
</table>
EOF
./tablemodel input.html        2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o input.html
