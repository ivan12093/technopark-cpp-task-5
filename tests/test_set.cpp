#include <gtest/gtest.h>

#include "avltree.hpp"

#define SET_EXPECT_EQ(set1, set2) EXPECT_EQ(set1.size(), set2.size());            \
for (auto it1 = set1.begin(), it2 = set2.begin(); it1 != set1.end(); ++it1, ++it2) \
{ EXPECT_NE(it2, set2.end()); EXPECT_EQ(*it1, *it2); }

TEST(SET, Set_default) {
    Blaze::AVLTree<int> set;
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
    Blaze::AVLTree<int> set1;
    set1.insert(1);
    set1.insert(4);
    set1.insert(5);
    set1.insert(3);
    set1.insert(2);

    Blaze::AVLTree<int> set2(set1);
    SET_EXPECT_EQ(set1, set2);
    auto set3 = set1;
    SET_EXPECT_EQ(set3, set1);
    auto set3p = &set3;
    set3 = *set3p;
    set3 = set2;
    SET_EXPECT_EQ(set3, set2);
    Blaze::AVLTree<int> set4(set3.begin(), set3.end());
    SET_EXPECT_EQ(set4, set3);
}

TEST(SET, Bidirectional_iterators) {
    Blaze::AVLTree<int> set = {5, 1, 3, 4, 2, 8, 6, 5, 7};
    int i = 1;
    for (auto it = set.begin(); i < 9; ++i, ++it)
        EXPECT_EQ(*it, i);
    i = 8;
    auto end = set.end();
    for (auto it = --end; i > 0; --i, --it)
        EXPECT_EQ(*it, i);
}

TEST(SET, find_elem_by_value) {
    Blaze::AVLTree<int> set = {5, 1, 3, 4, 2, 8, 6, 5, 7};
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
    Blaze::AVLTree<int> set = {1, 2, -1, 4, 7, 13};
    auto it = set.lower_bound(10);
    EXPECT_EQ(*it, 13);
    it = set.lower_bound(0);
    EXPECT_EQ(*it, 1);
    it = set.lower_bound(15);
    EXPECT_EQ(it, set.end());
    it = set.lower_bound(1);
    EXPECT_EQ(*it, 1);
}


TEST(SET, is_disjoint) {
    Blaze::AVLTree<int> set = {1, 2, -1, 4, 7, 13};
    Blaze::AVLTree<int> set1 = {10, 7, 3};

    EXPECT_FALSE(set.is_disjoint(set1));
    EXPECT_FALSE(set1.is_disjoint(set));

    set1 = {10, 15, 16, 19, -10, -12};
    EXPECT_TRUE(set.is_disjoint(set1));
    EXPECT_TRUE(set1.is_disjoint(set));
}

TEST(SET, is_equal) {
    Blaze::AVLTree<int> set = {1, 2, -1, 4, 7, 13};
    Blaze::AVLTree<int> set1 = {1, 2, -1, 4, 7, 13};

    EXPECT_TRUE(set.is_equal(set1));
    EXPECT_TRUE(set == set1);

    Blaze::AVLTree<int> set2 = {1, 2, -1, 4, 7, 13, 15};
    EXPECT_FALSE(set.is_equal(set2));
    EXPECT_FALSE(set2.is_equal(set));
    EXPECT_FALSE(set == set2);
    EXPECT_FALSE(set2 == set);
    EXPECT_TRUE(set != set2);
    EXPECT_TRUE(set2 != set);

}

TEST(SET, is_subset) {
    Blaze::AVLTree<int> set = {1, 2, 3, 4, 5};
    Blaze::AVLTree<int> set1 = {1, 4, 5};

    EXPECT_TRUE(set1.is_subset(set));
    EXPECT_TRUE(set1 <= set);
    EXPECT_FALSE(set.is_subset(set1));
    EXPECT_FALSE(set <= set1);

    set1.insert(6);
    EXPECT_FALSE(set1.is_subset(set));
    EXPECT_FALSE(set1 <= set);
    EXPECT_FALSE(set.is_subset(set1));
    EXPECT_FALSE(set <= set1);
}

TEST(SET, is_superset) {
    Blaze::AVLTree<int> set = {1, 2, 3, 4, 5};
    Blaze::AVLTree<int> set1 = {1, 4, 5};

    EXPECT_TRUE(set.is_superset(set1));
    EXPECT_TRUE(set >= set1);
    EXPECT_FALSE(set.is_subset(set1));
    EXPECT_FALSE(set <= set1);
}
