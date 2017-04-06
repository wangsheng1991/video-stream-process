#pragma once
#include <queue>
#include <mutex>
extern "C"
{
#include "libavcodec/avcodec.h";
#include "libavformat/avformat.h"
}

//#define snprintf _snprintf
using std::queue;
class FrameQueue
{
public:

    const int queue_max_size;

public:


    FrameQueue(const int &maxSize);
	~FrameQueue();
	FrameQueue(const FrameQueue &other);
	FrameQueue &operator=(const FrameQueue &) = delete;
	void push(AVFrame *frame);
	int pop_front(AVFrame **frame);
	AVFrame *front();
    int pop();
	int size();
private:
	std::mutex m;
	queue<AVFrame*> frame_queue;

};

