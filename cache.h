#pragma once
#include <atomic>

#include "lru.h"
#include <vector>

constexpr int DEFAULT_PAGE_SIZE = 4096;

class Page {
public:
    Page() {
        data_ = std::vector<uint8_t>(DEFAULT_PAGE_SIZE, 0);
        bytes_free_ = DEFAULT_PAGE_SIZE;
    };

    explicit Page(const int max_bytes_size) : max_bytes_size_(max_bytes_size) {
        data_ = std::vector<uint8_t>(max_bytes_size_, 0);
        bytes_free_ = max_bytes_size_;
    }

    ~Page() = default;

    [[nodiscard]] int get_bytes_free() const {
        return bytes_free_;
    }

    void pin() {
        pinned_ = true;
    }

    void unpin() {
        pinned_ = false;
    }

    bool is_pinned() {
        return pinned_;
    }

    [[nodiscard]] bool is_dirty() const {
        return dirty_.load();
    }

    void set_pid(const int pid) {
        pid_ = pid;
    }

    [[nodiscard]] int get_pid() const {
        return pid_;
    }

    [[nodiscard]] const uint8_t *get_data() const {
        return data_.data();
    }

    uint8_t *get_data_mut() {
        dirty_.store(true);
        return data_.data();
    }

private:
    std::vector<uint8_t> data_;
    std::atomic_bool dirty_{};
    std::atomic_bool pinned_{};
    int pid_{};
    int bytes_free_{};
    int max_bytes_size_{};
};

class Cache {
public:
    Cache() = default;

    ~Cache() = default;

private:
    LRU<int, Page> pages_;
};
