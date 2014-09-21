// (C) Copyright Tim Prince 2014.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef PAIRS_ITERATOR_HPP
#define PAIRS_ITERATOR_HPP

#include "combinations_iterator.hpp"

namespace boost
{

template<
    class Iterator, 
    class ValueT = std::pair<
        typename std::iterator_traits<Iterator>::value_type,
        typename std::iterator_traits<Iterator>::value_type
    >
>
using pairs_iterator = combinations_iterator<2, Iterator, ValueT>;

template<class It>
pairs_iterator<It>
make_pairs_iterator(It it, It end)
{
    return pairs_iterator<It>(it, end);
}

} /* namespace boost */

#endif /* PAIRS_ITERATOR_HPP */
