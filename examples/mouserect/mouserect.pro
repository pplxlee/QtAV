#-------------------------------------------------
#
# Project created by QtCreator 2019-06-21T09:48:23
#
#-------------------------------------------------

TEMPLATE = app
CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PROJECTROOT = $$PWD/../..
include($$PROJECTROOT/src/libQtAV.pri)
include($$PROJECTROOT/widgets/libQtAVWidgets.pri)
preparePaths($$OUT_PWD/../../out)


CONFIG += c++11

SOURCES += \
        main.cpp \
        PlayerWindow.cpp

HEADERS += \
        PlayerWindow.h

