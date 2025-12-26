#!/usr/bin/bash -e

. $(pwd)/proc/build-common.inc.sh

pushd $(pwd)/qttools* >/dev/null
tools_src_dir=$(pwd)
popd >/dev/null
#Force build directory to correct location
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
src_dir="$(dirname "$script_dir")"
tools_build_dir="${src_dir}/build/qttools"

mkdir -p $tools_build_dir
pushd $tools_build_dir >/dev/null

for variant in "debug" "release" ; do

    mkdir -p ${variant}
    pushd ${variant} >/dev/null

    params=""
    if [ "${variant}" = "debug" ] ; then
	params="${params}CONFIG+=debug QT_LIBINFIX=d "
    fi

    echo "configuring qttools ( ${variant} )" >&2

    $QMAKE $params $tools_src_dir/qttools.pro

    echo "done" >&2
    echo "" >&2

    echo "building qttools ( ${variant} )" >&2
    make -j${jobs}
    echo "done" >&2
    echo "" >&2

    echo "installing qttools ( ${variant} )" >&2
    make -j${jobs} install
    echo "done" >&2
    echo "" >&2

    popd >/dev/null
done
popd >/dev/null

