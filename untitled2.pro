QT += core
QT -= gui

TARGET = untitled2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ffmpeg_decode.cpp \
    FrameQueue.cpp \
    PacketQueue.cpp \
    PictureQueue.cpp \
    threadsafequeue.cpp \
    utiltool.cpp \
    videostate.cpp

HEADERS += \
    ffmpeg_decode.h \
    FrameQueue.h \
    PacketQueue.h \
    PictureQueue.h \
    threadsafequeue.h \
    utiltool.h \
    videostate.h

include($$PWD/opencv_config.pri)

LIBS += -L$(DEPENDS)/ffmpeg/x64/lib -lavformat \
     -lavutil \
     -lavfilter \
     -lavcodec \
     -lavdevice \
     -lswscale \
     -lswresample

INCLUDEPATH += $(DEPENDS)/ffmpeg/x64/include



INCLUDEPATH += $(DEPENDS)/pthreads-w32-2-9-1-release/Pre-built.2/include
LIBS += -L$(DEPENDS)/pthreads-w32-2-9-1-release/Pre-built.2/lib/x64 -lpthreadVC2


