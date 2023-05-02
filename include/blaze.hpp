#ifndef TASK_5_BLAZE_HPP
#define TASK_5_BLAZE_HPP

namespace Blaze {
    template<class T, class Compare>
    concept comparable_with = requires(const T& t, Compare cmp) {
        { cmp(t, t) } -> std::convertible_to<bool>;
    };

    template<class T, class Less = std::less<T>>
    requires std::copyable<T> && std::semiregular<Less> && comparable_with<T, Less>
    class AVLTree;

    template<class T>
    class ConstIterator;
}

#endif //TASK_5_BLAZE_HPP
