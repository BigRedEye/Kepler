#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T22:57:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kepler
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    planet.cpp \
    camera.cpp \
    renderer.cpp \
    planeteditor.cpp

HEADERS  += widget.h \
    planet.h \
    camera.h \
    renderer.h \
    planeteditor.h

FORMS    += widget.ui \
    planeteditor.ui
