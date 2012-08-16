#-------------------------------------------------
#
# Project created by QtCreator 2012-07-25T14:03:51
#
#-------------------------------------------------

QT       += core gui network

TARGET = ZIMA-PS2PDF
TEMPLATE = app

win32:INCLUDEPATH += ../
VPATH += ./src

SOURCES += \
    src/SettingsDialog.cpp \
    src/AboutDialog.cpp \
    src/Worker.cpp \
    src/MainWindow.cpp \
    src/ZIMA-PS2PDF.cpp

HEADERS += \
    src/SettingsDialog.h \
    src/AboutDialog.h \
    src/Worker.h \
    src/MainWindow.h \
    src/ZIMA-PS2PDF.h

FORMS   += \
    src/SettingsDialog.ui \
    src/AboutDialog.ui \
    src/MainWindow.ui

RESOURCES += \
    ZIMA-PS2PDF.qrc

OTHER_FILES += \
    src/ZIMA-PS2PDF.rc

CODECFORTR = UTF-8
TRANSLATIONS = locale/ZIMA-PS2PDF_cs_CZ.ts