#include "videostate.h"


VideoState::VideoState():pkt_queue(NULL),frame_queue(NULL),picture_queue(NULL)
{

}
VideoState::~VideoState()
{
    if(pkt_queue)
    {
        delete pkt_queue;
    }
    if(frame_queue)
    {
        delete frame_queue;
    }
    if(picture_queue)
    {
        delete picture_queue;
    }

}
