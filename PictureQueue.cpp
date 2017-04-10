#include "PictureQueue.h"


PictureQueue::PictureQueue(const int &maxSize):queue_max_size(maxSize)
{
}

PictureQueue::PictureQueue(const PictureQueue &other):queue_max_size(other.queue_max_size)
{
    std::lock_guard<std::mutex> lock(m);
    picture_queue=other.picture_queue;
}

PictureQueue::~PictureQueue()
{
    if(picture_queue.size()>0)
    {
        IplImage *img=NULL;
        for(int i=0;i<picture_queue.size();i++)
       {
            img=picture_queue.front();
            picture_queue.pop();
            cvReleaseImage(&img);
        }
    }
}
void PictureQueue::push(IplImage *pic)
{
    std::lock_guard<std::mutex> lock(m);
    if(picture_queue.size()>=queue_max_size)
    {
        return ;
    }
	picture_queue.push(pic);

}

int PictureQueue::pop_front(IplImage **pic)
{
    std::lock_guard<std::mutex> lock(m);
//    std::unique_lock<std::mutex> lock(m);
//    data_cond.wait(lock, [this]{return !picture_queue.empty();});
    if (0==picture_queue.size())
    {
        return -1;
    }
    *pic = picture_queue.front();
	picture_queue.pop();

	return 0;

}
IplImage *PictureQueue::front()
{
    std::lock_guard<std::mutex> lock(m);
	return picture_queue.front();
}

int PictureQueue::size()
{
    std::lock_guard<std::mutex> lock(m);
    return picture_queue.size();
}
