template<class T>
class Set {
 private:
    struct Node {
        T key;
        int height;
        Node* left;
        Node* right;
        explicit Node(T _key) : key(_key), height(1), left(nullptr), right(nullptr) {};
        ~Node() { delete left; delete right; };
    };
    int balance_factor(Node* node);
    int get_height(Node* node);
    void fix_height(Node* node);
    Node* rotate_right(Node* node);
    Node* rotate_left(Node* node);
    Node* balance(Node* node);
    Node* insert(Node* _root, T key);
    Node* find_min(Node* node);
    Node* remove_min(Node* node);
    Node* remove(Node* node, T key);
    Node* copy_nodes(Node* node);

    struct ListNode {
        T key;
        ListNode* prev;
        ListNode* next;
        explicit ListNode(T _key) : key(_key), prev(nullptr), next(nullptr) {};
    };

    ListNode* copy_nodes(ListNode* node);
    void delete_nodes(ListNode *node);

    Node* root = nullptr;
    size_t _size = 0;
    ListNode* _begin = nullptr;
    ListNode* _end = nullptr;

 public:
    class const_iterator {
        ListNode* _node = nullptr;
     public:
        const_iterator() = delete;
        explicit const_iterator(ListNode* node) : _node(node) {};
        const_iterator& operator++();
        const const_iterator operator++(int);
        const_iterator& operator--();
        const const_iterator operator--(int);
        const T& operator*();
    };



    Set() = default;
    Set(std::initializer_list<T> initializerList);

    template<class InputIt>
    Set(InputIt first, InputIt last);

    Set(const Set& rhs);
    Set& operator=(const Set& rhs);
    ~Set();

    const_iterator begin() const;
    const_iterator end() const;

    void erase(const T& val);

    void insert(const T& val);

    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;

    const_iterator find(const T& val) const;
    const_iterator lower_bound(const T& val) const;
};

template<class T>
typename Set<T>::const_iterator& Set<T>::const_iterator::operator++() {
    ++_node;
    return *this;
}

template<class T>
const typename Set<T>::const_iterator Set<T>::const_iterator::operator++(int) {
    ++_node;
    return const_iterator(_node->prev);
}

template<class T>
typename Set<T>::const_iterator &Set<T>::const_iterator::operator--() {
    --_node;
    return *this;
}

template<class T>
const typename Set<T>::const_iterator Set<T>::const_iterator::operator--(int) {
    --_node;
    return const_iterator(_node->next);
}

template<class T>
const T &Set<T>::const_iterator::operator*() {
    return _node->key;
}

template<class T>
int Set<T>::balance_factor(Set::Node *node) {
    return get_hight(node->right) - get_hight(node->left->height);
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
typename Set<T>::Node *Set<T>::insert(Set::Node *_root, T key) {
    if (!_root)
        return new Node(key);
    if (key < _root->key && _root->key < key)
        return _root;
    if (key < _root)
        _root->left = insert(_root->left, key);
    else
        _root->right = insert(root->right, key);
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
typename Set<T>::Node *Set<T>::remove(Set::Node *node, T key) {
    if (!node)
        return nullptr;
    if (key < node->key && node->key < key) {
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
    if (key < node->key)
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
typename Set<T>::ListNode *Set<T>::copy_nodes(Set::ListNode *_root) {
    ListNode* new_list
}

template<class T>
void Set<T>::delete_nodes(Set::ListNode *node) {
    ListNode* next = node->next;
    while (node) {
        delete node;
        node = next;
        next = node->next;
    }
}

template<class T>
Set<T>::Set(std::initializer_list<T> initializerList) {
    for (auto it = initializerList.begin(); it != initializerList.end(); ++it)
        insert(root, it);
}

template<class T>
template<class InputIt>
Set<T>::Set(InputIt first, InputIt last) {
    for (auto it = first; it != last; ++it)
        insert(root, it);
}

template<class T>
Set<T>::Set(const Set &rhs) {
}







