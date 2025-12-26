#!/bin/sh

#########################################################################
#
#   install generated Qt files to development directories:
#       - build
#       - runtime
#
#########################################################################

# exit immediately if a command exits with non-zero exit status
set -e

#-------------------------------------------------
# include version variable
. ./vars.inc

#-------------------------------------------------
RTDIR="$WSS/lib/${PROJECT}/${QT_VERSION}"

#-------------------------------------------------
echo "copy build files to $BLDDIR ..."
rm -rf $BLDDIR 2>/dev/null
mkdir -p $BLDDIR
cp -r -P ../bin          $BLDDIR
cp -r -P ../include      $BLDDIR
cp -r -P ../lib          $BLDDIR
cp -r -P ../mkspecs      $BLDDIR
cp -r -P ../plugins      $BLDDIR
cp -r -P ../translations $BLDDIR
cp       ../readme.txt   $BLDDIR

#echo "generating qt.conf for qt binaries at ${BLDDIR}/bin"
#generate custom qt.conf for qt binaries (qmake, moc, uic)
if [ -d ${BLDDIR}/bin ]
then
    cat >${BLDDIR}/bin/qt.conf << EOF
[Paths]
Prefix=${BLDDIR}
EOF
fi

#-------------------------------------------------
echo "copy runtime files to $RTDIR ..."
rm -rf $RTDIR 2>/dev/null
mkdir -p $RTDIR
ln -s $BLDDIR/lib $RTDIR/sharedlib

#-------------------------------------------------
echo "build & runtime files installed"

