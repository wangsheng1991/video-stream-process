QT += core
QT -= gui

TARGET = video_stream_process
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include($$PWD/opencv_config.pri)

SOURCES += main.cpp \
    FrameQueue.cpp \
    PacketQueue.cpp \
    PictureQueue.cpp \
    ffmpeg_decode.cpp \
    utiltool.cpp \
    videostate.cpp \
    threadsafequeue.cpp \
    videodecode.cpp

HEADERS += \
    ffmpeg_decode.h \
    FrameQueue.h \
    PacketQueue.h \
    PictureQueue.h \
    videostate.h \
    utiltool.h \
    threadsafequeue.h \
    videodecode.h



#unix:!macx: LIBS += -L/usr/local/ffmpeg/lib -lavformat \
#    -lavutil \
#    -lavfilter \
#    -lavcodec \
#    -lavdevice \
#    -lswscale \
#    -lswresample

LIBS += -LF:/depends/ffmpeg/x64/lib -lavformat \
     -lavutil \
     -lavfilter \
     -lavcodec \
     -lavdevice \
     -lswscale \
     -lswresample
INCLUDEPATH += F:/depends/ffmpeg/x64/include


#INCLUDEPATH += /usr/local/include
#DEPENDPATH += /usr/local/include

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += /usr/local/lib/pkgconfig/opencv.pc
#unix:PKGCONFIG +=/usr/local/lib/pkgconfig/hiredis.pc


INCLUDEPATH += F:/depends/pthreads-w32-2-9-1-release/Pre-built.2/include
LIBS += -LF:/depends/pthreads-w32-2-9-1-release/Pre-built.2/lib/x64 -lpthreadVC2

