#!/usr/bin/ksh

export QTDIR=$PWD/pkg-install
export BUILD_QTDIR=$PWD/build/qtbase/release
export PATH=$QTDIR/bin:$PATH

# CRITICAL: Ensure build directory has the required libraries
# Create symbolic links to resolve path mismatch
mkdir -p "$BUILD_QTDIR/lib"
ln -sf "$QTDIR/lib/libQt5Core.so" "$BUILD_QTDIR/lib/libQt5Core.so" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Sql.so" "$BUILD_QTDIR/lib/libQt5Sql.so" 2>/dev/null

# AIX-specific library path settings - Include BOTH install and build paths
export LIBPATH=$QTDIR/lib:$BUILD_QTDIR/lib:$QTDIR/plugins/sqldrivers:$LIBPATH

# Plugin path settings - CRITICAL FOR AIX
export QT_PLUGIN_PATH=$QTDIR/plugins:$QTDIR/plugins/sqldrivers
export QT_QPA_PLATFORM_PLUGIN_PATH=$QTDIR/plugins/platforms

# AIX specific - Force plugin loading
export QT_FORCE_PLUGINS=1
export QT_DEBUG_PLUGINS=1

cd "$(dirname "$0")"

rm -f .qmake.stash sql_database *.o test.db

# Enhanced debugging
echo "=== AIX Library Path Debug ==="
echo "QTDIR: $QTDIR"
echo "BUILD_QTDIR: $BUILD_QTDIR"
echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo "LIBPATH: $LIBPATH"
echo "QT_PLUGIN_PATH: $QT_PLUGIN_PATH"

# Verify symbolic links
echo "=== Verifying Library Links ==="
echo "Build Qt5Core link:" $(ls -la "$BUILD_QTDIR/lib/libQt5Core.so" 2>/dev/null || echo "NOT FOUND")
echo "Build Qt5Sql link:" $(ls -la "$BUILD_QTDIR/lib/libQt5Sql.so" 2>/dev/null || echo "NOT FOUND")

# Check plugin dependencies after links
echo "=== Plugin Dependencies After Links ==="
ldd "$QTDIR/plugins/sqldrivers/libqsqlite.so"

echo "=== End Debug ==="

qmake -makefile sql_database.pro 2>&1 | tee go.out
gmake                            2>&1 | tee -a go.out
ldd sql_database                 2>&1 | tee -a go.out

# Run with comprehensive debugging
echo "=== Running with debug flags ==="
QT_DEBUG_PLUGINS=1 QT_FORCE_PLUGINS=1 LIBPATH=$QTDIR/lib:$BUILD_QTDIR/lib:$QTDIR/plugins/sqldrivers:$LIBPATH ./sql_database 2>&1 | tee -a go.out

rm -f *~ .qmake.stash *.o test.db