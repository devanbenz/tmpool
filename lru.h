#pragma once
#include <forward_list>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

template<typename K>
concept printable_key = requires(K k)
{
    { std::cout << k } -> std::same_as<std::ostream &>;
};
template<typename V>
concept printable_value = requires(V v)
{
    { std::cout << v } -> std::same_as<std::ostream &>;
};

template<typename K, typename V>
class LRU {
public:
    LRU() = default;

    explicit LRU(const int max_size) : max_size_(max_size) {
        if (max_size_ <= 0) {
            throw std::invalid_argument("max_size must be greater than 0");
        }
    }

    ~LRU() = default;

    LRU &operator=(const LRU &other) {
        if (this == &other) {
            return *this;
        }
        max_size_ = other.max_size_;
        size_ = other.size_;
        nodes_ = other.nodes_;
        map_ = other.map_;
        return *this;
    }

    LRU &operator=(LRU &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        max_size_ = other.max_size_;
        size_ = other.size_;
        nodes_ = other.nodes_;
        map_ = other.map_;
        return *this;
    }

    void put(K key, V value) {
        if (auto it = map_.find(key); it != map_.end()) {
            it->second->second = value;
            nodes_.splice(nodes_.begin(), nodes_, it->second);
            return;
        }
        if (size_ + 1 > max_size_) {
            map_.erase(nodes_.back().first);
            nodes_.pop_back();
            size_ -= 1;
        }
        nodes_.push_front(std::make_pair(key, value));
        map_[key] = nodes_.begin();
        size_ += 1;
    }

    std::optional<V> back() {
        if (nodes_.empty()) {
            return std::nullopt;
        }
        return std::optional<V>(nodes_.back().second);
    }

    std::optional<V> get(K key) {
        auto it = map_.find(key);
        if (it == map_.end()) {
            return std::nullopt;
        }
        nodes_.splice(nodes_.begin(), nodes_, it->second);
        return std::optional<V>(it->second->second);
    }


    [[nodiscard]] bool empty() const {
        return size_ == 0;
    }

    [[nodiscard]] int size() const {
        return size_;
    }

private:
    std::list<std::pair<K, V> > nodes_{};
    // Where K is the key, int is the index in the list
    std::unordered_map<K, typename std::list<std::pair<K, V> >::iterator> map_{};
    int size_{};
    int max_size_{};
};

