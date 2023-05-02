#include <iostream>

#include "avltree.hpp"

int main() {
    Blaze::AVLTree<int> set;
    set.insert(1);
    set.insert(4);
    set.insert(5);
    set.insert(3);
    set.insert(2);

    for (auto i : set) {
        std::cout << i;
    }

    set.erase(1);
    set.erase(4);
    set.erase(5);
    set.erase(3);
    set.erase(2);
}
