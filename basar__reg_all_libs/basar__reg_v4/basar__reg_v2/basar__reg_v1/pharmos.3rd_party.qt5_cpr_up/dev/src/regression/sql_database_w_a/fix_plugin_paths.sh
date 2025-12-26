#!/usr/bin/ksh

# Script to fix SQLite plugin dependency paths

QTDIR=$PWD/../../pkg-install
BUILD_QTDIR=$PWD/../../build/qtbase/release
PLUGIN_FILE="$QTDIR/plugins/sqldrivers/libqsqlite.so"

echo "=== Fixing SQLite Plugin Paths ==="

# Backup original plugin
cp "$PLUGIN_FILE" "$PLUGIN_FILE.backup"

echo "Original plugin dependencies:"
ldd "$PLUGIN_FILE"

# Method 1: Create symbolic links in build directory
echo "Creating symbolic links in build directory..."
mkdir -p "$BUILD_QTDIR/lib"
ln -sf "$QTDIR/lib/libQt5Core.so" "$BUILD_QTDIR/lib/libQt5Core.so"
ln -sf "$QTDIR/lib/libQt5Sql.so" "$BUILD_QTDIR/lib/libQt5Sql.so"

echo "Links created:"
ls -la "$BUILD_QTDIR/lib/"

echo "Verifying dependencies after fix:"
ldd "$PLUGIN_FILE"

echo "=== Fix completed ==="