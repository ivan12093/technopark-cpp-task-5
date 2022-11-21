#include <gtest/gtest.h>

#include "set.hpp"

TEST(SET, Set_default) {
    Set<int> set;
    set.insert(1);
    set.insert(4);
    set.insert(5);
    set.insert(3);
    set.insert(2);

    EXPECT_EQ(set.size(), 5);
    EXPECT_EQ(set.empty(), false);

    set.erase(1);
    set.erase(4);
    set.erase(5);
    set.erase(3);
    set.erase(2);

    EXPECT_EQ(set.size(), 0);
    EXPECT_EQ(set.empty(), true);
}