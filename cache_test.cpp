#include "cache.h"

#include <cstring>

#include "gtest/gtest.h"

TEST(Page, basic) {
    const Page *page = new Page(128);
    EXPECT_EQ(page->get_bytes_free(), 128);
    delete page;
    const auto page2 = new Page(10);
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
    const DiskManager disk_manager;
    Cache cache(10, 3, disk_manager);
    cache.add_page(new Page(0));
    cache.add_page(new Page(1));
    cache.add_page(new Page(2));

    auto p1 = cache.get_page(0);
    auto p2 = cache.get_page(1);
    auto p3 = cache.get_page(2);
    auto *d1 = p1.value()->get_data_mut();
    std::memcpy(d1, "hello", 5);
    const std::string_view hello_str(reinterpret_cast<const char *>(d1), 5);
    EXPECT_EQ(hello_str, "hello");
}

TEST(Cache, get_put) {
    const DiskManager disk_manager;
    Cache cache(10, 3, disk_manager);
    cache.add_page(new Page(0));
    cache.add_page(new Page(1));
    cache.add_page(new Page(2));
    auto p1 = cache.get_page(0);
    auto p2 = cache.get_page(1);
    auto p3 = cache.get_page(2);
    EXPECT_EQ(p1.has_value(), true);
    EXPECT_EQ(p2.has_value(), true);
    EXPECT_EQ(p3.has_value(), true);
}
