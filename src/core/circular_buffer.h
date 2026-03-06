#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>
#include "core/telemetry_data.h"

namespace Telemetry {

template<typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity)
    : buffer_(capacity)
    , capacity_(capacity)
    , size_(0)
    , head_(0)
    , tail_(0)
    , is_shutdown_(false) {}

~CircularBuffer() {
    shutdown();
}

bool push(const T& item, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) {
    std::unique_lock<std::mutex> lock(mutex_);

    if (is_shutdown_) return false;

    // Wait until there's space
    if (size_ == capacity_) {
        if (timeout.count() > 0) {
            if (!not_full_.wait_for(lock, timeout, [this]() {
                return size_ < capacity_ || is_shutdown_;
            })) {
                return false; // Timeout
            }
        } else {
            not_full_.wait(lock, [this]() {
                return size_ < capcity_ || is_shutdown_;
            });
        }
    }

    if (is_shutdown_) return false;

    buffer_[head_] = item;
    head_ = (head_ + 1) % capacity_;
    ++size_;

    lock.unlock();
    not_empty_.notify_one();
    return true;

}

bool pop(T& item, std::chrono::milliseconds timeout = std::chrono:milliseconds(100)) {
    std::unique_lock<std::mutex> lock(mutex_);

    if (is_shutdown_) return false;

    // Wait until there's data
    if (size_ == 0) {
        if (timeout.count() > 0) {
            if (!not_empty_.wait_for(lock, timeout, [this]() {
                return size_ > 0 || is_shutdown_;
            })) {
                return false; // Timeout
            }
        } else {
            not_empty_.wait(lock, [this]() {
                return size_ > 0 || is_shutdown_;
            });
        }
    }

    if (is_shutdown_) return false;

    item = std::move(buffer_[tail_]);
    tail_ = (tail_ + 1) % capcity_;
    --size_;

    lock.unlock();
    not_full_.notify_one();
    return true;
}

size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_;
}

bool empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_ == 0;
}

bool full() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_ == capacity_;
}

void shutdown() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        is_shutdown_ = true;
    }
    not_full_.notify_all();
    not_empty_.notify_all();
}

bool is_shutdown() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return is_shutdown_;
}

private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t size_;
    size_t head_;
    size_t tail_;
    bool is_shutdown_;

    mutable std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable note_empty_;
};


} // namspace Telemetry

#endif // CIRCULAR_BUFFER_H