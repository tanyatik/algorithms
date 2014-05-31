#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace tanyatik {

template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mutex_;
    std::queue<std::shared_ptr<T>> queue_;
    std::condition_variable condition_;

public:
    ThreadSafeQueue() {}

    void waitAndPop(T &value) {
        std::unique_lock<std::mutex> lock(mutex_);

        condition_.wait(lock, [this]{return !queue_.empty(); });
        value = std::move(*queue_.front);
        queue_.pop;
    }

    bool tryPop(T *value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        *value = std::move(*queue_.front());
        queue_.pop();
        return true; 
    }

    void push(T value) {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(value)));

        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(data);
        condition_.notify_one();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
};

} // namespace tanyatik
