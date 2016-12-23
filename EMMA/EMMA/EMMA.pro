QT += core
QT -= gui

TARGET = EMMA
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../b_board.cpp

HEADERS += \
    ../b_board.h\

INCLUDEPATH += ../../inc/wiiuse/src

LIBS += -static -L../../inc/wiiuse/build/src -llibwiiuse.so
