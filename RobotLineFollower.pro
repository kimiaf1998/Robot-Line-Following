#-------------------------------------------------
#
# Project created by QtCreator 2018-05-05T21:06:58
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += serialport

TARGET = RobotLineFollower
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qrscanner.cpp \
    robot.cpp \
    capture.cpp \
    detector.cpp \
    processor.cpp

HEADERS += \
    qrscanner.h \
    robot.h \
    status.h \
    capture.h \
    detector.h \
    processor.h \
    data_type.h
CONFIG += thread
CONFIG += extserialport
LIBS += `pkg-config opencv --libs`

#INCLUDEPATH+= "/home/kimia/opencv-zbar/build"

#include(/home/kimia/qextserialport-master/src/qextserialport.pri)
