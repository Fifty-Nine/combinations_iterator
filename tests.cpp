#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <cstdio>

#include <boost/concept_check.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/test/unit_test.hpp>
#include <list>
#include <set>
#include <vector>

#include "combinations_iterator.hpp"
#include "pairs_iterator.hpp"

using namespace boost;
using namespace boost_concepts;

struct test_init
{
    test_init() { }
};

BOOST_GLOBAL_FIXTURE(test_init);

BOOST_AUTO_TEST_SUITE(pairs_iterator_tests)


void concept_check()
{
    typedef std::tuple<int, int, int> Foo;
    typedef std::list<int>::iterator ListIt;
    typedef std::set<int>::iterator SetIt;
    typedef std::vector<int>::iterator VectorIt;

    typedef pairs_iterator<ListIt> CListIt;
    typedef pairs_iterator<SetIt> CSetIt;
    typedef pairs_iterator<VectorIt> CVectorIt;
    
    typedef pairs_iterator<ListIt, Foo> CFListIt;
    typedef pairs_iterator<SetIt, Foo> CFSetIt;
    typedef pairs_iterator<VectorIt, Foo> CFVectorIt;

    BOOST_CONCEPT_ASSERT((ForwardTraversalConcept<CListIt>));
    BOOST_CONCEPT_ASSERT((ReadableIteratorConcept<CListIt>));
    
    BOOST_CONCEPT_ASSERT((ForwardTraversalConcept<CSetIt>));
    BOOST_CONCEPT_ASSERT((ReadableIteratorConcept<CSetIt>));
    
    BOOST_CONCEPT_ASSERT((ForwardTraversalConcept<CVectorIt>));
    BOOST_CONCEPT_ASSERT((ReadableIteratorConcept<CVectorIt>));
    
    BOOST_CONCEPT_ASSERT((ForwardTraversalConcept<CFListIt>));
    BOOST_CONCEPT_ASSERT((ReadableIteratorConcept<CFListIt>));
    
    BOOST_CONCEPT_ASSERT((ForwardTraversalConcept<CFSetIt>));
    BOOST_CONCEPT_ASSERT((ReadableIteratorConcept<CFSetIt>));
    
    BOOST_CONCEPT_ASSERT((ForwardTraversalConcept<CFVectorIt>));
    BOOST_CONCEPT_ASSERT((ReadableIteratorConcept<CFVectorIt>));
}

BOOST_AUTO_TEST_CASE(Simple)
{
    int in[] = { 0, 1, 2, 3, 4 };

    typedef std::pair<int, int> ValueT;
    typedef std::vector<ValueT> Container;

    int* begin = in;
    int* end = in + sizeof(in)/sizeof(int);

    Container out(
        make_pairs_iterator(begin, end),
        make_pairs_iterator(end, end)
    );

    BOOST_REQUIRE_EQUAL(out.size(), 10);
    Container::const_iterator it = out.begin();
    for (int i = 0; i < 5; ++i)
    {
        for (int j = i + 1; j < 5; ++j)
        {
            BOOST_CHECK_EQUAL(i, it->first);
            BOOST_CHECK_EQUAL(j, it->second);
            ++it;
        }
    }
}

BOOST_AUTO_TEST_CASE(Subset)
{
    std::vector<int> in(
        make_counting_iterator(0),
        make_counting_iterator(10)
    );
   
    typedef std::pair<int, int> ValueT;
    typedef std::vector<ValueT> Container;
    Container out(
        make_pairs_iterator(in.begin() + 3, in.end() - 1),
        make_pairs_iterator(in.end() - 1, in.end() - 1)
    );

    BOOST_REQUIRE_EQUAL(out.size(), 15);
    Container::const_iterator it = out.begin();
    for (int i = 3; i < 9; ++i)
    {
        for (int j = i + 1; j < 9; ++j)
        {
            BOOST_CHECK_EQUAL(i, it->first);
            BOOST_CHECK_EQUAL(j, it->second);
            ++it;
        }
    }
}

BOOST_AUTO_TEST_CASE(Empty)
{
    std::vector<int> in;

    typedef std::pair<int, int> ValueT;
    typedef std::vector<ValueT> Container;
    Container out(
        make_pairs_iterator(in.begin(), in.end()),
        make_pairs_iterator(in.end(), in.end())
    );

    BOOST_CHECK_EQUAL(out.size(), 0);
}

BOOST_AUTO_TEST_CASE(AlmostEmpty)
{
    std::vector<int> in(
        make_counting_iterator(0),
        make_counting_iterator(2)
    );

    typedef std::array<int, 4> ValueT;
    typedef std::vector<ValueT> Container;

    Container out(
        make_combinations_iterator<4>(in.begin(), in.end()),
        make_combinations_iterator<4>(in.end(), in.end())
    );

    BOOST_CHECK_EQUAL(out.size(), 0);
}

BOOST_AUTO_TEST_CASE(Triples)
{
    std::vector<int> in(
        make_counting_iterator(1),
        make_counting_iterator(6)
    );

    typedef std::array<int, 3> ValueT;
    typedef std::vector<ValueT> Container;
    Container out(
        make_combinations_iterator<3>(in.begin(), in.end()),
        make_combinations_iterator<3>(in.end(), in.end())
    );

    BOOST_CHECK_EQUAL(out.size(), 10);
}

BOOST_AUTO_TEST_SUITE_END()
