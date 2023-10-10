#include "CLQ.h"

template <typename T>
LockBasedQueue<T>::LockBasedQueue(int capacity) : head(0), tail(0), items(capacity) {}

template <typename T>
void LockBasedQueue<T>::enq(T x) {
    mutex.lock() ;
    if (tail - head == items.size()) {
        mutex.unlock() ;
        //throw std::runtime_error("Queue is full");
    }
    items[tail % items.size()] = x;
    tail++;
    mutex.unlock() ;
}

template <typename T>
T LockBasedQueue<T>::deq() {
    mutex.lock() ;
    if (tail == head) {
        mutex.unlock() ;
        //throw std::runtime_error("Queue is empty");
    }
    T x = items[head % items.size()];
    head++;
    mutex.unlock() ;
    return x;
}

