#!/usr/bin/ksh

# Quick fix script - just create the symbolic links

QTDIR=$PWD/../../pkg-install
BUILD_QTDIR=$PWD/../../build/qtbase/release

echo "=== Quick Fix: Creating Library Links ==="

# Create build directory if it doesn't exist
mkdir -p "$BUILD_QTDIR/lib"

# Create symbolic links
ln -sf "$QTDIR/lib/libQt5Core.so" "$BUILD_QTDIR/lib/libQt5Core.so" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Sql.so" "$BUILD_QTDIR/lib/libQt5Sql.so" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Core.so.5" "$BUILD_QTDIR/lib/libQt5Core.so.5" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Sql.so.5" "$BUILD_QTDIR/lib/libQt5Sql.so.5" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Core.so.5.15.16" "$BUILD_QTDIR/lib/libQt5Core.so.5.15.16" 2>/dev/null
ln -sf "$QTDIR/lib/libQt5Sql.so.5.15.16" "$BUILD_QTDIR/lib/libQt5Sql.so.5.15.16" 2>/dev/null

echo "Links created:"
ls -la "$BUILD_QTDIR/lib/"

echo "Verifying plugin dependencies:"
ldd "$QTDIR/plugins/sqldrivers/libqsqlite.so"

echo "=== Quick fix completed ==="