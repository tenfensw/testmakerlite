#-------------------------------------------------
#
# Project created by QtCreator 2019-04-02T20:21:22
#
#-------------------------------------------------

TESTMAKERLITE_VERSION_MAJOR = 1
TESTMAKERLITE_VERSION_MINOR = 6
TESTMAKERLITE_VERSION_UPDATE = 0

QT       += core gui network

DEFINES += TESTMAKERLITE_VERSION_MAJOR=$$TESTMAKERLITE_VERSION_MAJOR
DEFINES += TESTMAKERLITE_VERSION_MINOR=$$TESTMAKERLITE_VERSION_MINOR
DEFINES += TESTMAKERLITE_VERSION_UPDATE=$$TESTMAKERLITE_VERSION_UPDATE

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testmakerlite
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
