:

[ -n "$1" ] && jobs=$1 || jobs=10

src_dir=$(pwd)
install_dir=$(pwd)/pkg-install

export QMAKE=${install_dir}/bin/qmake
export LD_LIBRARY_PATH=${install_dir}/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
unset LIBPATH

echo "" >&2
echo "Using QMAKE=\"$QMAKE\"" >&2
echo "Using LD_LIBRARY_PATH=\"$LD_LIBRARY_PATH\"" >&2
echo "" >&2
