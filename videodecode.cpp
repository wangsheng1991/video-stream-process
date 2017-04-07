#include "videodecode.h"
#include "videostate.h"
#include "ffmpeg_decode.h"
#include <thread>
using std::thread;
VideoDecode::VideoDecode(const char *videoPath):path(videoPath),decoder(NULL),hasStarted(false)
{

    vs=new VideoState();
    vs->abort_request=false;
    vs->url=path;

}

VideoDecode::~VideoDecode()
{

    delete vs;
    if(decoder)
    {
        delete decoder;
    }
}

void VideoDecode::stop()
{
    vs->abort_request=true;
}

int VideoDecode::start()
{

    if(hasStarted)
    {
        return 1;
    }
    decoder=new Ffmpeg_decode(path);

    bool ret=decoder->init(vs);

    if(!ret)
    {
        return -1;
    }

    thread read_thread(Ffmpeg_decode::read_packet_thread,(void *)vs);
    thread video_thread(Ffmpeg_decode::video_thread,(void*)vs);
    thread picture_thread(Ffmpeg_decode::picture_thread,(void*)vs);

    read_thread.detach();
    video_thread.detach();
    picture_thread.detach();

    return 0;

}

IplImage* VideoDecode::getPicture()
{

    IplImage *img=NULL;
    int ret=vs->picture_queue->pop_front(img);
    if(-1==ret)
    {
        return NULL;
    }

    return img;
}
