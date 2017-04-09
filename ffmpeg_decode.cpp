#include "ffmpeg_decode.h"
#include <iostream>
#include <QString>
#include "FrameQueue.h"
#include "PacketQueue.h"
#include "PictureQueue.h"
#include "threadsafequeue.h"
#include "threadsafequeue.cpp"
#include "utiltool.h"
#define GRAB_RGB24
Ffmpeg_decode::Ffmpeg_decode(string input) :file_name(input)
{
	av_register_all();
	avformat_network_init();
    pCodecCtx=NULL;

}

Ffmpeg_decode::~Ffmpeg_decode()
{
	avformat_network_deinit();
    if(pCodecCtx)
    {
        avcodec_close(pCodecCtx);
        av_free(pCodecCtx);
    }
}

void Ffmpeg_decode::read_packet_thread(void *arg)
{
	VideoState *vs = (VideoState *)arg;

	int ret = -1;
	AVPacket pkt;
	while (true)
	{
		if (vs->abort_request)
		{
            std::cerr<<"read packet abort"<<std::endl;
			break;
		}

        if(vs->pkt_queue->size()>=vs->pkt_queue->queue_max_size)
        {

            //wait 10 millseconds
            av_usleep(10000);
            continue;
        }

		ret = av_read_frame(vs->ifmt_ctx, &pkt);
        if (ret == AVERROR(EAGAIN)) {
                    av_usleep(10000);
                    continue;
        }
		if (ret < 0)
		{
			continue;
		}

        if (pkt.stream_index!= vs->pkt_queue->getStreamIndex())
		{
            av_packet_unref(&pkt);
			continue;
		}

		vs->pkt_queue->push(pkt);
		av_packet_unref(&pkt);
	}
	av_packet_unref(&pkt);
	
}

void Ffmpeg_decode::video_thread(void *arg)
{
	VideoState *vs = (VideoState*)arg;
	int ret = -1;
	while (true)
	{
		if (vs->abort_request)
		{
            std::cerr<<"video thread abort"<<std::endl;
			break;
		}
		ret=decoder_decode_frame(vs->codec_ctx, vs->pkt_queue, vs->frame_queue);
		
	}

}

int Ffmpeg_decode::decoder_decode_frame(AVCodecContext *codecCtx, PacketQueue *pktQueue, FrameQueue *frameQueue)
{

	AVFrame *decodeFrame = av_frame_alloc();
	AVPacket *pkt = NULL;
	int ret = -1, got_frame = -1;
	do
	{

		ret = pktQueue->pop_front(&pkt);
		if (ret < 0)
		{
			break;
		}
        if (NULL == pkt)
		{
			continue;
		}
		switch (pktQueue->media_type)
		{

		case AVMEDIA_TYPE_VIDEO:

			ret = avcodec_decode_video2(codecCtx, decodeFrame, &got_frame, pkt);

			break;
		case AVMEDIA_TYPE_AUDIO:

			break;
        default:
            break;

		}
		av_packet_free(&pkt);
		if (0 != got_frame)
		{
            //when frame queue is full ,drop frame
            if(frameQueue->size()>=frameQueue->queue_max_size)
            {
                break;
            }
			frameQueue->push(decodeFrame);
		}
		
	} while (!got_frame);

	if (ret < 0)
	{
		av_frame_free(&decodeFrame);
	}
	

	return got_frame;

}

void Ffmpeg_decode::picture_thread(void *arg)
{
	VideoState *vs = (VideoState*)arg;
	int ret = -1;
    unsigned int index = 0;

#ifdef GRAB_RGB24
    struct SwsContext* imgCtx = NULL;
    int width = vs->codec_ctx->width;
    int height = vs->codec_ctx->height;
    imgCtx = sws_getCachedContext(imgCtx,width, height, vs->codec_ctx->pix_fmt, width, height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

//    AVFrame *pFrameBGR=NULL;
//    pFrameBGR = av_frame_alloc();
//    IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
//    av_image_fill_arrays(pFrameBGR->data, pFrameBGR->linesize, (uint8_t *)img->imageData, AV_PIX_FMT_BGR24, width, height, 1);
#endif

	while (true)
	{
		if (vs->abort_request)
		{
            std::cerr<<"picture_thread abort"<<std::endl;
			break;
		}

#ifdef GRAB_RGB24
        //这些指针如何释放的问题？ push到picture_queue的在这里面不能释放。
        AVFrame *pFrameBGR=NULL;
        pFrameBGR = av_frame_alloc();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        av_image_fill_arrays(pFrameBGR->data, pFrameBGR->linesize, (uint8_t *)img->imageData, AV_PIX_FMT_BGR24, width, height, 1);

        AVFrame *frame=NULL;
        int ret=vs->frame_queue->pop_front(&frame);
        if(ret<0)
        {
            continue;
        }
        sws_scale(imgCtx, (const uint8_t *const *)frame->data, frame->linesize, 0, frame->height, pFrameBGR->data, pFrameBGR->linesize);
        //char img_name[255];
        //snprintf(img_name, 255, "./image_%ld.jpg", index);
        QString img_name = "./image_" + QString::number(index) + ".jpg";
        cvSaveImage(img_name.toStdString().c_str(),img);
        if((index % vs->fps ==0) && (vs->picture_queue->size() < vs->picture_queue->queue_max_size))
        {
            vs->picture_queue->push(img);
        }
        else
        {
            //释放不再需要的临时指针，那以后的指针指针怎么办？处理一张释放一张?
            av_frame_free(&pFrameBGR);
            av_frame_free(&frame);
            cvReleaseImage(&img);

        }
#else
//        ret = save_picture(vs, index,c);

//        if (ret < 0)
//        {
//            continue;
//        }

//        vs->frame_queue->pop();
#endif
        ++index;
	}



}


bool Ffmpeg_decode::init(VideoState *vs)
{
	pFormatCtx = avformat_alloc_context();

	int err = avformat_open_input(&pFormatCtx, file_name.c_str(), NULL, NULL);
	if (err < 0)
	{
        return false;
	}
	err = avformat_find_stream_info(pFormatCtx, NULL);
	if (err < 0)
	{
        return false;
	}
	
	video_index = -1;
	int i;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_index = i;
			break;
		}
	}
	if (video_index == -1){
        return false;
	}
	pStream = pFormatCtx->streams[video_index];
	pCodecCtx = pFormatCtx->streams[video_index]->codec;
	video_height = pCodecCtx->height;
	video_width = pCodecCtx->width;
	pCodecCtx->thread_count = 10;
	pCodecCtx->thread_type = FF_THREAD_FRAME;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if ( NULL==pCodec )
	{
        return false;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
        return false;
	}
	vs->ifmt_ctx = pFormatCtx;
	vs->abort_request = false;
	vs->codec_ctx = pCodecCtx;
    vs->frame_queue = new FrameQueue(100);
    vs->picture_queue = new PictureQueue(100);
    vs->pkt_queue = new PacketQueue(AVMEDIA_TYPE_VIDEO,100);
    vs->pkt_queue->setStreamIndex(video_index);
    vs->recognize_task=new ThreadSafeQueue<string>(100);
    vs->fps = 5;
    return true;
}

//int Ffmpeg_decode::save_picture(VideoState *vs,unsigned int &index,redisContext *c)
//{

//    FrameQueue *frameQueue=vs->frame_queue;
//	if (0 == frameQueue->size())
//	{
//		return -1;
//	}
//	int ret = -1;
//	AVFrame *frame=NULL;
////	ret = frameQueue->pop_front(&frame);
////	if (ret < 0)
////	{
////		return ret;
////    }
//    frame=frameQueue->front();
//    if(NULL==frame)
//    {
//        return -1;
//    }
//    if(index%5!=0)
//    {
//        return 0;
//    }
//    index=0;
//    std::cerr<<"url:"<<vs->url<<std::endl;

//    string uuid=UtilTool::getUuid();
//    string key=uuid+"_"+vs->url;

//    //add imgdata info ,include width, height, chanel
//    double start=cvGetTickCount();

//    redisReply *reply=(redisReply*)redisCommand(c,"HMSET %s width %d height %d chanel %d imgData %b",key.c_str(),frame->width,frame->height,1,(char *)frame->data[0],(size_t)frame->width*frame->height);

//    av_frame_free(&frame);
//    double end=cvGetTickCount();
//    double time=(end-start)/(cvGetTickFrequency()*1000);
//    std::cerr<<"save time:"<<time<<"ms"<<std::endl;
//    //process the problem of redis error,abort this process?
//   if(NULL==reply)
//   {
//        vs->abort_request=true;
//        return 1;
//   }

//   switch(reply->type)
//   {
//       case REDIS_REPLY_ERROR:

//        std::cerr<<"error===>"<<reply->str<<std::endl;
//       break;
//   case REDIS_REPLY_STATUS:

//       /*
//        *
//        * */
//        //to process when redis command is OK
//       if(0==strncmp(reply->str,"OK",2))
//       {
//               //std::cerr<<"key=="<<key<<std::endl;
//              // vs->abort_request=true;
//               vs->recognize_task->push(key);
//        }
//       break;
//   default:
//       std::cerr<<"default"<<std::endl;
//       break;
//   }

//   freeReplyObject(reply);
//	return 0;
//}
