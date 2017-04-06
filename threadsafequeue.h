/*
 when use template ,please include .h and .cpp of the template in you use place;
*/
#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
template<typename T>
class ThreadSafeQueue
{
public:
    const int queue_max_size;
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    ThreadSafeQueue(const int maxSize);

    ThreadSafeQueue(const ThreadSafeQueue&);
    ThreadSafeQueue& operator=(const ThreadSafeQueue &)=delete;
    bool push(T value);

    void wait_and_pop(T &value);
    int size() const;
    bool empty() const;
};

#endif // THREADSAFEQUEUE_H
