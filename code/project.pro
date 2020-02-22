#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T21:28:29
#
#-------------------------------------------------

QT       += core gui

QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app
RC_FILE = title.rc
VERSION = 1.0.0.0

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialog.cpp \
    note.cpp \
    login.cpp

HEADERS += \
        mainwindow.h \
    dialog.h \
    note.h \
    login.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    note.ui \
    login.ui

RESOURCES += \
    imageico.qrc
