#!/usr/bin/bash -e

. $(pwd)/proc/build-common.inc.sh

pushd $(pwd)/qtbase* >/dev/null
    base_src_dir=$(pwd)
popd >/dev/null

# Always use dev/src/build regardless of execution context
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
src_dir="$(dirname "$script_dir")"
base_build_dir="${src_dir}/build/qtbase"

# Detect AIX runtime libpath
echo "Detecting default LIBPATH..." >&2
echo "int main() {}" > libpath.c
gcc -maix64 -pthread -lpthread -o libpath libpath.c
libpath=$(dump -H -X64 libpath 2>/dev/null | awk '/^0[ \\t]/ {print $2}')
rm -f libpath libpath.c
echo "LIBPATH=${libpath}" >&2
echo "done" >&2
echo "" >&2

mkdir -p "${base_build_dir}"
pushd "${base_build_dir}" >/dev/null

# Create soft-links: lib.so -> lib.so.x.y.z
dot_so_link() {
    theLib="$1"
    theTargetDir="$2"
    theDotSO=$(echo "$1" | cut -d. -f1-2)
    goBack=$(pwd)

    printf "Soft-linking within %s: %s -> %s\n" "$theTargetDir" "$theDotSO" "$theLib"
    mkdir -p "$theTargetDir" && cd "$theTargetDir" && \
    touch "$theLib" && ln -sf "$theLib" "$theDotSO" && cd "$goBack"
}

# Determine build variants based on BUILD_VARIANT environment variable
case "${BUILD_VARIANT}" in
    "debug")
        echo "Building debug variant only" >&2
        variants="debug"
        ;;
    "release")
        echo "Building release variant only" >&2
        variants="release"
        ;;
    "both")
        echo "Building both debug and release variants" >&2
        variants="debug release"
        ;;
    *)
        echo "BUILD_VARIANT not set or unknown value '${BUILD_VARIANT}', defaulting to release only" >&2
        variants="release"
        ;;
esac

for variant in $variants; do
    echo "========================================" >&2
    echo "Starting build for variant: ${variant}" >&2
    echo "========================================" >&2

    mkdir -p "${variant}"
    pushd "${variant}" >/dev/null

    echo "Configuring qtbase (${variant})..." >&2

    # Base parameters common to all variants
    base_params="\
        QMAKE_CXXFLAGS+=-std=c++17 \
        QMAKE_CXXFLAGS+=-Wno-implicit-fallthrough \
        QMAKE_CXXFLAGS+=-Wno-unused-parameter \
        QMAKE_CXXFLAGS+=-pthread \
        QMAKE_CXXFLAGS+=-D_GNU_SOURCE \
        QMAKE_CXXFLAGS+=-D_THREAD_SAFE \
        QMAKE_CXXFLAGS+=-D_REENTRANT \
        -opensource -verbose \
        -confirm-license \
        -strip -shared \
        -platform aix-g++-64 \
        -c++std c++17 \
        -no-gui \
        -no-widgets \
        -no-openssl \
        -no-opengl \
        -no-pkg-config \
        -nomake examples \
        -nomake tools \
        -feature-bearermanagement \
        -feature-codecs \
        -feature-commandlineparser \
        -feature-completer \
        -feature-concurrent \
        -feature-cssparser \
        -feature-datestring \
        -feature-datetimeparser \
        -feature-desktopservices \
        -feature-dirmodel \
        -feature-dnslookup \
        -feature-dom \
        -feature-filesystemiterator \
        -feature-filesystemmodel \
        -feature-filesystemwatcher \
        -feature-ftp \
        -feature-http \
        -feature-library \
        -feature-localserver \
        -feature-mimetype \
        -feature-multiprocess \
        -feature-networkdiskcache \
        -feature-networkinterface \
        -feature-networkproxy \
        -feature-process \
        -feature-processenvironment \
        -feature-properties \
        -feature-regularexpression \
        -feature-settings \
        -feature-sha3-fast \
        -feature-sharedmemory \
        -feature-socks5 \
        -feature-sqlmodel \
        -feature-statemachine \
        -feature-systemsemaphore \
        -feature-temporaryfile \
        -feature-textcodec \
        -feature-textdate \
        -feature-texthtmlparser \
        -feature-textmarkdownreader \
        -feature-textmarkdownwriter \
        -feature-textodfwriter \
        -feature-thread \
        -feature-timezone \
        -feature-topleveldomain \
        -feature-translation \
        -feature-udpsocket \
        -feature-xmlstream \
        -feature-xmlstreamreader \
        -feature-xmlstreamwriter \
        -L/usr/lib \
    "

    # Variant-specific parameters
    if [ "${variant}" = "debug" ]; then
        echo "Configuring debug build with testing features..." >&2
        params="${base_params} \
            -debug \
            -qtlibinfix d \
            -feature-testlib \
            -feature-itemmodeltester \
        "
    else
        echo "Configuring release build without testing features..." >&2
        params="${base_params} \
            -release \
            -no-feature-testlib \
        "
    fi

    # Set variant_suffix variable for later use
    variant_suffix=""
    if [ "${variant}" = "debug" ]; then
        variant_suffix="d"
    fi

    spec_dir="${base_src_dir}/mkspecs/aix-g++-64"
    qconf="${spec_dir}/qmake.conf"
    qconf_backup="${qconf}.bak"

    if [ ! -f "${qconf_backup}" ]; then
        cp "${qconf}" "${qconf_backup}"
    fi
    cp "${qconf_backup}" "${qconf}"

    # Enhanced qmake.conf modifications with proper threading support
    echo "Updating qmake.conf for ${variant} build..." >&2
    sed -i \
        -e "s#QMAKE_LFLAGS[[:space:]]*=.*#QMAKE_LFLAGS = -pthread -Wl,-brtl -Wl,-G -Wl,-bbigtoc -maix64 -lpthread -lpthreads -lc_r#g" \
        -e "s#QMAKE_CFLAGS[[:space:]]*=.*#QMAKE_CFLAGS = -maix64 -fPIC -D_LARGE_FILES -D_LARGEFILE_SOURCE -D_REENTRANT -D_THREAD_SAFE -D_GNU_SOURCE -ftls-model=initial-exec -pthread#g" \
        -e "s#QMAKE_CXXFLAGS[[:space:]]*=.*#QMAKE_CXXFLAGS = \$\$QMAKE_CFLAGS -maix64 -fPIC -std=c++17 -fno-strict-aliasing -ftls-model=initial-exec -pthread -D_GNU_SOURCE -D_THREAD_SAFE -D_REENTRANT#g" \
        -e "s#QMAKE_LIBS_THREAD[[:space:]]*=.*#QMAKE_LIBS_THREAD = -lpthread -lpthreads -lc_r#g" \
        -e "s#QMAKE_LIBS[[:space:]]*=.*#QMAKE_LIBS = -lpthread -lpthreads -lc_r#g" \
        "${qconf}"

    # Add threading-specific defines to qmake.conf
    echo "" >> "${qconf}"
    echo "# Threading support for Qt ${variant} build" >> "${qconf}"
    echo "DEFINES += _GNU_SOURCE _THREAD_SAFE _REENTRANT" >> "${qconf}"
    echo "QMAKE_CXXFLAGS += -pthread" >> "${qconf}"
    echo "QMAKE_LFLAGS += -pthread" >> "${qconf}"
    
    if [ "${variant}" = "debug" ]; then
        echo "DEFINES += QT_TESTLIB_LIB" >> "${qconf}"
    fi

    # Export flags for configure probes - variant specific with enhanced threading support
    echo "Setting up environment variables for ${variant} build..." >&2
    
    # Common threading flags
    COMMON_THREAD_FLAGS="-D_THREAD_SAFE -D_REENTRANT -D_GNU_SOURCE -ftls-model=initial-exec -pthread"
    COMMON_DEFINES="-DHAVE_POSIX_FALLOCATE=0"
    
    if [ "${variant}" = "debug" ]; then
        export CFLAGS="-maix64 -g -O0 -fno-strict-aliasing ${COMMON_THREAD_FLAGS} ${COMMON_DEFINES} -DQT_TESTLIB_LIB"
        export CXXFLAGS="-maix64 -fPIC -g -O0 -fno-strict-aliasing -std=c++17 ${COMMON_THREAD_FLAGS} ${COMMON_DEFINES} -DQT_TESTLIB_LIB"
    else
        export CFLAGS="-maix64 -O2 -fno-strict-aliasing ${COMMON_THREAD_FLAGS} ${COMMON_DEFINES}"
        export CXXFLAGS="-maix64 -fPIC -O2 -fno-strict-aliasing -std=c++17 ${COMMON_THREAD_FLAGS} ${COMMON_DEFINES}"
    fi
    
    export LDFLAGS="-pthread -Wl,-bbigtoc -lpthread -lpthreads -lc_r"
    
    # Additional environment variables for Qt configure
    export QMAKE_CFLAGS="$CFLAGS"
    export QMAKE_CXXFLAGS="$CXXFLAGS"
    export QMAKE_LFLAGS="$LDFLAGS"
    
    # Threading library configuration
    export QMAKE_LIBS_THREAD="-lpthread -lpthreads -lc_r"
    export QMAKE_LIBS="-lpthread -lpthreads -lc_r"

    echo "Environment setup complete for ${variant}" >&2
    echo "CFLAGS: $CFLAGS" >&2
    echo "CXXFLAGS: $CXXFLAGS" >&2
    echo "LDFLAGS: $LDFLAGS" >&2
    echo "" >&2

    # Run configure with enhanced error handling
    echo "Running configure for ${variant} build..." >&2
    configure_cmd="${base_src_dir}/configure \
        $params \
        -prefix ${install_dir} \
        -extprefix ${install_dir} \
        -hostprefix ${install_dir} \
        -I${base_build_dir}/${variant}/include \
        -platform aix-g++-64 \
        -opensource \
        -confirm-license \
        -${variant}"

    echo "Configure command: $configure_cmd" >&2
    
    if ! $configure_cmd 2>&1 | tee configure_${variant}.log; then
        echo "ERROR: Configure failed for ${variant} build!" >&2
        echo "Check configure_${variant}.log for details" >&2
        exit 1
    fi

    echo "Configure completed successfully for ${variant}" >&2
    echo "Building qtbase (${variant})..." >&2

    # Create soft-links for libraries (variant-specific)
    echo "Creating library soft-links for ${variant}..." >&2
    if [ "${variant}" = "debug" ]; then
        for theLib in Concurrent Core DBus Network Sql Xml; do
            dot_so_link "libQt5${theLib}d.so.5.15.16" "${base_build_dir}/${variant}/lib"
        done
    else
        for theLib in Concurrent Core DBus Network Sql Xml; do
            dot_so_link "libQt5${theLib}.so.5.15.16" "${base_build_dir}/${variant}/lib"
        done
    fi

    echo "Starting full QtBase build for ${variant}..." >&2
    
    # Build with enhanced error handling
    if ! make -j${jobs} 2>&1 | tee build_${variant}.log; then
        echo "ERROR: Build failed for ${variant}!" >&2
        echo "Check build_${variant}.log for details" >&2
        echo "Last 50 lines of build log:" >&2
        tail -50 build_${variant}.log >&2
        exit 1
    fi

    echo "Build qtbase completed successfully for ${variant}" >&2
    echo "Installing qtbase (${variant})..." >&2

    # Install with error handling
    if ! make -j${jobs} install 2>&1 | tee install_${variant}.log; then
        echo "ERROR: Installation failed for ${variant}!" >&2
        echo "Check install_${variant}.log for details" >&2
        exit 1
    fi

    echo "Installation qtbase completed for ${variant}" >&2

    # --- Create unversioned symlinks in the install directory ---
    echo "Creating unversioned symlinks in the installation directory for ${variant}..." >&2

    install_lib_path="${install_dir}/lib"

    if [ "${variant}" = "debug" ]; then
        for theLib in Concurrent Core DBus Network Sql Xml; do
            dot_so_link "libQt5${theLib}d.so.5.15.16" "${install_lib_path}"
        done
    else
        for theLib in Concurrent Core DBus Network Sql Xml; do
            dot_so_link "libQt5${theLib}.so.5.15.16" "${install_lib_path}"
        done
    fi
    
    echo "Done creating installation symlinks for ${variant}." >&2
    # --- END Installation Symlinks ---

    # Verify installation
    echo "Verifying installation for ${variant}..." >&2
    if [ "${variant}" = "debug" ]; then
        test_lib="${install_lib_path}/libQt5Cored.so.5.15.16"
    else
        test_lib="${install_lib_path}/libQt5Core.so.5.15.16"
    fi
    
    if [ -f "$test_lib" ]; then
        echo "✓ Installation verification passed for ${variant}" >&2
        echo "✓ Library found: $test_lib" >&2
    else
        echo "✗ Installation verification failed for ${variant}" >&2
        echo "✗ Library missing: $test_lib" >&2
        echo "Available libraries in ${install_lib_path}:" >&2
        ls -la "${install_lib_path}"/libQt5* 2>/dev/null || echo "No Qt5 libraries found" >&2
    fi

    popd >/dev/null
    
    echo "========================================" >&2
    echo "Completed build for variant: ${variant}" >&2
    echo "========================================" >&2
    echo "" >&2
done

popd >/dev/null

# Copy qt.conf to installation
echo "Copying qt.conf to installation..." >&2
if [ -f "${base_src_dir}/conf/qt.conf" ]; then
    cp -f "${base_src_dir}/conf/qt.conf" "${install_dir}/bin"
    echo "✓ qt.conf copied successfully" >&2
else
    echo "⚠ qt.conf not found at ${base_src_dir}/conf/qt.conf" >&2
fi

echo "" >&2
echo "========================================" >&2
echo "ALL BUILDS COMPLETED SUCCESSFULLY!" >&2
echo "========================================" >&2
echo "Build summary:" >&2
echo "- Variants built: ${BUILD_VARIANT:-release (default)}" >&2
echo "- Installation directory: ${install_dir}" >&2
echo "- Build logs available in: ${base_build_dir}/" >&2
echo "" >&2
echo "Built libraries:" >&2
ls -la "${install_dir}"/lib/libQt5* 2>/dev/null || echo "No Qt5 libraries found in installation directory" >&2
echo "" >&2
