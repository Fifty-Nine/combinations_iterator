// (C) Copyright Tim Prince 2014.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef PAIRS_ITERATOR_HPP
#define PAIRS_ITERATOR_HPP

#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/static_assert.hpp>

namespace boost
{

template<class Iterator, class ValueT>
class pairs_iterator;

namespace detail
{

template<class Iterator, class ValueT>
struct pairs_iterator_base
{
    typedef iterator_adaptor<
        pairs_iterator<Iterator, ValueT>,
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

} /* namespace detail */

template<
    class Iterator, 
    class ValueT = 
        std::pair<
            typename std::iterator_traits<Iterator>::value_type, 
            typename std::iterator_traits<Iterator>::value_type
        >
    >
class pairs_iterator :
    public detail::pairs_iterator_base<
        Iterator, 
        ValueT
    >::type
{
    typedef typename detail::pairs_iterator_base<
        Iterator, 
        ValueT
    >::type base;

    friend class iterator_core_access;

public:
    pairs_iterator() { }
    pairs_iterator(Iterator begin, Iterator end = Iterator()) :
        base(begin), i_(begin), j_((begin != end) ? begin + 1 : end), end_(end)
    {
    }

    template<class OtherIterator, class OtherValueT>
    pairs_iterator(
        const pairs_iterator<OtherIterator, OtherValueT>& t,
        typename enable_if_convertible<OtherIterator, Iterator>::type* = 0,
        typename enable_if_convertible<OtherValueT, ValueT>::type* = 0) : 
        base(t.base()), i_(t.i()), j_(t.j()), end_(t.end()) 
    { }

    Iterator i() const { return i_; }
    Iterator j() const { return j_; }
    Iterator end() const { return end_; }
private:
    template<class OtherIterator, class OtherValueT>
    bool equal(const pairs_iterator<OtherIterator, OtherValueT>& t) const
    {
        return end() == t.end() &&
            i() == t.i() &&
            j() == t.j();
    }
    void increment()
    {
        ++j_;

        if (j_ == end_)
        {
            j_ = ++i_;
            if (j_ != end_)
            {
                ++j_;
            }

            if (j_ == end_) ++i_;
        }
    }
    void decrement() 
    {
    }

    ValueT dereference() const 
    { 
        return (ValueT){ *i_, *j_ }; 
    }

    Iterator i_;
    Iterator j_;
    Iterator end_;
};

template<class It>
pairs_iterator<It>
make_pairs_iterator(It it, It end)
{
    return pairs_iterator<It>(it, end);
}

} /* namespace boost */

#endif /* PAIRS_ITERATOR_HPP */
