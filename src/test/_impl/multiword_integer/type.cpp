
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/multiword_integer/type.h>

#include <cnl/_impl/multiword_integer/type.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_rep {
        static_assert(
                assert_same<
                        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
                        cnl::_impl::multiword_integer_rep<cnl::int32, 2>::type>::value,
                "");
        static_assert(
                assert_same<
                        cnl::_impl::duplex_integer<
                                cnl::int32,
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>,
                        cnl::_impl::multiword_integer_rep<cnl::int32, 3>::type>::value,
                "");
    }
    namespace test_parameters {
//        static_assert(
//                identical(123, cnl::_impl::multiword_integer<int, 123>::size),
//                "cnl::_impl::multiword_integer::size");
    }

    namespace test_bool {
        static_assert(!cnl::_impl::multiword_integer<int, 2>{}, "constructor taking multiword_integer");
        static_assert(!cnl::_impl::multiword_integer<int, 3>{}, "constructor taking multiword_integer");
//        static_assert(!cnl::_impl::multiword_integer<cnl::uint8, 2>{0}, "constructor taking multiword_integer");
//        static_assert(cnl::_impl::multiword_integer<cnl::uint8, 2>{0x1234}, "constructor taking multiword_integer");
//        static_assert(cnl::_impl::multiword_integer<cnl::int64, 2>{0x1234}, "constructor taking multiword_integer");
    }

//    namespace test_equal {
//        static_assert(
//                cnl::_impl::multiword_integer<int>{0}==cnl::_impl::multiword_integer<int>{},
//                "");
//        static_assert(
//                cnl::_impl::multiword_integer<cnl::uint64>{123}==cnl::_impl::multiword_integer<cnl::uint64>{123},
//                "");
//        static_assert(
//                !(cnl::_impl::multiword_integer<cnl::int8>{123}==cnl::_impl::multiword_integer<cnl::int8>{124}),
//                "");
//        static_assert(
//                !(cnl::_impl::multiword_integer<short>{123}==cnl::_impl::multiword_integer<short>{-124}),
//                "");
//    }
//
//    namespace test_not_equal {
//        static_assert(
//                !(cnl::_impl::multiword_integer<int>{0}!=cnl::_impl::multiword_integer<int>{}),
//                "");
//        static_assert(
//                !(cnl::_impl::multiword_integer<cnl::uint64>{123}!=cnl::_impl::multiword_integer<cnl::uint64>{123}),
//                "");
//        static_assert(
//                cnl::_impl::multiword_integer<cnl::int8>{123}!=cnl::_impl::multiword_integer<cnl::int8>{124},
//                "");
//        static_assert(
//                !(cnl::_impl::multiword_integer<short>{123}!=cnl::_impl::multiword_integer<short>{-124}),
//                "");
//    }
//
//    namespace test_less {
//        static_assert(
//                !(cnl::_impl::multiword_integer<int>{0}<cnl::_impl::multiword_integer<int>{}),
//                "");
//        static_assert(
//                !(cnl::_impl::multiword_integer<cnl::uint64>{123}<cnl::_impl::multiword_integer<cnl::uint64>{123}),
//                "");
//        static_assert(
//                cnl::_impl::multiword_integer<cnl::int8>{123}<cnl::_impl::multiword_integer<cnl::int8>{124},
//                "");
//        static_assert(
//                !(cnl::_impl::multiword_integer<short>{123}<cnl::_impl::multiword_integer<short>{-124}),
//                "");
//    }
}
