#ifndef VIDEOSTATE_H 
#define VIDEOSTATE_H
#include "FrameQueue.h"
#include "PacketQueue.h"
#include "PictureQueue.h"
#include "threadsafequeue.h"
#include <string>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

class VideoState{
public:

	AVFormatContext *ifmt_ctx;
	AVCodecContext *codec_ctx;
	PacketQueue *pkt_queue;
	FrameQueue *frame_queue;
	PictureQueue *picture_queue;
    ThreadSafeQueue<std::string> *recognize_task;
	bool abort_request;
    std::string host;
    int port;
    std::string url;
    //
    unsigned int fps;
public:
    VideoState();
    ~VideoState();
};

#endif
