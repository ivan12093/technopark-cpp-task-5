#ifndef TASK_5_AVLTREE_HPP
#define TASK_5_AVLTREE_HPP

#include <list>
#include <cstddef>
#include <functional>
#include <utility>
#include <concepts>
#include <memory>

#include <set.hpp>

namespace Blaze {
    template<class T, class Compare>
    concept comparable_with = requires(const T& t, Compare cmp) {
        { cmp(t, t) } -> std::convertible_to<bool>;
    };

    template<class T, class Less = std::less<T>>
    requires std::copyable<T> && std::semiregular<Less> && comparable_with<T, Less>
    class AVLTree;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
class Blaze::AVLTree : public Blaze::ISet<T> {
 public:
    using key_type = T;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using key_compare = Less;
    using value_compare = Less;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = std::list<T>::const_iterator;
    using const_iterator = std::list<T>::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    AVLTree() = default;
    explicit AVLTree(const Less& _less);
    AVLTree(std::initializer_list<T> initializerList);

    template<class InputIt> requires std::input_iterator<InputIt>
    AVLTree(InputIt first, InputIt last);

    AVLTree(const AVLTree& rhs);
    AVLTree(AVLTree&& rhs) noexcept;

    AVLTree& operator=(const AVLTree& rhs);
    AVLTree& operator=(AVLTree&& rhs) noexcept;
    ~AVLTree();

    iterator begin() const noexcept;
    iterator end() const noexcept;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() const noexcept;

    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    void erase(const T& val) override;
    void insert(const T& val) override;

    [[nodiscard]] size_t size() const noexcept override;
    [[nodiscard]] bool empty() const noexcept override;

    iterator find(const T& val) const;
    iterator lower_bound(const T& val) const;

    bool contains(const T& val) const override { return find(val) != end(); };

    bool is_disjoint(const ISet<T> &other) const noexcept override; // нет одинаковых элементов

    bool is_equal(const ISet<T> &other) const noexcept override;
    bool operator==(const ISet<T> &other) const noexcept override { return is_equal(other); };

    bool is_subset(const ISet<T> &other) const noexcept override;
    bool operator<=(const ISet<T> &other) const noexcept override { return is_subset(other); };

    bool is_superset(const ISet<T> &other) const noexcept override { return other.is_subset(*this); };
    bool operator>=(const ISet<T> &other) const noexcept override { return is_superset(other); };

    AVLTree Union(const AVLTree &other) const;
    AVLTree operator|(const AVLTree &other) const { return Union(other); };

    AVLTree intersection(const AVLTree &other) const;
    AVLTree operator&(const AVLTree &other) const { return intersection(other); };

    AVLTree difference(const AVLTree &other) const;
    AVLTree operator-(const AVLTree &other) const { return difference(other); };

    std::unique_ptr<ISet<T>> Union(const ISet<T> &other) const override;
    std::unique_ptr<ISet<T>> operator|(const ISet<T> &other) const override { return std::move(Union(other)); };

    std::unique_ptr<ISet<T>> intersection(const ISet<T> &other) const override;
    std::unique_ptr<ISet<T>> operator&(const ISet<T> &other) const override { return std::move(intersection(other)); };

    std::unique_ptr<ISet<T>> difference(const ISet<T> &other) const override;
    std::unique_ptr<ISet<T>> operator-(const ISet<T> &other) const override { return std::move(difference(other)); };

 private:
    struct Node {
        T key;
        unsigned height;
        Node* left;
        Node* right;
        typename std::list<T>::const_iterator list_it;
        explicit Node(const T& _key) : key(_key), height(1), left(nullptr), right(nullptr), list_it(nullptr) {};
        ~Node() { delete left; delete right; };
    };
    // разница высот между высотами правого и левого поддеревьев
    int balance_factor(Node* node) const noexcept;
    int get_height(Node* node) const noexcept;
    void fix_height(Node* node) noexcept;
    Node* rotate_right(Node* node) noexcept;
    Node* rotate_left(Node* node) noexcept;
    Node* balance(Node* node) noexcept;
    Node* insert(Node* node, const T& key, Node *parent);
    Node* find_min(Node* node) const noexcept;
    Node* find(Node* node, const T& key) const noexcept;
    Node* find_parent(Node* node, const T& key) const noexcept;
    Node* lower_bound(Node* node, const T& key) const noexcept;
    Node* remove_min(Node* node) noexcept;
    Node* remove(Node* node, const T& key) noexcept;
    Node* copy_nodes(Node* node) const;

    Node* root = nullptr;
    size_t _size = 0;
    std::list<T> linkedList;
    Less less = Less();

};

template<class T, class Less>
Blaze::AVLTree<T, Less>::AVLTree(const Less &_less) : less(_less) {}

template<class T, class Less>
Blaze::AVLTree<T, Less>::AVLTree(const AVLTree &rhs) {
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        insert(*it);
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::AVLTree(AVLTree &&rhs) noexcept :
    root(std::exchange(rhs.root, nullptr)),
    _size(std::exchange(rhs._size, 0)),
    linkedList(std::exchange(rhs.linkedList, std::list<T>())),
    less(rhs.less) {}

template<class T, class Less>
Blaze::AVLTree<T, Less> &Blaze::AVLTree<T, Less>::operator=(const AVLTree &rhs) {
    if (this == std::addressof(rhs))
        return *this;
    _size = 0;
    linkedList.clear();
    delete root;
    root = nullptr;
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        insert(*it);
    return *this;
}

template<class T, class Less>
Blaze::AVLTree<T, Less> &Blaze::AVLTree<T, Less>::operator=(AVLTree &&rhs) noexcept {
    std::swap(root, rhs.root);
    std::swap(_size, rhs._size);
    std::swap(linkedList, rhs.linkedList);
    std::swap(less, rhs.less);

    return *this;
}

template<class T, class Less>
int Blaze::AVLTree<T, Less>::balance_factor(AVLTree::Node *node) const noexcept {
    return get_height(node->right) - get_height(node->left);
}

template<class T, class Less>
int Blaze::AVLTree<T, Less>::get_height(AVLTree::Node *node) const noexcept {
    if (!node)
        return 0;
    return node->height;
}

template<class T, class Less>
void Blaze::AVLTree<T, Less>::fix_height(AVLTree::Node *node) noexcept {
    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::rotate_right(AVLTree::Node *node) noexcept {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    fix_height(node);
    fix_height(new_root);
    return new_root;
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::rotate_left(AVLTree::Node *node) noexcept {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    fix_height(node);
    fix_height(new_root);
    return new_root;
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::balance(AVLTree::Node *node) noexcept {
    fix_height(node);
    if (balance_factor(node) == 2) {
        if (balance_factor(node->right) < 0)
            node->right = rotate_right(node->right);
        node = rotate_left(node);
    }
    else if (balance_factor(node) == -2) {
        if (balance_factor(node->left) > 0)
            node->left = rotate_left(node->left);
        node = rotate_right(node);
    }
    return node;
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::insert(AVLTree::Node *node, const T& key, Node *parent) {
    if (!node) {
        ++_size;
        auto new_node = new Node(key);
        if (!parent) {
            linkedList.push_back(key);
            new_node->list_it = linkedList.begin();
            return new_node;
        }
        if (less(key, parent->key)) {
            linkedList.insert(parent->list_it, key);
            auto it = parent->list_it;
            new_node->list_it = --it;
            return new_node;
        }
        auto it = parent->list_it;
        linkedList.insert(++it, key);
        new_node->list_it = --it;
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
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::find_min(AVLTree::Node *node) const noexcept {
    Node *min = node;
    while (min->left)
        min = min->left;
    return min;
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::find(AVLTree::Node *node, const T& key) const noexcept {
    if (!node)
        return nullptr;
    if (!less(key, node->key) && !less(node->key, key))
        return node;
    if (less(key, node->key))
        return find(node->left, key);
    return find(node->right, key);
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::find_parent(AVLTree::Node *node, const T& key) const noexcept {
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
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::lower_bound(AVLTree::Node *node, const T& key) const noexcept {
    if (!node)
        return nullptr;
    if (!less(key, node->key) && !less(node->key, key))
        return node;
    if (!less(key, node->key))
        return lower_bound(node->right, key);

    Node* candidate = lower_bound(node->left, key);
    if (candidate && candidate->key < node->key)
        return candidate;
    return node;
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::remove(AVLTree::Node *node, const T& key) noexcept {
    if (!node)
        return nullptr;
    if (!less(key, node->key) && !less(node->key, key)) {
        --_size;
        Node* left = node->left;
        Node* right = node->right;
        linkedList.erase(node->list_it);
        node->left = nullptr;
        node->right = nullptr;
        delete node;
        if (!right)
            return left;
        Node* min = find_min(right);
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
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::remove_min(AVLTree::Node *node) noexcept {
    if (node->left==0)
        return node->right;
    node->left = remove_min(node->left);
    return balance(node);
}

template<class T, class Less>
typename Blaze::AVLTree<T, Less>::Node *Blaze::AVLTree<T, Less>::copy_nodes(AVLTree::Node *node) const {
    if (!node)
        return nullptr;
    Node* new_root = new Node(node);
    new_root->right = copy_nodes(node->right);
    new_root->left = copy_nodes(node->left);
    return new_root;
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::AVLTree(std::initializer_list<T> initializerList) {
    for (auto it = initializerList.begin(); it != initializerList.end(); ++it)
        insert(*it);
}

template<class T, class Less>
template<class InputIt> requires std::input_iterator<InputIt>
Blaze::AVLTree<T, Less>::AVLTree(InputIt first, InputIt last) {
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::begin() const noexcept {
    return linkedList.cbegin();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::end() const noexcept {
    return linkedList.cend();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::const_iterator Blaze::AVLTree<T, Less>::cbegin() const noexcept {
    return linkedList.cbegin();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::const_iterator Blaze::AVLTree<T, Less>::cend() const noexcept {
    return linkedList.cend();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::reverse_iterator Blaze::AVLTree<T, Less>::rbegin() const noexcept {
    return linkedList.crbegin();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::reverse_iterator Blaze::AVLTree<T, Less>::rend() const noexcept {
    return linkedList.crend();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::const_reverse_iterator Blaze::AVLTree<T, Less>::crbegin() const noexcept {
    return linkedList.crbegin();
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::const_reverse_iterator Blaze::AVLTree<T, Less>::crend() const noexcept {
    return linkedList.crend();
}

template<class T, class Less>
void Blaze::AVLTree<T, Less>::erase(const T &val) {
    root = remove(root, val);
}

template<class T, class Less>
void Blaze::AVLTree<T, Less>::insert(const T &val) {
    root = insert(root, val, nullptr);
}

template<class T, class Less>
size_t Blaze::AVLTree<T, Less>::size() const noexcept {
    return _size;
}

template<class T, class Less>
bool Blaze::AVLTree<T, Less>::empty() const noexcept {
    return _size == 0;
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::AVLTree::~AVLTree() {
    delete root;
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::find(const T &val) const {
    Node* target = find(root, val);
    if (target == nullptr)
        return linkedList.end();
    return target->list_it;
}

template<class T, class Less>
Blaze::AVLTree<T, Less>::iterator Blaze::AVLTree<T, Less>::lower_bound(const T &val) const {
    Node* lower = lower_bound(root, val);
    if (!lower)
        return linkedList.end();
    return lower->list_it;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
bool Blaze::AVLTree<T, Less>::is_disjoint(const ISet<T> &other) const noexcept {
    for (const T& elem : *this) {
        if (other.contains(elem)) {
            return false;
        }
    }
    return true;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
bool Blaze::AVLTree<T, Less>::is_equal(const ISet<T> &other) const noexcept {
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
bool Blaze::AVLTree<T, Less>::is_subset(const ISet<T> &other) const noexcept {
    for (const T& elem : *this) {
        if (!other.contains(elem)) {
            return false;
        }
    }
    return true;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less> Blaze::AVLTree<T, Less>::Union(const AVLTree &other) const {
    AVLTree<T, Less> result(*this);
    for (const T& elem : other) {
        result.insert(elem);
    }
    return result;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
Blaze::AVLTree<T, Less> Blaze::AVLTree<T, Less>::intersection(const AVLTree &other) const {
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
Blaze::AVLTree<T, Less> Blaze::AVLTree<T, Less>::difference(const AVLTree &other) const {
    AVLTree result(less);
    for (const T& elem : *this) {
        if (!other.contains(elem)) {
            result.insert(elem);
        }
    }
    return result;
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::unique_ptr<Blaze::ISet<T>> Blaze::AVLTree<T, Less>::Union(const Blaze::ISet<T> &other) const {
    auto result = std::make_unique<AVLTree<T, Less>>(*this);
    for (const T& elem : other) {
        result->insert(elem);
    }
    return std::move(result);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::unique_ptr<Blaze::ISet<T>> Blaze::AVLTree<T, Less>::intersection(const Blaze::ISet<T> &other) const {
    auto result = std::make_unique<AVLTree<T, Less>>();
    for (const T& elem : *this) {
        if (other.contains(elem)) {
            result->insert(elem);
        }
    }
    return std::move(result);
}

template<class T, class Less>
requires std::copyable<T> && std::semiregular<Less> && Blaze::comparable_with<T, Less>
std::unique_ptr<Blaze::ISet<T>> Blaze::AVLTree<T, Less>::difference(const Blaze::ISet<T> &other) const {
    auto result = std::make_unique<AVLTree<T, Less>>(*this);
    for (const T& elem : other) {
        result->erase(elem);
    }
    return std::move(result);
}

#endif // TASK_5_AVLTREE_HPP
