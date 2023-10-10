#ifndef HWQUEUE_H
#define HWQUEUE_H

#include <atomic>

template <typename T>
class HWQueue {
public:
    int counter ;
    HWQueue();
    void enq(T* x);
    T* deq();

private:
    static const int CAPACITY = 1024;
    std::atomic<T*> items[CAPACITY];
    std::atomic<int> tail;
};

#endif 
