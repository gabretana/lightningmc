#-------------------------------------------------
#
# Project created by QtCreator 2015-08-13T09:55:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lightningmc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    convert.cpp \
    lightningfiles.cpp \
    codecconfigdialog.cpp

HEADERS  += mainwindow.h \
    convert.h \
    lightningfiles.h \
    codecconfigdialog.h

FORMS    += mainwindow.ui \
    codecconfigdialog.ui

RESOURCES += \
    application.qrc
