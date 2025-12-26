QT += core network
QT -= gui

CONFIG += console
CONFIG -= app_bundle
CONFIG -= ssl

TARGET = http_nossl_local
TEMPLATE = app

SOURCES += main.cpp \
           ../common/network_tester.cpp

HEADERS += ../common/network_tester.h

INCLUDEPATH += ../common

# AIX-specific settings
unix:aix {
    QMAKE_CXXFLAGS += -pthread
    QMAKE_LFLAGS += -pthread
    LIBS += -lpthread
}

# Ensure proper library linking for AIX
unix:aix {
    LIBS += -L/usr/lib -lpthread -lc_r
}