#ifndef TASK_5_ITERATOR
#define TASK_5_ITERATOR

#include <cstddef>
#include <iterator>
#include <memory>

#include "blaze.hpp"

template<class T>
class Blaze::ConstIterator {
public:
    using value_type = T;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using iterator_category = std::bidirectional_iterator_tag;
    using node = typename AVLTree<T>::Node;

    ConstIterator() = default;
    ConstIterator(const ConstIterator& iter) = default;
    explicit ConstIterator(const std::weak_ptr<node>& _node) : ptr(_node) {};
    explicit ConstIterator(const std::shared_ptr<node>& _node) : ptr(_node) {};
    ~ConstIterator() = default;

    ConstIterator& operator=(const ConstIterator& iter) { ptr = iter.ptr; return *this; };

    bool operator==(const ConstIterator& iter) const { return ptr.lock().get() == iter.ptr.lock().get(); };

    explicit operator bool() const noexcept { return !ptr.expired(); };

    const T& operator*() const { check_invalid(); return ptr.lock()->key; };
    const T* operator->() const { check_invalid(); return std::addressof(ptr.lock()->key); };

    ConstIterator& operator++() { check_invalid(); ptr = ptr.lock()->next; return *this; };
    ConstIterator operator++(int) { check_invalid(); auto copy = *this; ptr = ptr.lock()->next; return copy; };

    ConstIterator& operator--() { check_invalid(); ptr = ptr.lock()->prev; return *this; };
    ConstIterator operator--(int) { check_invalid(); auto copy = *this; ptr = ptr.lock()->prev; return copy; };

private:
    void check_invalid() const { if (ptr.expired()) throw std::runtime_error("FUCKED UP!"); };
    std::weak_ptr<node> ptr;
};

#endif //TASK_5_ITERATOR
