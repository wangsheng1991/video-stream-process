#include <QCoreApplication>
#include <stdio.h>
#include "ffmpeg_decode.h"
#include "videostate.h"
#include "videodecode.h"
#include <thread>
#include <string>
#include <iostream>
#include "threadsafequeue.h"
#include "threadsafequeue.cpp"
using std::string;
using std::thread;

int main(int argc, char *argv[])
{

    char *path="rtsp://192.168.1.147:554/stream0/admin/admin";
    VideoDecode *video_decode = new VideoDecode(path);
    int ret = video_decode->start();
    if(0 != ret)
    {
        std::cerr << "video start failed!" << std::endl;
        return -1;
    }
    int count = 0;

    while(video_decode->vs->picture_queue->size() > 0)
    {
        IplImage *img = video_decode->getPicture();

        QString img_name = "./image_" + QString::number(count) + ".jpg";
        cvSaveImage(img_name.toStdString().c_str(),img);
        cvShowImage(img_name.toStdString().c_str(), img);
    }


    return 0;
}
