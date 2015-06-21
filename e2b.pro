#-------------------------------------------------
#
# Project created by QtCreator 2014-08-30T14:36:28
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = e2b
TEMPLATE = app

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR=build

DESTDIR = bin


SOURCES += main.cpp\
        mainwindow.cpp \
    eagle.cpp \
    eagle_m.cpp \
    eaglercloader.cpp \
    graphicsitems.cpp \
    zoomablegraphicsview.cpp \
    layercolorlookup.cpp

HEADERS  += mainwindow.h \
    eagle.h \
    eagle_m.h \
    eaglercloader.h \
    graphicsitems.h \
    zoomablegraphicsview.h \
    layercolorlookup.h

FORMS    += mainwindow.ui
