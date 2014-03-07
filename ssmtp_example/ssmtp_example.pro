#-------------------------------------------------
#
# Project created by QtCreator 2014-03-06T22:38:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ssmtp_example
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/mimemessage.cpp \
    ../src/ssmtp.cpp

HEADERS  += mainwindow.h \
    ../src/mimemessage.h \
    ../src/ssmtp.h

FORMS    += mainwindow.ui
