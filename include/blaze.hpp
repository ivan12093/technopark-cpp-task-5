#ifndef TASK_5_BLAZE_HPP
#define TASK_5_BLAZE_HPP

#include <concepts>
#include <functional>

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

    template <typename T>
    concept iterable = requires(const T& t) {
        typename T::value_type;
        typename T::size_type;
        typename T::iterator;

        typename std::iterator_traits<typename T::iterator>::value_type;
        typename std::iterator_traits<typename T::iterator>::value_type;

        requires std::incrementable<typename T::iterator>;

        // Наличие у итераторов оператора *
        requires std::indirectly_readable<typename T::iterator>;
        requires std::equality_comparable<typename T::iterator>;

        { std::begin(t) } -> std::convertible_to<typename T::iterator>;
        { std::end(t) } -> std::convertible_to<typename T::iterator>;
    };

    template<typename C>
    concept has_contains = requires(const C& c) {
        { c.contains(std::declval<typename C::value_type>()) } -> std::convertible_to<bool>;
    };
}

#endif //TASK_5_BLAZE_HPP
