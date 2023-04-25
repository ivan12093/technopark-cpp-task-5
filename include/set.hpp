#ifndef TASK_5_SET_HPP
#define TASK_5_SET_HPP

#include <memory>

namespace Blaze {
    template<class T>
    class ISet;
};

template<class T>
class Blaze::ISet {
 public:
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;

    virtual void erase(const T& val) = 0;
    virtual void insert(const T& val) = 0;
    virtual bool contains(const T& val) const = 0;

    virtual bool is_disjoint(const ISet &other) const noexcept = 0;

    virtual bool is_equal(const ISet &other) const noexcept = 0;
    virtual bool operator==(const ISet &other) const noexcept = 0;

    virtual bool is_subset(const ISet &other) const noexcept = 0;
    virtual bool operator<=(const ISet &other) const noexcept = 0;

    virtual bool is_superset(const ISet &other) const noexcept = 0;
    virtual bool operator>=(const ISet &other) const noexcept = 0;

    virtual std::unique_ptr<ISet> Union(const ISet &other) const = 0;
    virtual std::unique_ptr<ISet> operator|(const ISet &other) const = 0;

    virtual std::unique_ptr<ISet> intersection(const ISet &other) const = 0;
    virtual std::unique_ptr<ISet> operator&(const ISet &other) const = 0;

    virtual std::unique_ptr<ISet> difference(const ISet &other) const = 0;
    virtual std::unique_ptr<ISet> operator-(const ISet &other) const = 0;

    virtual ~ISet() = 0;
};

template<class T>
Blaze::ISet<T>::ISet::~ISet() = default;

#endif //TASK_5_SET_HPP
