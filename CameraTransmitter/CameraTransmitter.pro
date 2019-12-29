QT       += core gui multimedia network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../../libraries/opencv/build/include
INCLUDEPATH += ../../libraries/opencv/build/include/opencv2

LIBPATH += ../../libraries/opencv/build/x64/vc15/lib

LIBS += opencv_world412.lib
LIBS += opencv_world412d.lib


SOURCES += \
    CameraTransmitter.cpp \
    CaptureEngine.cpp \
    CaptureManager.cpp \
    Coder.cpp \
    TransmitManager.cpp \
    Transmitter.cpp \
    main.cpp

HEADERS += \
    CameraTransmitter.h \
    CaptureEngine.h \
    CaptureManager.h \
    Coder.h \
    TransmitManager.h \
    Transmitter.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
