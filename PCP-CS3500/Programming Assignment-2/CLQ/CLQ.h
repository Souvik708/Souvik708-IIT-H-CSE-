#ifndef LOCKBASEDQUEUE_H
#define LOCKBASEDQUEUE_H

#include <vector>
#include <mutex>
#include <stdexcept>

template <typename T>
class LockBasedQueue {
public:
    LockBasedQueue(int capacity);
    void enq(T x);
    T deq();

private:
    int head, tail;
    std::vector<T> items;
    std::mutex mutex;
};

#endif

