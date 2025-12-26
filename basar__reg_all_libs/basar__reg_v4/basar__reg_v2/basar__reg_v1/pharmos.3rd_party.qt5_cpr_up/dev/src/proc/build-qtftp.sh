#!/usr/bin/bash -e

. $(pwd)/proc/build-common.inc.sh

pushd $(pwd)/qtftp* >/dev/null
    ftp_src_dir=$(pwd)
popd >/dev/null

ftp_build_dir=$(pwd)/build/qtftp

mkdir -p $ftp_build_dir
pushd $ftp_build_dir >/dev/null

for variant in "debug" "release" ; do

    mkdir -p ${variant}
    pushd ${variant} >/dev/null

    params=""
    if [ "${variant}" = "debug" ] ; then
        params="${params}CONFIG+=debug QT_LIBINFIX=d "
    fi

    echo "configuring qtftp ( ${variant} )" >&2

    $QMAKE $params $ftp_src_dir/qtftp.pro

    echo "done" >&2
    echo "" >&2

    echo "building qtftp ( ${variant} )" >&2
    make -j${jobs}
    echo "done" >&2
    echo "" >&2

    echo "installing qtftp ( ${variant} )" >&2
    make -j${jobs} install
    echo "done" >&2
    echo "" >&2

    popd >/dev/null
done
popd >/dev/null

cp -f ${ftp_src_dir}/src/qftp/*.h ${install_dir}/include/QtFtp/
cp -rf ${src_dir}/include/QtFtp ${install_dir}/include/
cp -rf ${src_dir}/include/QtNetwork ${install_dir}/include/
