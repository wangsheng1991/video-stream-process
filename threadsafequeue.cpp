#include "threadsafequeue.h"

template<typename T> ThreadSafeQueue<T>::ThreadSafeQueue(const int maxSize):queue_max_size(maxSize)
{

}

template<typename T> ThreadSafeQueue<T>::ThreadSafeQueue(const ThreadSafeQueue &other):queue_max_size(other.queue_max_size)
{
    std::lock_guard<std::mutex> lock(mut);
    data_queue=other.data_queue;
}

template<typename T> bool ThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lock(mut);

    if(data_queue.size()>=queue_max_size)
    {
        return false;
    }
    data_queue.push(value);
    data_cond.notify_one();
    return true;
}

template<typename T> void ThreadSafeQueue<T>::wait_and_pop(T &value)
{
    std::unique_lock<std::mutex> u_lock(mut);
    data_cond.wait(u_lock,[this]{return !data_queue.empty();});
    value=data_queue.front();
    data_queue.pop();

}

template<typename T> int ThreadSafeQueue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mut);
    return data_queue.size();
}

template<typename T> bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mut);
    return data_queue.empty();
}
