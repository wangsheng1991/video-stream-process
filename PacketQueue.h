#pragma once
#include <queue>
#include <mutex>
extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}

//#define snprintf _snprintf
using std::queue;
class PacketQueue{
public:

    const AVMediaType media_type;
    const int queue_max_size;
public:
    PacketQueue(const AVMediaType &mediaType,const int &maxSize);
	~PacketQueue();
	PacketQueue(const PacketQueue &other);
	PacketQueue& operator=(const PacketQueue&) = delete;
    void setStreamIndex(int index);
    int getStreamIndex();
	void push(AVPacket &packet);
	int pop_front(AVPacket **pkt);
	AVPacket *front();
	int size();
private:
    int stream_index;
	queue<AVPacket*> pkt_queue;
	mutable std::mutex m;

};

