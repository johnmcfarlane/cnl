
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/multiword_integer/operators.h>

#include <cnl/_impl/multiword_integer/operators.h>
#include <cnl/_impl/multiword_integer/type.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_numeric_limits {
        static_assert(
                cnl::numeric_limits<cnl::_impl::multiword_integer<int>>::is_specialized,
                "");
    }

    namespace test_equal {
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<unsigned int, 4>{100000000},
                        cnl::_impl::multiword_integer<unsigned int, 4>{10000}
                        *cnl::_impl::multiword_integer<unsigned int, 4>{10000}),
                "");
    }

    namespace test_equal {
        static_assert(
                cnl::_impl::multiword_integer<int>{0}==cnl::_impl::multiword_integer<int>{},
                "");
        static_assert(
                cnl::_impl::multiword_integer<cnl::uint64>{123}==cnl::_impl::multiword_integer<cnl::uint64>{123},
                "");
        static_assert(
                !(cnl::_impl::multiword_integer<cnl::int8>{123}==cnl::_impl::multiword_integer<cnl::int8>{124}),
                "");
        static_assert(
                !(cnl::_impl::multiword_integer<short>{123}==cnl::_impl::multiword_integer<short>{-124}),
                "");
    }

    namespace test_not_equal {
        static_assert(
                !(cnl::_impl::multiword_integer<int>{0}!=cnl::_impl::multiword_integer<int>{}),
                "");
        static_assert(
                !(cnl::_impl::multiword_integer<cnl::uint64>{123}!=cnl::_impl::multiword_integer<cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::multiword_integer<cnl::int8>{123}!=cnl::_impl::multiword_integer<cnl::int8>{124},
                "");
        static_assert(
                cnl::_impl::multiword_integer<short>{123}!=cnl::_impl::multiword_integer<short>{-124},
                "");
    }

    namespace test_less {
        static_assert(
                !(cnl::_impl::multiword_integer<int>{0}<cnl::_impl::multiword_integer<int>{}),
                "");
        static_assert(
                !(cnl::_impl::multiword_integer<cnl::uint64>{123}<cnl::_impl::multiword_integer<cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::multiword_integer<cnl::int8>{123}<cnl::_impl::multiword_integer<cnl::int8>{124},
                "");
        static_assert(
                !(cnl::_impl::multiword_integer<short>{123}<cnl::_impl::multiword_integer<short>{-124}),
                "");
    }

    namespace test_shift_left {
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<int, 2>{0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::multiword_integer<int, 2>,
                                cnl::_impl::multiword_integer<int, 2>>{}(0, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long int, 4>{0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::multiword_integer<long int, 4>,
                                cnl::_impl::multiword_integer<long int, 4>>{}(0, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long int, 4>{0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::multiword_integer<long int, 4>,
                                cnl::_impl::multiword_integer<long int, 4>>{}(0, 1)),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long int, 2>{1},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::multiword_integer<long int, 2>,
                                cnl::_impl::multiword_integer<long int, 2>>{}(1, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long int, 4>{246},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::multiword_integer<long int, 4>,
                                cnl::_impl::multiword_integer<long int, 4>>{}(123, 1)),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long int, 4>{246},
                        cnl::_impl::multiword_integer<long int, 4>{123}
                                << cnl::_impl::multiword_integer<long int, 4>{1}),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long unsigned, 2>{246},
                        cnl::_impl::multiword_integer<long unsigned, 2>{123}
                                << 1L),
                "");
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<long unsigned, 2>{246},
                        cnl::_impl::multiword_integer<long unsigned, 2>{123}
                                << cnl::_impl::multiword_integer<long unsigned, 2>{1}),
                "");
    }
}
