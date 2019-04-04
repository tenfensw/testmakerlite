#-------------------------------------------------
#
# TestMaker Lite QMake project
#
#-------------------------------------------------

TESTMAKERLITE_VERSION_MAJOR = 1
TESTMAKERLITE_VERSION_MINOR = 6
TESTMAKERLITE_VERSION_UPDATE = 0

DEFINES += TESTMAKERLITE_VERSION_MAJOR=$$TESTMAKERLITE_VERSION_MAJOR
DEFINES += TESTMAKERLITE_VERSION_MINOR=$$TESTMAKERLITE_VERSION_MINOR
DEFINES += TESTMAKERLITE_VERSION_UPDATE=$$TESTMAKERLITE_VERSION_UPDATE

!greaterThan(QT_MAJOR_VERSION, 4) {
	error("TestMaker Lite does not support Qt 4.8 and earlier.")
}

QT       += core gui network widgets

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
        testsystem/main.cpp \
        testsystem/mainwindow.cpp

HEADERS += \
        testsystem/mainwindow.h

FORMS += \
        testsystem/mainwindow.ui
