#include "NLQ.h"

template <typename T>
HWQueue<T>::HWQueue() : tail(0) {
    for (int i = 0; i < CAPACITY; ++i) {
        items[i].store(nullptr);
    }
}

template <typename T>
void HWQueue<T>::enq(T* x) {
    int i = tail.fetch_add(1, std::memory_order_relaxed);
    items[i % CAPACITY].store(x, std::memory_order_relaxed);
}

template <typename T>
T* HWQueue<T>::deq() {
    counter=100 ;
    while (counter--) {
        int range = tail.load(std::memory_order_relaxed);
        for (int i = 0; i < range; ++i) {
            T* value = items[i % CAPACITY].exchange(nullptr, std::memory_order_relaxed);
            if (value != nullptr) {
                return value;
            }
        }
    }
    return nullptr ;
}

