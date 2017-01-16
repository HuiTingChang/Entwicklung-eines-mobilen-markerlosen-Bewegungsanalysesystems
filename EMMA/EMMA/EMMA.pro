#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T15:08:17
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EMMA
TEMPLATE = app

debug { DESTDIR = $${OUT_PWD}/debug }
release { DESTDIR = $${OUT_PWD}/release }
debug_and_release { DESTDIR = $${OUT_PWD}/bin }

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        BalanceBoard.cpp \
        ApplicationData.cpp \
        BalanceBoardThread.cpp \
        body/body_widget.cpp \
        body/camera.cpp \
        body/cvCapture.cpp \
        body/cvConverter.cpp \
        body/qimagelabel.cpp

HEADERS  += \
        BalanceBoard.h \
        ApplicationData.h \
        BalanceBoardThread.h \
        body/body_widget.h \
        body/camera.h \
        body/cvCapture.h \
        body/cvConverter.h \
        body/qimagelabel.h \
        body/util.h

FORMS    += emma.ui

INCLUDEPATH += ../../inc/wiiuse/src \
	body

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

win32:INCLUDEPATH += "$$(KINECTSDK20_DIR)/inc"\
        $$(OPENCV_DIR)/build/include

# set environment variable ARC to x86 if you need
# in all other cases we use x64
ARC = $$(ARC)
!equals(ARC, x86):ARC = x86

OPENCV_VER = $$(OPENCV_VER)

equals(OPENCV_VER, 310) {
        OPENCV_LIBDIR = $$(OPENCV_DIR)/build/$${ARC}/vc14/lib
} else {
        OPENCV_LIBDIR = $$(OPENCV_DIR)/build/$${ARC}/vc12/lib
}

debug {
        LIB_SUFFIX = d
} else {
        LIB_SUFFIX = ""
}

equals(OPENCV_VER, 310) {
        win32:LIBS += "$$(KINECTSDK20_DIR)/Lib/$${ARC}/Kinect20.lib"\
                $${OPENCV_LIBDIR}/opencv_world$$(OPENCV_VER)$${LIB_SUFFIX}.lib
} else {
        win32:LIBS += "$$(KINECTSDK20_DIR)/Lib/$${ARC}/Kinect20.lib"\
                $${OPENCV_LIBDIR}/opencv_calib3d$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_contrib$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_core$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_features2d$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_flann$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_gpu$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_highgui$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_imgproc$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_legacy$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_ml$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_nonfree$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_objdetect$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_ocl$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_photo$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_stitching$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_superres$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_ts$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_video$$(OPENCV_VER)$${LIB_SUFFIX}.lib\
                $${OPENCV_LIBDIR}/opencv_videostab$$(OPENCV_VER)$${LIB_SUFFIX}.lib
}
