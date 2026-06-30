#include "lru.h"
#include <gtest/gtest.h>
#include <chrono>
#include <random>

TEST(LRU, Test) {
    LRU<int, int> lru(3);
    lru.put(1, 1);
    lru.put(2, 2);
    lru.put(3, 3);
    EXPECT_EQ(lru.get(1), 1);
    EXPECT_EQ(lru.get(2), 2);
    EXPECT_EQ(lru.get(3), 3);
    //Should insert a new element and we evict '1'
    lru.put(4, 4);
    EXPECT_EQ(lru.get(4), 4);
    EXPECT_EQ(lru.get(1), std::nullopt);
    EXPECT_EQ(lru.get(2), 2);
    EXPECT_EQ(lru.get(3), 3);
    lru.put(5, 5);
    EXPECT_EQ(lru.get(5), 5);
    EXPECT_EQ(lru.get(2), 2);
    EXPECT_EQ(lru.get(3), 3);
    EXPECT_EQ(lru.get(4), std::nullopt);
    EXPECT_EQ(lru.get(1), std::nullopt);
}

// Spec: ["LRUCache","get","put","get","put","put","get","get"]
//       [[2],[2],[2,6],[1],[1,5],[1,2],[1],[2]]
TEST(LRU, Spec) {
    LRU<int, int> lru(2);
    EXPECT_EQ(lru.get(2), std::nullopt);
    lru.put(2, 6);
    EXPECT_EQ(lru.get(1), std::nullopt);
    lru.put(1, 5);
    lru.put(1, 2);
    EXPECT_EQ(lru.get(1), 2);
    EXPECT_EQ(lru.get(2), 6);
}

// Performance smoke test: hammer the cache with a large, mixed workload of
// puts and gets and report throughput. Not a correctness assertion beyond a
// trivial sanity check — run with --gtest_filter=LRU.Performance and watch the
// printed timings. Key space is ~10x the cache capacity so we exercise hits,
// misses, and evictions rather than just appending.
TEST(LRU, Performance) {
    constexpr int kCapacity = 10'000;
    constexpr int kKeySpace = kCapacity * 10;
    constexpr int kOps = 5'000'000;

    LRU<int, int> lru(kCapacity);

    // Deterministic PRNG so runs are comparable across builds.
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> key_dist(0, kKeySpace - 1);
    std::uniform_int_distribution<int> coin(0, 3); // ~25% puts, ~75% gets

    long long hits = 0;
    long long puts = 0;
    long long gets = 0;

    for (int i = 0; i < kOps; ++i) {
        const int key = key_dist(rng);
        if (coin(rng) == 0) {
            lru.put(key, key);
            ++puts;
        } else {
            if (lru.get(key).has_value()) {
                ++hits;
            }
            ++gets;
        }
    }
}
