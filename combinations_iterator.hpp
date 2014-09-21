// (C) Copyright Tim Prince 2014.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef COMBINATIONS_ITERATOR_HPP
#define COMBINATIONS_ITERATOR_HPP

#include <algorithm>
#include <array>
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <stdexcept>
#include <vector>

namespace boost
{

template<size_t Order, class Iterator, class ValueT>
class combinations_iterator;

namespace detail
{

template<size_t Order, class Iterator, class ValueT>
struct combinations_iterator_base
{
    typedef iterator_adaptor<
        combinations_iterator<Order, Iterator, ValueT>,
        Iterator,
        ValueT,
        typename mpl::if_<
            is_convertible<
              typename iterator_traversal<Iterator>::type,
              random_access_traversal_tag
            >,
          bidirectional_traversal_tag,
          use_default
        >::type,
        ValueT
    > type;
};

template<size_t N, size_t Order>
struct init_value_helper
{
    template<class ValueT, class Array>
    static void set(ValueT& v, const Array& a)
    {
        std::get<N>(v) = *a[N];
        init_value_helper<N+1, Order>::set(v, a);
    }
};

template<size_t Order>
struct init_value_helper<Order, Order>
{
    template<class ValueT, class Array>
    static void set(ValueT&, const Array&) { }
};

template<class ValueT, class It, size_t Order>
void init_value(ValueT& v, const std::array<It, Order>& a)
{
    return init_value_helper<0, Order>::set(v, a);
}

} /* namespace detail */

template<
    size_t Order,
    class Iterator, 
    class ValueT = 
        std::array<
            typename std::iterator_traits<Iterator>::value_type, 
            Order
        >
    >
class combinations_iterator :
    public detail::combinations_iterator_base<
        Order,
        Iterator, 
        ValueT
    >::type
{
    typedef typename detail::combinations_iterator_base<
        Order,
        Iterator, 
        ValueT
    >::type base;

    friend class iterator_core_access;

public:
    combinations_iterator() { }
    combinations_iterator(Iterator begin, Iterator end) :
        base(begin), end_(end)
    {
        Iterator it = begin;
        for (size_t i = 0; i < Order; ++i)
        {
            it_[i] = it;
            if (it != end) ++it;
        }
    }

    template<class OtherIterator, class OtherValueT>
    combinations_iterator(
        const combinations_iterator<Order, OtherIterator, OtherValueT>& t,
        typename enable_if_convertible<OtherIterator, Iterator>::type* = 0,
        typename enable_if_convertible<OtherValueT, ValueT>::type* = 0) : 
        base(t.base()), end_(t.end()) 
    { 
        std::copy(t.state().begin(), t.state().end(), it_.begin());
    }

    const std::vector<Iterator>& state() const { return it_; }
    Iterator end() const { return end_; }

private:
    template<class OtherIterator, class OtherValueT>
    bool equal(
        const combinations_iterator<Order, OtherIterator, OtherValueT>& t) const
    {
        return
            (atEnd() && t.atEnd()) ||
            (end() == t.end() && it_ == t.it_);
    }

    bool atEnd() const
    {
        for (int i = Order - 1; i >= 0; i--)
        {
            if (it_[i] == end_) return true;
        }
        return false;
    }

    Iterator increment(size_t n)
    {
        auto end = (n + 1) < Order ? it_[n + 1] : end_;
        auto& j = it_[n];
        auto i = (n - 1) > 0 ? it_[n - 1] : it_[0];

        if (std::next(j) == end)
        {
            j = (n > 0) ? increment(n - 1) : end_;
            return j != end_ ? std::next(j) : end_;
        }

        j++;
        return std::next(j);
    }

    void increment()
    {
        (void)increment(Order - 1);
    }

    ValueT dereference() const 
    {
        ValueT result;
        detail::init_value(result, it_);
        return result;
    }

    std::array<Iterator, Order> it_;
    Iterator end_;
};

template<size_t Order, class It>
combinations_iterator<Order, It>
make_combinations_iterator(It it, It end)
{
    return combinations_iterator<Order, It>(it, end);
}

} /* namespace boost */

#endif /* COMBINATIONS_ITERATOR_HPP */
