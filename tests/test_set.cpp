#include <gtest/gtest.h>

#include "set.hpp"

#define SET_EXPECT_EQ(set1, set2) EXPECT_EQ(set1.size(), set2.size());            \
for (auto it1 = set1.begin(), it2 = set2.begin(); it1 != set1.end(); ++it1, ++it2) \
EXPECT_EQ(*it1, *it2);

TEST(SET, Set_default) {
    Set<int> set;
    set.insert(1);
    set.insert(4);
    set.insert(5);
    set.insert(3);
    set.insert(2);
    set.insert(2);
    set.insert(2);
    set.insert(6);
    set.insert(7);
    set.insert(8);
    set.insert(9);
    set.insert(10);

    EXPECT_EQ(set.size(), 10);
    EXPECT_EQ(set.empty(), false);
    int i = 1;
    for (auto it = set.begin(); i < 11; ++i, ++it)
        EXPECT_EQ(*it, i);

    set.erase(4);
    set.erase(0);
    set.erase(1);
    set.erase(5);
    set.erase(3);
    set.erase(2);
    set.erase(6);
    set.erase(7);
    set.erase(8);
    set.erase(9);
    set.erase(10);
    set.erase(137);

    EXPECT_EQ(set.size(), 0);
    EXPECT_EQ(set.empty(), true);
}

TEST(SET, Copy_and_assign) {
    Set<int> set1;
    set1.insert(1);
    set1.insert(4);
    set1.insert(5);
    set1.insert(3);
    set1.insert(2);

    Set<int> set2(set1);
    SET_EXPECT_EQ(set1, set2);
    auto set3 = set1;
    SET_EXPECT_EQ(set3, set1);
    auto set3p = &set3;
    set3 = *set3p;
    set3 = set2;
    SET_EXPECT_EQ(set3, set2);
    Set<int> set4(set3.begin(), set3.end());
    SET_EXPECT_EQ(set4, set3);
}

TEST(SET, Bidirectional_iterators) {
    Set<int> set = {5, 1, 3, 4, 2, 8, 6, 5, 7};
    int i = 1;
    for (auto it = set.begin(); i < 9; ++i, ++it)
        EXPECT_EQ(*it, i);
    i = 8;
    auto end = set.end();
    for (auto it = --end; i > 0; --i, --it)
        EXPECT_EQ(*it, i);
}

TEST(SET, find_elem_by_value) {
    Set<int> set = {5, 1, 3, 4, 2, 8, 6, 5, 7};
    auto it = set.find(2);
    EXPECT_EQ(*it, 2);
    it = set.find(7);
    EXPECT_EQ(*it, 7);
    ++it;
    EXPECT_EQ(*it, 8);
    it = set.find(13);
    EXPECT_EQ(it, set.end());
}

TEST(SET, find_lower_bound) {
    Set<int> set = {1, 2, -1, 4, 7, 13};
    auto it = set.lower_bound(10);
    EXPECT_EQ(*it, 13);
    it = set.lower_bound(0);
    EXPECT_EQ(*it, 1);
    it = set.lower_bound(15);
    EXPECT_EQ(it, set.end());
    it = set.lower_bound(1);
    EXPECT_EQ(*it, 1);
}
