#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export PATH=$QTDIR/bin:$PATH
cd "$(dirname "$0")"

rm -f .qmake.stash locale main.o locale_*
qmake -makefile locale.pro    2>&1 | tee go.out
gmake                         2>&1 | tee -a go.out
lupdate -verbose locale.pro   2>&1 | tee -a go.out

# Simulate manual update
sed -e '/unfinished/s# type="unfinished">#>Hallo, Welt!#'  locale_de.ts > temp && \
mv temp locale_de.ts
sed -e '/unfinished/s# type="unfinished">#>Hello, world!#' locale_en.ts > temp && \
mv temp locale_en.ts

lrelease locale.pro           2>&1 | tee -a go.out
./locale                      2>&1 | tee -a go.out
./locale --lang=de            2>&1 | tee -a go.out
./locale --lang=fr            2>&1 | tee -a go.out
rm -f *~ .qmake.stash main.o *.ts
