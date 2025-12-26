QT += core
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = threading_advanced
TEMPLATE = app

SOURCES += main.cpp

# Enable debug output
CONFIG += debug_and_release