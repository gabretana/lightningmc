#-------------------------------------------------
#
# Project created by QtCreator 2015-08-13T09:55:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug_and_release

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

target.path = /usr/bin/

icon.files = img/lightningmc.svg
icon.path = /usr/share/icons/hicolor/scalable/apps

iconPixmaps.files = img/lightningmc.svg
iconPixmaps.path = /usr/share/pixmaps

desktop.files = lightningmc.desktop
desktop.path = /usr/share/applications

INSTALLS += target icon iconPixmaps desktop

Release:DESTDIR = ../lightning-release
Release:OBJECTS_DIR = ../lightning-release/.obj
Release:MOC_DIR = ../lightning-release/.moc
Release:RCC_DIR = ../lightning-release/.rcc
Release:UI_DIR = ../lightning-release/.ui


Debug:DESTDIR = ../lightning-debug
Debug:OBJECTS_DIR = ../lightning-debug/.obj
Debug:MOC_DIR = ../lightning-debug/.moc
Debug:RCC_DIR = ../lightning-debug/.rcc
Debug:UI_DIR = ../lightning-debug/.ui