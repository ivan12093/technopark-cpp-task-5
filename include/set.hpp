#include <list>

template<class T>
class Set {
 private:
    struct Node {
        T key;
        int height;
        Node* left;
        Node* right;
        typename std::list<T>::const_iterator list_it;
        explicit Node(T _key) : key(_key), height(1), left(nullptr), right(nullptr), list_it(nullptr) {};
        ~Node() { delete left; delete right; };
    };
    int balance_factor(Node* node);
    int get_height(Node* node);
    void fix_height(Node* node);
    Node* rotate_right(Node* node);
    Node* rotate_left(Node* node);
    Node* balance(Node* node);
    Node* insert(Node* _root, T key, Node *parent);
    Node* find_min(Node* node);
    Node* find(Node* node, T key);
    Node* find_parent(Node* node, T key);
    Node* lower_bound(Node* node, T key);
    Node* remove_min(Node* node);
    Node* remove(Node* node, T key);
    Node* copy_nodes(Node* node);

    Node* root = nullptr;
    size_t _size = 0;
    std::list<T> linkedList;

 public:
    Set() = default;
    Set(std::initializer_list<T> initializerList);

    template<class InputIt>
    Set(InputIt first, InputIt last);

    Set(const Set& rhs);
    Set& operator=(const Set& rhs);
    ~Set();

    typename std::list<T>::const_iterator begin() const;
    typename std::list<T>::const_iterator end() const;

    void erase(const T& val);
    void insert(const T& val);

    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;

    typename std::list<T>::const_iterator find(const T& val) const;
    typename std::list<T>::const_iterator lower_bound(const T& val) const;
};

template<class T>
int Set<T>::balance_factor(Set::Node *node) {
    return get_height(node->right) - get_height(node->left);
}

template<class T>
int Set<T>::get_height(Set::Node *node) {
    if (!node)
        return 0;
    return node->height;
}

template<class T>
void Set<T>::fix_height(Set::Node *node) {
    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
}

template<class T>
typename Set<T>::Node *Set<T>::rotate_right(Set::Node *node) {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    fix_height(node);
    fix_height(new_root);
    return new_root;
}

template<class T>
typename Set<T>::Node *Set<T>::rotate_left(Set::Node *node) {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    fix_height(node);
    fix_height(new_root);
    return new_root;
}

template<class T>
typename Set<T>::Node *Set<T>::balance(Set::Node *node) {
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

template<class T>
typename Set<T>::Node *Set<T>::insert(Set::Node *_root, T key, Node *parent) {
    if (!_root) {
        ++_size;
        auto new_node = new Node(key);
        if (!parent) {
            linkedList.push_back(key);
            new_node->list_it = linkedList.begin();
            return new_node;
        }
        if (key < parent->key) {
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
    if (!(key < _root->key) && !(_root->key < key))
        return _root;
    if (key < _root->key)
        _root->left = insert(_root->left, key, _root);
    else
        _root->right = insert(_root->right, key, _root);
    return balance(_root);
}

template<class T>
typename Set<T>::Node *Set<T>::find_min(Set::Node *node) {
    Node *min = node;
    while (min->left)
        min = min->left;
    return min;
}

template<class T>
typename Set<T>::Node *Set<T>::find(Set::Node *node, T key) {
    if (!node)
        return nullptr;
    if (!(key < node->key) && !(node->key < key))
        return node;
    if (key < node->key)
        return find(node->left, key);
    return find(node->right, key);
}

template<class T>
typename Set<T>::Node *Set<T>::find_parent(Set::Node *node, T key) {
    if (!node)
        return nullptr;
    else if (node->left && !(key < node->left->key) && !(node->left->key < key))
        return node;
    else if (node->right && !(key < node->right->key) && !(node->right->key < key))
        return node;
    else if (key < node->key)
        return find_parent(node->left, key);
    else if (key > node->key)
        return find_parent(node->right, key);
    return nullptr;
}

template<class T>
typename Set<T>::Node *Set<T>::lower_bound(Set::Node *node, T key) {
    if (!node)
        return nullptr;
    if (!(key < node->key) && !(node->key < key))
        return node;
    if (key < node->key)
        return lower_bound(node->left, key);

    Node* candidate = lower_bound(node->right, key);
    if (!candidate)
        return node;
    if (key - candidate->key < key - node->key)
        return candidate;
    return node;
}

template<class T>
typename Set<T>::Node *Set<T>::remove(Set::Node *node, T key) {
    if (!node)
        return nullptr;
    if (!(key < node->key) && !(node->key < key)) {
        --_size;
        Node* left = node->left;
        Node* right = node->right;
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
    if (key < node->key)
        node->left = remove(node->left, key);
    else
        node->right = remove(node->right, key);
    return balance(node);
}

template<class T>
typename Set<T>::Node *Set<T>::remove_min(Set::Node *node) {
    if (node->left==0)
        return node->right;
    node->left = remove_min(node->left);
    return balance(node);
}

template<class T>
typename Set<T>::Node *Set<T>::copy_nodes(Set::Node *node) {
    if (!node)
        return nullptr;
    Node* new_root = new Node(node);
    new_root->right = copy_nodes(node->right);
    new_root->left = copy_nodes(node->left);
    return new_root;
}

template<class T>
Set<T>::Set(std::initializer_list<T> initializerList) {
    for (auto it = initializerList.begin(); it != initializerList.end(); ++it)
        insert(*it);
}

template<class T>
template<class InputIt>
Set<T>::Set(InputIt first, InputIt last) {
    for (auto it = first; it != last; ++it)
        insert(*it);
}

template<class T>
typename std::list<T>::const_iterator Set<T>::begin() const {
    return linkedList.begin();
}

template<class T>
typename std::list<T>::const_iterator Set<T>::end() const {
    return linkedList.end();
}

template<class T>
void Set<T>::erase(const T &val) {
    Node* target = find(root, val);
    if (!target)
        return;

    linkedList.erase(target->list_it);
    root = remove(root, val);
}

template<class T>
void Set<T>::insert(const T &val) {
    if (find(root, val) != nullptr)
        return;
    root = insert(root, val, nullptr);
    std::cout << "\n";
}

template<class T>
size_t Set<T>::size() const {
    return _size;
}

template<class T>
bool Set<T>::empty() const {
    return _size == 0;
}

template<class T>
Set<T>::~Set() {
    delete root;
}

template<class T>
typename std::list<T>::const_iterator Set<T>::find(const T &val) const {
    Node* target = find(root, val);
    if (target == nullptr)
        return linkedList.end();
    return target->list_it;
}

template<class T>
typename std::list<T>::const_iterator Set<T>::lower_bound(const T &val) const {
    Node* lower = lower_bound(root, val);
    if (!lower)
        return linkedList.end();
    return lower->list_it;
}

template<class T>
Set<T>::Set(const Set &rhs) {
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        insert(*it);
}

template<class T>
Set<T> &Set<T>::operator=(const Set &rhs) {
    if (this == &rhs)
        return *this;
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        insert(*it);
    return *this;
}
