#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>
#include <cstddef>
#include <utility>

namespace Telemetry {

/**
 *  A simple single‑threaded circular buffer.  No locking, no timeouts,
 *  no shutdown support – callers are responsible for synchronisation if
 *  they ever use it from multiple threads.
 */
template<typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(std::size_t capacity)
        : buffer_(capacity),
          capacity_(capacity),
          size_(0),
          head_(0),
          tail_(0)
    {}

    ~CircularBuffer() = default;

    /// Add item if room; return false when full.
    bool push(const T &item) {
        if (size_ == capacity_) {
            return false;
        }
        buffer_[head_] = item;
        head_ = (head_ + 1) % capacity_;
        ++size_;
        return true;
    }

    /// Remove front item into `item`; return false when empty.
    bool pop(T &item) {
        if (size_ == 0) {
            return false;
        }
        item = std::move(buffer_[tail_]);
        tail_ = (tail_ + 1) % capacity_;
        --size_;
        return true;
    }

    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    bool full() const noexcept { return size_ == capacity_; }

private:
    std::vector<T>  buffer_;
    std::size_t     capacity_;
    std::size_t     size_;
    std::size_t     head_;
    std::size_t     tail_;
};

} // namespace Telemetry

#endif // CIRCULAR_BUFFER_H