QT += core sql
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = sql_database
TEMPLATE = app

SOURCES += main.cpp

# Enable debug output
CONFIG += debug_and_release

# AIX specific configurations
aix {
    
    QTPLUGIN += qsqlite
    
    # Add specific AIX library paths
    LIBS += -L$$[QT_INSTALL_PLUGINS]/sqldrivers
    
    # Remove problematic flags that cause g++ errors
    # QMAKE_LFLAGS += -brtl -bexpall  # REMOVED - these cause g++ errors
    
    # Enable plugin debugging
    DEFINES += QT_DEBUG_PLUGINS
    DEFINES += QT_FORCE_PLUGINS
}

# Ensure SQL plugins are included
# CONFIG += sql-plugins  # This might not be needed

# Debug configuration
debug {
    DEFINES += QT_DEBUG_PLUGINS
    DEFINES += QT_FORCE_PLUGINS
}