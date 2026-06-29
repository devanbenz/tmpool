#include "lru.h"
#include <gtest/gtest.h>

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
