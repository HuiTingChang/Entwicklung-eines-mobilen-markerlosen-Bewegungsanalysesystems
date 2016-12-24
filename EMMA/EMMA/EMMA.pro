QT += core
QT -= gui

TARGET = EMMA
CONFIG += console
CONFIG -= app_bundle

debug { DESTDIR = $${OUT_PWD}/debug }
release { DESTDIR = $${OUT_PWD}/release }
debug_and_release { DESTDIR = $${OUT_PWD}/bin }

TEMPLATE = app

SOURCES += main.cpp \
    ../b_board.cpp

HEADERS += \
    ../b_board.h\

INCLUDEPATH += ../../inc/wiiuse/src

unix:LIBS += -static -L../../inc/wiiuse/build/src -llibwiiuse.so

win32:LIBS += ../../inc/wiiuse/build/src/Debug/wiiuse_debug.lib

win32 {
    WIIDLL = $${PWD}/../../inc/wiiuse/build/src/Debug/wiiuse_debug.dll
    WIIDLL_TARGET = $$system_path($${DESTDIR}/wiiuse_debug.dll)
    dllcopy.commands = copy $$system_path($$WIIDLL) $$system_path($$DESTDIR)
    dllcopy.target = $$WIIDLL_TARGET
    QMAKE_EXTRA_TARGETS += dllcopy
    PRE_TARGETDEPS += $$WIIDLL_TARGET
}
