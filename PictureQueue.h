#pragma once
#include <queue>
#include <mutex>
#include "opencv2/opencv.hpp"
using std::queue;
class PictureQueue
{
public:
    const int queue_max_size;

public:
    PictureQueue(const int &maxSize);
	~PictureQueue();
    PictureQueue(const PictureQueue &other);
    PictureQueue& operator=(const PictureQueue &)=delete;
	void push(IplImage *pic);
	int pop_front(IplImage *pic);
	IplImage* front();
    int size();
private:
	queue<IplImage*> picture_queue;
    mutable std::mutex m;
};

