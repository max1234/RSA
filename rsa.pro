#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T05:22:13
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = rsa
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    rsa.cpp \
    algorithms.cpp \
    long.cpp

HEADERS += \
    rsa.h \
    algorithms.h \
    long.h

unix|win32: LIBS += -L$$PWD/ -llibgmp-3

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
