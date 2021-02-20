QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../../Game/statistics.cpp \
    tst_statistics_test.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../Game/statistics.hh \
    ../../Course/CourseLib/interfaces/istatistics.hh

INCLUDEPATH += $$PWD/../Course/CourseLib

DEPENDPATH += $$PWD/../Course/CourseLib
