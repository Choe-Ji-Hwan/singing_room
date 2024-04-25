#ifndef SINGINGROOM_SYNCHRONIZEDQUEUE_H
#define SINGINGROOM_SYNCHRONIZEDQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class SynchronizedQueue {
public:
    void enqueue(const T &value) {
        std::lock_guard<std::mutex> lock(mMutex);
        mQueue.push(value);
        mConditionVariable.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mMutex);
        mConditionVariable.wait(lock, [this] { return !mQueue.empty(); });
        T value = mQueue.front();
        mQueue.pop();
        return value;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

private:
    std::queue<T> mQueue;
    mutable std::mutex mMutex;
    std::condition_variable mConditionVariable;
};

#endif //SINGINGROOM_SYNCHRONIZEDQUEUE_H
