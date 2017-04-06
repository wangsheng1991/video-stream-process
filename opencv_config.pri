#CONFIG += opencv2.4
CONFIG += opencv3.0

opencv2.4{
    OPENCV_VERSION = 2.4.11
    OPENCV_PATH = E:/opencv/opencv2.4.11/build
    OPENCV_LIB_PATH = $$OPENCV_PATH/x64/vc12/lib

    INCLUDEPATH += $$OPENCV_PATH/include
    DEPENDPATH  += $$OPENCV_PATH/include

#win32{
    CONFIG(release, debug|release){
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_core2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_highgui2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_imgproc2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ml2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_objdetect2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ts2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_video2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_nonfree2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ocl2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_photo2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_stitching2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_superres2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_videostab2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_calib3d2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_contrib2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_features2d2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_flann2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_gpu2411
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_legacy2411
    } else {
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_core2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_highgui2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_imgproc2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ml2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_objdetect2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ts2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_video2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_nonfree2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ocl2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_photo2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_stitching2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_superres2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_videostab2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_calib3d2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_contrib2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_features2d2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_flann2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_gpu2411d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_legacy2411d
    }
#}

    #unix{
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_core
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_highgui
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_imgproc
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_imgcodecs
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_ml
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_objdetect
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_ts
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_video
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_nonfree
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_ocl
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_photo
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_stitching
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_superres
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_videostab
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_calib3d
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_contrib
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_features2d
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_flann
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_gpu
    #    LIBS += -L$$OPENCV_LIB_PATH -lopencv_legacy
    #}
}#opencv2.4


opencv3.0{
    OPENCV_VERSION = 3.0.0
    OPENCV_PATH = F:/depends/opencv3.0.0/opencv/build
    OPENCV_LIB_PATH = $$OPENCV_PATH/x64/vc12/lib

    INCLUDEPATH += $$OPENCV_PATH/include
    DEPENDPATH  += $$OPENCV_PATH/include

    CONFIG(release, debug|release){
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ts300
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_world300
    }else{
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_ts300d
        LIBS += -L$$OPENCV_LIB_PATH -lopencv_world300d
    }
}#opencv3.0
