#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T15:08:17
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets \
        printsupport # for QCustomPlot

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


CONFIG += c++11

SOURCES += ../../inc/inih/ini.c \
        ../../inc/inih/cpp/INIReader.cpp \
        ../../inc/QCustomPlot/src/qcustomplot.cpp \
        ../../inc/QtOpenCV/cvmatandqimage.cpp \
        ApplicationData.cpp \
        CurrentState.cpp \
        main.cpp \
        BalanceBoard.cpp \
        BalanceBoardThread.cpp \
	StreamIO.cpp \
        body/body_widget.cpp \
        body/CvCamera.cpp \
        body/cvCapture.cpp \
        body/cvConverter.cpp

HEADERS  += ../../inc/inih/ini.h \
        ../../inc/inih/cpp/INIReader.h \
        ../../inc/QCustomPlot/src/qcustomplot.h \
        ../../inc/QtOpenCV/cvmatandqimage.h \
        Anatomy.h \
        ApplicationData.h \
        CurrentState.h \
        BalanceBoard.h \
        BalanceBoardThread.h \
	StreamIO.h \
        body/body_widget.h \
        body/CvCamera.h \
        body/cvCapture.h \
        body/cvConverter.h

win32 {
    SOURCES += body/KinectCamera.cpp
    HEADERS += body/KinectCamera.h \
            body/util.h
}

FORMS    += emma.ui

INCLUDEPATH += ../../inc/wiiuse/src \
    ../../inc/inih/cpp \
    ../../inc/QCustomPlot/src \
    ../../inc/QtOpenCV \
    body

config_files.path = $${DESTDIR}
config_files.files = emma.ini

INSTALLS += config_files

unix:LIBS += -L$${PWD}/../../inc/wiiuse/build/src -lwiiuse

win32:LIBS += ../../inc/wiiuse/build/src/Debug/wiiuse_debug.lib

win32 {
    wiidll.path = $${DESTDIR}
    wiidll.files = ../../inc/wiiuse/build/src/Debug/wiiuse_debug.dll
    INSTALLS += wiidll
}

win32:INCLUDEPATH += "$$(KINECTSDK20_DIR)/inc"\
        $$(OPENCV_DIR)/build/include

# set environment variable ARC to x86 or x64 if
# it is different from the architecture of the Qt build kit
ARC = $$(ARC)
isEmpty(ARC) {
    ARC = $$QMAKE_HOST.arch
    contains(ARC, x86_64) {
        ARC = x64
    } else {
        ARC = x86
    }
}

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

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
