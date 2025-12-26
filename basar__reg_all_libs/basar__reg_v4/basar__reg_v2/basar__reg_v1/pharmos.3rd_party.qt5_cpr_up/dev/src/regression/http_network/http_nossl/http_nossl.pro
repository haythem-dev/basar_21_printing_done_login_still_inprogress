QT += core network
QT -= gui

CONFIG += console
CONFIG -= app_bundle
CONFIG -= ssl

TARGET = http_nossl
TEMPLATE = app

SOURCES += main.cpp

# Disable SSL for this test
DEFINES += QT_NO_OPENSSL

# AIX-specific settings for threading and networking
unix:aix {
    QMAKE_CXXFLAGS += -pthread -maix64
    QMAKE_LFLAGS += -pthread -maix64
    LIBS += -lpthread -lpthreads -lc_r
}

# Ensure proper library linking for AIX
unix:aix {
    LIBS += -L/usr/lib -lpthread -lc_r
}

# Debug settings
CONFIG(debug, debug|release) {
    DEFINES += QT_DEBUG
}