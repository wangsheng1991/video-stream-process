#pragma once
#include <string>
#include "opencv/highgui.h"
#include <list>
#include <queue>
#include <hiredis.h>
#include "videostate.h"
extern "C"{
#include "libavutil/imgutils.h"

#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
}
#define __STDC_CONSTANT_MACROS
//#define snprintf _snprintf
using std::string;

class FrameQueue;
class PacketQueue;



class Ffmpeg_decode
{
public:
	Ffmpeg_decode(string input);
	~Ffmpeg_decode();
    bool init(VideoState *vs);
	static void read_packet_thread(void *arg);
	static int decoder_decode_frame(AVCodecContext *codecCtx,PacketQueue *pktQueue,FrameQueue *frameQueue);
	static void video_thread(void *arg);
	static void picture_thread(void *arg);
    static int save_picture(VideoState *vs,unsigned int & index,redisContext *c);

private:
	const string file_name;
	int video_index;
	int video_width;
	int video_height;
	IplImage *img;
	AVFormatContext *pFormatCtx;
	AVStream *pStream;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
    //AVFrame *pFrame;
    //AVFrame *pFrameBGR;
	uint8_t *bufferBGR;
	AVPacket packet;
	
};

