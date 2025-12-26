TARGET = ftp_download
CONFIG += console
CONFIG -= app_bundle
QT += core
QT -= gui

SOURCES += main.cpp

# AIX specific configuration
unix {
    QMAKE_CXXFLAGS += -pthread
    QMAKE_LFLAGS += -pthread
}