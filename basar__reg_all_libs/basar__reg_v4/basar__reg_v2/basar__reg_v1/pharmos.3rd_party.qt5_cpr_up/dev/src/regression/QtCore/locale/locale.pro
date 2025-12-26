QT -= gui
QT += core
CONFIG += console
CONFIG -= app_bundle

TARGET = locale
TEMPLATE = app

SOURCES += main.cpp

# Enable translation system
TRANSLATIONS = locale_en.ts \
               locale_de.ts
