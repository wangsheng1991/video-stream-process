#include "FrameQueue.h"


FrameQueue::FrameQueue(const int &maxsize):queue_max_size(maxsize)
{
}
FrameQueue::FrameQueue(const FrameQueue &other):queue_max_size(other.queue_max_size)
{
	std::lock_guard<std::mutex> lock(m);
	frame_queue = other.frame_queue;
}


FrameQueue::~FrameQueue()
{
    if(frame_queue.size()>0)
    {
        AVFrame *frame=NULL;
        for(int i=0;i<frame_queue.size();i++)
        {
            frame=frame_queue.front();
            frame_queue.pop();
            av_frame_free(&frame);
        }
    }
}
void FrameQueue::push(AVFrame *frame)
{
	std::lock_guard<std::mutex> lock(m);
	frame_queue.push(frame);
}


int FrameQueue::pop_front(AVFrame **frame)
{

	std::lock_guard<std::mutex> lock(m);
	if (0 == frame_queue.size())
	{
		return -1;
	}
	*frame = frame_queue.front();
	frame_queue.pop();
	return 0;
}
int FrameQueue::pop()
{
    std::lock_guard<std::mutex> lock(m);

    if(0==frame_queue.size())
    {
        return -1;
    }
    frame_queue.pop();
    return 0;
}

AVFrame *FrameQueue::front()
{

	std::lock_guard<std::mutex> lock(m);
    if(0==frame_queue.size())
    {
        return NULL;
    }
	return frame_queue.front();
}

int FrameQueue::size()
{

	std::lock_guard<std::mutex> lock(m);
	return frame_queue.size();
}

