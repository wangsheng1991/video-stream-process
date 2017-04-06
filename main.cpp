#include <QCoreApplication>
#include "ffmpeg_decode.h"
#include "videostate.h"
#include <thread>
#include <libgearman/gearman.h>
#include <hiredis/hiredis.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "threadsafequeue.h"
#include "threadsafequeue.cpp"
using std::string;
using std::thread;
struct TaskContext{
    redisContext *c;
    VideoState *vs;
    string task_str;
};

static void *do_video_process(gearman_job_st *job,void *context,size_t *result_size,gearman_return_t *ret_ptr);
static void *checkQuit(void *arg);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //char *input="rtsp://192.168.1.147:554/stream0/admin/admin";

    gearman_worker_st worker;
    if(NULL==gearman_worker_create(&worker))
    {
        std::cerr<<"memory allocation failure on worker creation"<<std::endl;
        return -1;
    }

    string host="192.168.0.120";
    int port=4730;
    gearman_return_t ret;
    ret=gearman_worker_add_server(&worker,host.c_str(),port);

    if(GEARMAN_SUCCESS!=ret)
    {
        std::cerr<<gearman_worker_error(&worker)<<std::endl;
        return -1;
    }

    ret=gearman_worker_add_function(&worker,"video_stream_process",0,do_video_process,NULL);
    if(GEARMAN_SUCCESS!=ret)
    {
        std::cerr<<gearman_worker_error(&worker)<<std::endl;
        return -1;
    }

    std::cerr<<"pid : "<<getpid()<<std::endl;
    while(1)
    {
        ret=gearman_worker_work(&worker);
        if(GEARMAN_SUCCESS!=ret)
        {
            std::cerr<<gearman_worker_error(&worker)<<std::endl;
            break;
        }
       // std::cerr<<"one work done"<<std::endl;

    }
    gearman_worker_free(&worker);

    return a.exec();
}

bool isDeleteTask(redisContext *c ,string taskStr)
{

    bool result=true;
     redisReply *reply;
     reply=(redisReply*)redisCommand(c,"GET %s",taskStr.c_str());


     if(NULL!=reply&&REDIS_REPLY_NIL==reply->type)
     {

         result=false;

     }
     if(NULL==reply)
     {
         std::cerr<<"execute redis command failed"<<std::endl;
     }else{
         freeReplyObject(reply);
     }

     if(result){
         redisCommand(c,"DEL %s",taskStr.c_str());
     }
     std::cerr<<"to be delete mission:"<<taskStr<<"==>"<<result<<std::endl;

    return result;
}


void *checkQuit(void *arg)
{
    TaskContext *tc=(TaskContext *)arg;
    string taskStr=tc->task_str;
    VideoState *vs=tc->vs;
    while(true)
    {

        sleep(10);
        if(vs->abort_request)
        {
            break;
        }
        if(isDeleteTask(tc->c,taskStr))
        {
            vs->abort_request=true;
            break;
        }

    }


    return NULL;

}
void *dispatchRecognize(void *arg)
{
     VideoState *vs=(VideoState*)arg;

     gearman_client_st client;
     if(NULL==gearman_client_create(&client))
     {
         std::cerr<<"memeory allocation failure on client creation"<<std::endl;
         vs->abort_request=true;
         return NULL;
     }
     char *host="127.0.0.1";
     int port=4730;
     gearman_return_t ret=gearman_client_add_server(&client,host,port);

     if(GEARMAN_SUCCESS!=ret)
     {
         std::cerr<<"add server failed"<<std::endl;
         return NULL;
     }

     while(true)
     {

         if(vs->abort_request)
         {
             break;
         }
         string key;
         vs->recognize_task->wait_and_pop(key);
         gearman_job_handle_t job_handle;
         gearman_client_do_background(&client,"ocr_recognize",NULL,key.c_str(),(size_t)key.size(),job_handle);
     }
    return NULL;
}
 void *do_video_process(gearman_job_st *job,void *context,size_t *result_size,gearman_return_t *ret_ptr)
 {

     (void )context;
     const char *workload;
     workload=(const char *)gearman_job_workload(job);
     *result_size=gearman_job_workload_size(job);
    unsigned int size=*result_size;
    string loadData=workload;
    string jobData=loadData.substr(0,size);

    std::cerr<<"job: "<<gearman_job_handle(job)<<std::endl;
    std::cout<<"job data:"<<jobData<<std::endl;
    std::cerr<<"job size:"<<size<<std::endl;
     Ffmpeg_decode *decoder=new Ffmpeg_decode(jobData);
    VideoState *vs= new VideoState();
    vs->abort_request=false;
    vs->url=jobData;
    vs->fps=5;
    redisContext *c=redisConnect("127.0.0.1",6379);
    if(0!=c->err){
        redisFree(c);
        delete vs;
        delete decoder;

        std::string  result="one done";
        *result_size=result.size();
        *ret_ptr=GEARMAN_SUCCESS;
        return strdup(result.c_str());
    }
    TaskContext tc;
    tc.vs=vs;
    tc.task_str=jobData;
    tc.c=c;
    bool ret=decoder->init(vs);

     if(!ret)
     {
         std::cerr<<"decoder init failed "<<std::endl;
     }else{
         thread read_thread(Ffmpeg_decode::read_packet_thread,(void *)vs);
         thread video_thread(Ffmpeg_decode::video_thread,(void *)vs);
         thread picture_thread(Ffmpeg_decode::picture_thread,(void *)vs);
        thread check_thread(checkQuit,(void *)&tc);

        check_thread.join();
         read_thread.join();
         video_thread.join();
         picture_thread.join();

     }
      delete vs;
     redisFree(c);
     delete decoder;

     std::cerr<<"worker done"<<std::endl;
    std::string  result="one done";
    *result_size=result.size();
    *ret_ptr=GEARMAN_SUCCESS;
    return strdup(result.c_str());
 }

