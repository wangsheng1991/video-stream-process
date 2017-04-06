#include "PacketQueue.h"


PacketQueue::PacketQueue(const AVMediaType &mediaType,const int &maxSize):media_type(mediaType),queue_max_size(maxSize)
{

}
PacketQueue::PacketQueue(const PacketQueue &other):media_type(other.media_type),queue_max_size(other.queue_max_size)
{
	std::lock_guard<std::mutex> lock(m);
	pkt_queue = other.pkt_queue;
    setStreamIndex(other.stream_index);
}

PacketQueue::~PacketQueue()
{
    if(pkt_queue.size()>0)
    {
        AVPacket *pkt;
        for(int i=0;i<pkt_queue.size();i++)
        {
            pkt=pkt_queue.front();
            pkt_queue.pop();
            av_packet_unref(pkt);
        }
    }

}
void PacketQueue::setStreamIndex(int index)
{
    this->stream_index=index;
}
int PacketQueue::getStreamIndex()
{
    std::lock_guard<std::mutex> lock(m);
    return stream_index;
}

void PacketQueue::push(AVPacket &packet)
{
	std::lock_guard<std::mutex> lock(m);
	AVPacket *pkt;// = av_packet_alloc();
	//*pkt = packet;
	pkt=av_packet_clone(&packet);
	pkt_queue.push(pkt);
}

AVPacket *PacketQueue::front()
{
	std::lock_guard<std::mutex> lock(m);
	return pkt_queue.front();
}


int PacketQueue::pop_front(AVPacket **pkt)
{
	std::lock_guard<std::mutex> lock(m);
	if (0 == pkt_queue.size())
	{
		return -1;
	}
	*pkt = pkt_queue.front();
	pkt_queue.pop();
	return 0;
}
int PacketQueue::size()
{
	std::lock_guard<std::mutex> lock(m);
	return pkt_queue.size();
}
