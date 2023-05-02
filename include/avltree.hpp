#ifndef TASK_5_AVLTREE_HPP
#define TASK_5_AVLTREE_HPP

#include <list>
#include <cstddef>
#include <functional>
#include <utility>
#include <concepts>
#include <memory>
#include <iostream>

#include "blaze.hpp"
#include "iterator.hpp"
#include "exceptions.h"

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
class Blaze::AVLTree {
 public:
    using key_type = T;
    using value_type = T;
    using size_type = size_t;
    using key_compare = Less;
    using value_compare = Less;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = ConstIterator<T>;
    using const_iterator = ConstIterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    AVLTree();
    explicit AVLTree(const Less& _less);
    AVLTree(std::initializer_list<T> initializerList);

    template<class InputIt> requires std::input_iterator<InputIt>
    AVLTree(InputIt first, InputIt last);

    AVLTree(const AVLTree& rhs);
    AVLTree(AVLTree&& rhs) noexcept;

    AVLTree& operator=(const AVLTree& rhs);
    AVLTree& operator=(AVLTree&& rhs) noexcept;

    iterator begin() const noexcept;
    iterator end() const noexcept;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() const noexcept;

    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    void erase(const T& val);
    void insert(const T& val);

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    iterator find(const T& val) const;
    iterator lower_bound(const T& val) const;

    bool contains(const T& val) const { return find(val) != end(); }

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool is_disjoint(const C &other) const noexcept; // нет одинаковых элементов

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool is_equal(const C &other) const noexcept;

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool operator==(const C &other) const noexcept { return is_equal(other); }

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool is_subset(const C &other) const noexcept;

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool operator<=(const C &other) const noexcept { return is_subset(other); }

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool is_superset(const C &other) const noexcept { return other.is_subset(*this); }

    template<Blaze::has_contains C>
    requires std::convertible_to<typename C::value_type, T>
    bool operator>=(const C &other) const noexcept { return is_superset(other); }

    template<Blaze::iterable C>
    requires std::convertible_to<typename C::value_type, T>
    AVLTree Union(const C &other) const;

    template<Blaze::iterable C>
    requires std::convertible_to<typename C::value_type, T>
    AVLTree operator|(const C &other) const { return Union(other); }

    template<Blaze::iterable C>
    requires std::convertible_to<typename C::value_type, T>
    AVLTree intersection(const C &other) const;

    template<Blaze::iterable C>
    requires std::convertible_to<typename C::value_type, T>
    AVLTree operator&(const C &other) const { return intersection(other); }

    template<Blaze::iterable C>
    requires std::convertible_to<typename C::value_type, T>
    AVLTree difference(const C &other) const;

    template<Blaze::iterable C>
    requires std::convertible_to<typename C::value_type, T>
    AVLTree operator-(const C &other) const { return difference(other); }

 private:
    friend ConstIterator<T>;
    struct Node {
        T key;
        unsigned height;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> prev;
        std::weak_ptr<Node> next;
        explicit Node(const T& _key) : key(_key), height(1), left(),
                                       right(), prev(), next() {};
    };
    // разница высот между высотами правого и левого поддеревьев
    int balance_factor(const std::shared_ptr<Node>& node) const noexcept;
    int get_height(const std::shared_ptr<Node>& node) const noexcept;
    void fix_height(const std::shared_ptr<Node>& node) noexcept;
    std::shared_ptr<Node> rotate_right(const std::shared_ptr<Node>& node) noexcept;
    std::shared_ptr<Node> rotate_left(const std::shared_ptr<Node>& node) noexcept;
    std::shared_ptr<Node> balance(const std::shared_ptr<Node>& node) noexcept;
    std::shared_ptr<Node> insert(const std::shared_ptr<Node>& node, const T& key, const std::shared_ptr<Node>& parent);
    std::shared_ptr<Node> find(const std::shared_ptr<Node>& node, const T& key) const noexcept;
    std::shared_ptr<Node> find_parent(const std::shared_ptr<Node>& node, const T& key) const noexcept;
    std::shared_ptr<Node> lower_bound(const std::shared_ptr<Node>& node, const T& key) const noexcept;
    std::shared_ptr<Node> remove_min(const std::shared_ptr<Node>& node) noexcept;
    std::shared_ptr<Node> remove(const std::shared_ptr<Node>& node, const T& key) noexcept;
    std::shared_ptr<Node> copy_nodes(const std::shared_ptr<Node>& node) const;

    std::shared_ptr<Node> root;
    std::weak_ptr<Node> most_left;
    std::weak_ptr<Node> most_right;
    std::shared_ptr<Node> connector_iter;
    size_t _size = 0;
    Less less = Less();
};

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::AVLTree() {
    try {
        connector_iter = std::make_shared<Node>(T());
    } catch (const std::bad_alloc&) {
        throw Blaze::AllocateMemoryException(__FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
    }
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::AVLTree(const Less &_less) : AVLTree() {
    less = _less;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::AVLTree(const AVLTree &rhs) : AVLTree(rhs.less) {
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        insert(*it);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::AVLTree(AVLTree &&rhs) noexcept :
    root(std::exchange(rhs.root, nullptr)),
    most_left(std::exchange(rhs.most_left), nullptr),
    most_right(std::exchange(rhs.most_right), nullptr),
    connector_iter(std::exchange(rhs.connector_iter), nullptr),
    _size(std::exchange(rhs._size, 0)),
    less(rhs.less) {}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less> &Blaze::AVLTree<T, Less>::operator=(const AVLTree &rhs) {
    if (this == std::addressof(rhs))
        return *this;
    _size = 0;
    root = nullptr;
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        insert(*it);
    return *this;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less> &Blaze::AVLTree<T, Less>::operator=(AVLTree &&rhs) noexcept {
    std::swap(rhs.root, root);
    std::swap(rhs.most_left, most_left);
    std::swap(rhs.most_right, most_right);
    std::swap(rhs.connector_iter, connector_iter);
    std::swap(rhs._size, _size);
    std::swap(rhs.less, less);

    return *this;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
int Blaze::AVLTree<T, Less>::balance_factor(const std::shared_ptr<Node>& node) const noexcept {
    return get_height(node->right) - get_height(node->left);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
int Blaze::AVLTree<T, Less>::get_height(const std::shared_ptr<Node>& node) const noexcept {
    if (!node)
        return 0;
    return node->height;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
void Blaze::AVLTree<T, Less>::fix_height(const std::shared_ptr<Node>& node) noexcept {
    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::rotate_right(const std::shared_ptr<Node>& node) noexcept {
    auto new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    fix_height(node);
    fix_height(new_root);
    return new_root;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::rotate_left(const std::shared_ptr<Node>& node) noexcept {
    auto new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    fix_height(node);
    fix_height(new_root);
    return new_root;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::balance(const std::shared_ptr<Node>& node) noexcept {
    fix_height(node);
    auto new_root = node;
    if (balance_factor(new_root) == 2) {
        if (balance_factor(new_root->right) < 0)
            node->right = rotate_right(new_root->right);
        new_root = rotate_left(node);
    }
    else if (balance_factor(new_root) == -2) {
        if (balance_factor(new_root->left) > 0)
            new_root->left = rotate_left(new_root->left);
        new_root = rotate_right(new_root);
    }
    return new_root;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::insert(const std::shared_ptr<Node>& node, const T& key, const std::shared_ptr<Node>& parent) {
    if (!node) {
        ++_size;
        std::shared_ptr<Node> new_node;
        try {
            new_node = std::make_shared<Node>(key);
        } catch (const std::bad_alloc&) {
            throw Blaze::AllocateMemoryException(__FILE__, __LINE__, typeid(*this).name(), __FUNCTION__);
        }

        if (!parent) {
            most_left = new_node;
            most_right = new_node;
            connector_iter->next = new_node;
            connector_iter->prev = new_node;
            new_node->prev = connector_iter;
            new_node->next = connector_iter;
            return new_node;
        }
        if (less(key, parent->key)) {
            if (most_left.lock().get() == parent.get())
                most_left = new_node;
            new_node->prev = parent->prev;
            if (!new_node->prev.expired())
                new_node->prev.lock()->next = new_node;
            new_node->next = parent;
            parent->prev = new_node;
            return new_node;
        }
        if (most_right.lock().get() == parent.get())
            most_right = new_node;
        new_node->prev = parent;
        new_node->next = parent->next;
        if (!new_node->next.expired())
            new_node->next.lock()->prev = new_node;
        parent->next = new_node;
        return new_node;
    }
    if (!less(key, node->key) && !less(node->key, key))
        return node;
    if (less(key, node->key))
        node->left = insert(node->left, key, node);
    else
        node->right = insert(node->right, key, node);
    return balance(node);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::find(const std::shared_ptr<Node>& node, const T& key) const noexcept {
    if (!node)
        return nullptr;
    if (!less(key, node->key) && !less(node->key, key))
        return node;
    if (less(key, node->key))
        return find(node->left, key);
    return find(node->right, key);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::find_parent(const std::shared_ptr<Node>& node, const T& key) const noexcept {
    if (!node)
        return nullptr;
    else if (node->left && !less(key, node->left->key) && !less(node->left->key, key))
        return node;
    else if (node->right && !less(key, node->right->key) && !less(node->right->key, key))
        return node;
    else if (less(key, node->key))
        return find_parent(node->left, key);
    else if (less(node->key, key))
        return find_parent(node->right, key);
    return nullptr;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::lower_bound(const std::shared_ptr<Node>& node, const T& key) const noexcept {
    if (!node)
        return nullptr;
    if (!less(key, node->key) && !less(node->key, key))
        return node;
    if (!less(key, node->key))
        return lower_bound(node->right, key);

    auto candidate = lower_bound(node->left, key);
    if (candidate && candidate->key < node->key)
        return candidate;
    return node;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::remove(const std::shared_ptr<Node>& node, const T& key) noexcept {
    if (!node)
        return nullptr;
    if (!less(key, node->key) && !less(node->key, key)) {
        --_size;
        auto left = node->left;
        auto right = node->right;
        auto prev = node->prev;
        auto next = node->next;
        if (!prev.expired())
            prev.lock()->next = next;
        if (!next.expired())
            next.lock()->prev = prev;

        if (most_right.lock().get() == node.get())
            most_right = right;
        if (most_left.lock().get() == node.get())
            most_left = left;

        node->left = nullptr;
        node->right = nullptr;
        if (!right)
            return left;
        auto min = next.lock();
        min->right = remove_min(right);
        min->left = left;
        return balance(min);
    }
    if (less(key, node->key))
        node->left = remove(node->left, key);
    else
        node->right = remove(node->right, key);
    return balance(node);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::remove_min(const std::shared_ptr<Node>& node) noexcept {
    if (node->left == nullptr)
        return node->right;
    node->left = remove_min(node->left);
    return balance(node);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::shared_ptr<typename Blaze::AVLTree<T, Less>::Node>
Blaze::AVLTree<T, Less>::copy_nodes(const std::shared_ptr<Node>& node) const {
    if (!node)
        return nullptr;
    auto new_root = new Node(node);
    new_root->right = copy_nodes(node->right);
    new_root->left = copy_nodes(node->left);
    return new_root;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::AVLTree(std::initializer_list<T> initializerList) : AVLTree() {
    for (auto it = initializerList.begin(); it != initializerList.end(); ++it)
        insert(*it);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<class InputIt> requires std::input_iterator<InputIt>
Blaze::AVLTree<T, Less>::AVLTree(InputIt first, InputIt last) : AVLTree() {
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::begin() const noexcept {
    if (empty())
        return end();
    return ConstIterator<T>(most_left);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::end() const noexcept {
    return ConstIterator<T>(connector_iter);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::const_iterator Blaze::AVLTree<T, Less>::cbegin() const noexcept {
    return begin();
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::const_iterator Blaze::AVLTree<T, Less>::cend() const noexcept {
    return end();
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::reverse_iterator Blaze::AVLTree<T, Less>::rbegin() const noexcept {
    return ConstIterator<T>(connector_iter);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::reverse_iterator Blaze::AVLTree<T, Less>::rend() const noexcept {
    if (empty())
        return rbegin();
    return ConstIterator<T>(most_right);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::const_reverse_iterator Blaze::AVLTree<T, Less>::crbegin() const noexcept {
    return rbegin();
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::const_reverse_iterator Blaze::AVLTree<T, Less>::crend() const noexcept {
    return rend();
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
void Blaze::AVLTree<T, Less>::erase(const T &val) {
    root = remove(root, val);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
void Blaze::AVLTree<T, Less>::insert(const T &val) {
    root = insert(root, val, nullptr);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
size_t Blaze::AVLTree<T, Less>::size() const noexcept {
    return _size;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
bool Blaze::AVLTree<T, Less>::empty() const noexcept {
    return _size == 0;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::find(const T &val) const {
    auto target = find(root, val);
    if (target == nullptr)
        return end();
    return ConstIterator<T>(target);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::lower_bound(const T &val) const {
    auto lower = lower_bound(root, val);
    if (!lower)
        return end();
    return ConstIterator<T>(lower);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<Blaze::has_contains C>
requires std::convertible_to<typename C::value_type, T>
bool Blaze::AVLTree<T, Less>::is_disjoint(const C &other) const noexcept {
    for (const T& elem : *this) {
        if (other.contains(elem)) {
            return false;
        }
    }
    return true;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<Blaze::has_contains C>
requires std::convertible_to<typename C::value_type, T>
bool Blaze::AVLTree<T, Less>::is_equal(const C &other) const noexcept {
    for (const T& elem : *this) {
        if (!other.contains(elem)) {
            return false;
        }
    }
    for (const T& elem : other) {
        if (!contains(elem)) {
            return false;
        }
    }
    return true;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<Blaze::has_contains C>
requires std::convertible_to<typename C::value_type, T>
bool Blaze::AVLTree<T, Less>::is_subset(const C &other) const noexcept {
    for (const T& elem : *this) {
        if (!other.contains(elem)) {
            return false;
        }
    }
    return true;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<Blaze::iterable C>
requires std::convertible_to<typename C::value_type, T>
Blaze::AVLTree<T, Less> Blaze::AVLTree<T, Less>::Union(const C &other) const {
    AVLTree<T, Less> result(*this);
    for (const T& elem : other) {
        result.insert(elem);
    }
    return result;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<Blaze::iterable C>
requires std::convertible_to<typename C::value_type, T>
Blaze::AVLTree<T, Less> Blaze::AVLTree<T, Less>::intersection(const C &other) const {
    AVLTree result(less);
    for (const T& elem : *this) {
        if (other.contains(elem)) {
            result.insert(elem);
        }
    }
    return result;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
template<Blaze::iterable C>
requires std::convertible_to<typename C::value_type, T>
Blaze::AVLTree<T, Less> Blaze::AVLTree<T, Less>::difference(const C &other) const {
    AVLTree result(less);
    for (const T& elem : *this) {
        if (!other.contains(elem)) {
            result.insert(elem);
        }
    }
    return result;
}

#endif // TASK_5_AVLTREE_HPP
