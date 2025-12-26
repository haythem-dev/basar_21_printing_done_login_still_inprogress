#!/bin/sh

#########################################################################
#
#   archive generated Qt files for deployment
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
echo "create archive ../${PROJECT}-${QT_VERSION}.tar ..."

cd ..

mkdir -p ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/

cp -r -P lib/* ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/

rm -rf ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/pkgconfig
rm     ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/*.la
rm     ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/*.prl
rm     ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/*Gui* 2>/dev/null || true

chmod -R 775 ./tmp/${PROJECT}/${QT_VERSION}/sharedlib/*

tar -cz -C ./tmp -f ./${PROJECT}-${QT_VERSION}.tar.gz .
rm -rf ./tmp

#-------------------------------------------------
echo "runtime files archived"

