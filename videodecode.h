#ifndef VIDEODECODE_H
#define VIDEODECODE_H

#include <string>
#include <opencv/highgui.h>
class VideoState;
class Ffmpeg_decode;
class VideoDecode
{
public:
    VideoDecode(const char *videoPath);
    ~VideoDecode();

    /*
     * return:
     * -1 when encount error,
     * 0 means normal,
     * 1 means this function has been called before
     * */
    int start();

    void stop();

    IplImage* getPicture();


public:
    VideoState *vs;
private:
    const std::string path;   
    Ffmpeg_decode *decoder;
    bool hasStarted;
};

#endif // VIDEODECODE_H
