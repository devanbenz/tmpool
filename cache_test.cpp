#include "cache.h"

#include "gtest/gtest.h"

TEST(Page, basic) {
    const Page *page = new Page(128);
    EXPECT_EQ(page->get_bytes_free(), 128);
    delete page;
    const auto page2 = new Page;
    EXPECT_EQ(page2->get_bytes_free(), 4096);
    std::vector<uint8_t> data(4096, 0);
    std::cout << static_cast<int>(page2->get_data()[0]) << std::endl;
    // EXPECT_EQ(page2->get_data(), data.data());
    // EXPECT_EQ(page2->is_dirty(), false);
    // EXPECT_EQ(page2->get_data_mut(), data.data());
    // EXPECT_EQ(page2->is_dirty(), true);
    delete page2;
}
