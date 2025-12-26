#!/usr/bin/bash -ex

top_dir=$(pwd)
package_name=qt5
export_dir=${top_dir}/../../export/${package_name}

echo "exporting ${package_name} to ${export_dir}..." >&2

if [ ! -d ${top_dir}/pkg-install ] ; then
	echo "error: ${package_name} has not been built." >&2
	exit 1
fi

pushd ${top_dir}/pkg-install >/dev/null

# move out static libraries, and symlink shared ones to use with aix
mkdir -p libstatic
mv lib/*.a libstatic

for f in $(find lib -name "*.so.5") ; do
	ln -sf $(basename $f) $(dirname $f)/$(basename $f .so.5).so
	ln -sf $(basename $f) $(dirname $f)/$(basename $f .so.5).a
done

popd

mkdir -p ${export_dir}

tar -C ${top_dir}/pkg-install -cvzf ${export_dir}/${package_name}.tar.gz .

cp ${top_dir}/ivy.xml ${export_dir}
cp ${top_dir}/build.xml ${export_dir}

echo "done" >&2
