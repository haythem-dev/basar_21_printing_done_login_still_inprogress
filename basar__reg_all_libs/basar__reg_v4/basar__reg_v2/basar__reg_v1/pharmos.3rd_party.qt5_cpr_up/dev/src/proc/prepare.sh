#!/usr/bin/bash -e
top_dir=$(pwd)
dev_dir=${top_dir}/..

for artefact in base tools translations; do
# Qt${artefact} module
# qt${artefact}-everywhere-src-5.15.16.zip
#
echo "extracting qt5${artefact} sources to ${dev_dir}/src..." >&2
# For non-interactive extraction, auto-accept license prompts
export DEBIAN_FRONTEND=noninteractive
echo "A" | /opt/freeware/bin/unzip -q -aa -d ${dev_dir}/src ${dev_dir}/ext/qt5${artefact}_source/qt${artefact}-everywhere-src-5*.zip 2>/dev/null || \
  yes "A" | /opt/freeware/bin/unzip -q -aa -d ${dev_dir}/src ${dev_dir}/ext/qt5${artefact}_source/qt${artefact}-everywhere-src-5*.zip
 
echo "done" >&2
done

echo "" >&2
src_dir=${top_dir}/$(cd ${top_dir} && ls -d qtbase*/ | sort | tail -n 1)
echo "patching qtbase files..." >&2
pushd ${src_dir} >/dev/null
# the patch utility on aix has a serious bug with unified patches, so we need to use a non-unified one...
echo "applying Qt 5.15.16 AIX patches..." >&2
patch -N -i ${top_dir}/patch/aix-mkspecs-common-aix-qplatformdefs.h.patch        mkspecs/common/aix/qplatformdefs.h
patch -N -i ${top_dir}/patch/aix-qmake-library-ioutils.cpp.patch                 qmake/library/ioutils.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-codecs-qutfcodec.cpp.patch          src/corelib/codecs/qutfcodec.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-global-qglobal.cpp.patch            src/corelib/global/qglobal.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-global-qlibraryinfo.cpp.patch       src/corelib/global/qlibraryinfo.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-global-qlogging.cpp.patch           src/corelib/global/qlogging.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-io-qfilesystemengine_unix.cpp.patch src/corelib/io/qfilesystemengine_unix.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-io-qfsfileengine_unix.cpp.patch     src/corelib/io/qfsfileengine_unix.cpp
patch -N -i ${top_dir}/patch/aix-src-corelib-text-qbytearray.h.patch             src/corelib/text/qbytearray.h
patch -N -i ${top_dir}/patch/aix-src-corelib-text-qstring.h.patch                src/corelib/text/qstring.h
patch -N -i ${top_dir}/patch/aix-src-corelib-text-qstring.cpp.patch              src/corelib/text/qstring.cpp
patch -N -i ${top_dir}/patch/aix-src-network-kernel-qdnslookup_unix.cpp.patch    src/network/kernel/qdnslookup_unix.cpp
patch -N -i ${top_dir}/patch/aix-src-3rdparty-sqlite-sqlite3.c.patch             src/3rdparty/sqlite/sqlite3.c
popd >/dev/null
echo "qtbase patching completed" >&2
echo "" >&2
