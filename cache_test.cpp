#include "cache.h"

#include <cstring>

#include "gtest/gtest.h"

TEST(Page, basic) {
    const Page *page = new Page(128);
    EXPECT_EQ(page->get_bytes_free(), 128);
    delete page;
    const auto page2 = new Page;
    EXPECT_EQ(page2->get_bytes_free(), 4096);
    EXPECT_EQ(static_cast<int>(page2->get_data()[0]), 0);
    std::memcpy(page2->get_data_mut(), "hello", 5);
    auto hello = std::vector<uint8_t>(5, 0);
    std::memcpy(hello.data(), page2->get_data(), 5);
    const std::string_view hello_str(reinterpret_cast<const char *>(hello.data()), 5);
    EXPECT_EQ(hello_str, "hello");
    delete page2;
}

TEST(Cache, basic) {
    Cache cache(4096, 100);
}
