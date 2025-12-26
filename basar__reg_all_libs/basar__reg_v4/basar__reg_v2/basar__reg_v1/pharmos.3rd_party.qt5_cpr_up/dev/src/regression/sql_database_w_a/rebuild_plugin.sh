#!/usr/bin/ksh

# Script to rebuild the SQLite plugin with correct paths

QTDIR=$PWD/../../pkg-install
BUILD_QTDIR=$PWD/../../build/qtbase/release
QT_SOURCE_DIR=$PWD/../../build/qtbase

echo "=== Rebuilding SQLite Plugin ==="

# First, let's try the symbolic link approach (easier and safer)
echo "Creating symbolic links for library compatibility..."
mkdir -p "$BUILD_QTDIR/lib"
ln -sf "$QTDIR/lib/libQt5Core.so" "$BUILD_QTDIR/lib/libQt5Core.so" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Sql.so" "$BUILD_QTDIR/lib/libQt5Sql.so" 2>/dev/null

echo "Links created:"
ls -la "$BUILD_QTDIR/lib/"

echo "Verifying plugin dependencies after links:"
ldd "$QTDIR/plugins/sqldrivers/libqsqlite.so"

echo "=== Link-based fix completed ==="