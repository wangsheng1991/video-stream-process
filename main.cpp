#include <QCoreApplication>
#include <stdio.h>
#include "ffmpeg_decode.h"
#include "videostate.h"
#include <thread>
#include <string>
#include <iostream>
#include "threadsafequeue.h"
#include "threadsafequeue.cpp"
using std::string;
using std::thread;
void *rec(void *arg);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char *path="rtsp://admin:fl123456@192.168.0.132:554/h264/ch36/main/av_stream";
//    VideoDecode *video_decode = new VideoDecode(path);
//    std::cout << path << std::endl;

//    int ret = video_decode->start();
//    std::cout << ret << std::endl;
//    if(0 != ret)
//    {
//        std::cerr << "video start failed!" << std::endl;
//        return -1;
//    }

    //int count = 0;

    VideoState *vs=new VideoState();
    vs->abort_request=false;

    Ffmpeg_decode *decoder=new Ffmpeg_decode(path);
    bool ret=decoder->init(vs);

    if(!ret)
    {
        return -1;
    }
    std::cout << 2 << std::endl;
    thread read_thread(Ffmpeg_decode::read_packet_thread,(void *)vs);
    thread video_thread(Ffmpeg_decode::video_thread,(void*)vs);
    thread picture_thread(Ffmpeg_decode::picture_thread,(void*)vs);

    thread rec_thread(rec,(void*)vs);

    read_thread.join();
    video_thread.join();
    picture_thread.join();
    rec_thread.join();
//    while(video_decode->vs->picture_queue->size() > 0)
//    {
//        IplImage *img = video_decode->getPicture();

//        QString img_name = "./image_" + QString::number(count) + ".jpg";
//        cvSaveImage(img_name.toStdString().c_str(),img);
//        cvShowImage(img_name.toStdString().c_str(), img);
//        cvWaitKey();
//        cvDestroyWindow(img_name.toStdString().c_str());
//    }

//    此处设置关闭信号：
//    video_decode->vs->abort_request = true;

    return a.exec();
}
void *rec(void *arg)
{
    VideoState *vs=(VideoState*)arg;

    int count=0;

    while(1)
    {
        if(vs->abort_request)
        {
            break;
        }
        IplImage *img=NULL;
        vs->picture_queue->pop_front(&img);

        if(NULL==img)
        {
            continue;
        }
        QString img_name = "./image_" + QString::number(count) + ".jpg";
        cvSaveImage(img_name.toStdString().c_str(),img);
        cvShowImage(img_name.toStdString().c_str(), img);
        cvWaitKey();
        cvDestroyWindow(img_name.toStdString().c_str());
        cvReleaseImage(&img);
        ++count;

    }

    return NULL;
}
