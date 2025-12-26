#!/usr/bin/bash -e

. $(pwd)/proc/build-common.inc.sh

pushd $(pwd)/qttranslations* >/dev/null
translations_src_dir=$(pwd)
popd >/dev/null
#Force build directory to correct location
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
src_dir="$(dirname "$script_dir")"
translations_build_dir="${src_dir}/build/qttranslations"

mkdir -p $translations_build_dir
pushd $translations_build_dir >/dev/null

for variant in "debug" "release" ; do

    mkdir -p ${variant}
    pushd ${variant} >/dev/null

    params=""
    if [ "${variant}" = "debug" ] ; then
	params="${params}CONFIG+=debug QT_LIBINFIX=d "
    fi

    echo "configuring qttranslations ( ${variant} )" >&2

    $QMAKE $params $translations_src_dir/qttranslations.pro

    echo "done" >&2
    echo "" >&2

    echo "building qttranslations ( ${variant} )" >&2
    make -j${jobs}
    echo "done" >&2
    echo "" >&2

    echo "installing qttranslations ( ${variant} )" >&2
    make -j${jobs} install
    echo "done" >&2
    echo "" >&2

    popd >/dev/null
done
popd >/dev/null

