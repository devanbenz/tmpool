#pragma once
#include <atomic>

#include "lru.h"
#include <vector>

constexpr int DEFAULT_PAGE_SIZE = 4096;
constexpr int DEFAULT_PAGES = 100;

class Page {
public:
    explicit Page(const int pid) : pid_(pid) {
        data_ = std::vector<uint8_t>(DEFAULT_PAGE_SIZE, 0);
        bytes_free_ = DEFAULT_PAGE_SIZE;
    };

    explicit Page(const int max_bytes_size, int pid) : pid_(pid), max_bytes_size_(max_bytes_size) {
        data_ = std::vector<uint8_t>(max_bytes_size_, 0);
        bytes_free_ = max_bytes_size_;
    }

    ~Page() = default;

    Page &operator=(const Page &other) {
        if (this == &other) {
            return *this;
        }
        data_ = other.data_;
        bytes_free_ = other.bytes_free_;
        return *this;
    };

    Page &operator=(Page &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        data_ = other.data_;
        bytes_free_ = other.bytes_free_;
        return *this;
    };

    [[nodiscard]] int get_bytes_free() const {
        return bytes_free_;
    }

    [[nodiscard]] int pid() const {
        return pid_;
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

    void set_bytes_free(const int bytes_free) {
        bytes_free_ = bytes_free;
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
    Cache() : pages_(DEFAULT_PAGES), page_size_(DEFAULT_PAGE_SIZE) {
    }

    explicit Cache(const int page_size, const int pages) : pages_(pages), page_size_(page_size) {
    }

    ~Cache() = default;

    std::optional<Page *> get_page(const int pid) {
        // TODO: Implement DiskManager where we do a disk seek to get
        // page if it's not in cache
        return pages_.get(pid);
    }

    void add_page(Page *page) {
        size_ += 1;
        // TODO: Right now my LRU automatically evicts pages when it's full.
        // That's not good! I need to implement a policy to evict pages based on Page
        // semantics such as pinned status and dirty status.
        pages_.put(page->pid(), page);
    }

private:
    LRU<int, Page *> pages_;
    int page_size_{};
    int size_{};
};
