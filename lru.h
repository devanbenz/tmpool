#pragma once
#include <deque>
#include <iostream>
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
        // TODO: overwriting the kv seems expensive. How should we handle this?
        if (map_.contains(key)) {
            nodes_.erase(std::find(nodes_.begin(), nodes_.end(), key));
            map_.erase(key);
            nodes_.emplace_front(key);
            map_.emplace(key, value);
            return;
        }
        if (size_ + 1 > max_size_) {
            map_.erase(nodes_.back());
            nodes_.pop_back();
            map_.emplace(key, value);
            nodes_.emplace_front(key);
        } else {
            size_++;
            map_.emplace(key, value);
            nodes_.emplace_front(key);
        }
    }

    std::optional<V> get(K key) {
        if (!map_.contains(key)) {
            return std::nullopt;
        }

        if (nodes_.back() == key) {
            nodes_.pop_back();
            nodes_.emplace_front(key);
        }
        return std::optional<V>(map_.at(key));
    }

private:
    std::deque<K> nodes_{};
    std::unordered_map<K, V> map_{};
    int size_{};
    int max_size_{};
};

