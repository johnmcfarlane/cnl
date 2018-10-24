
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/operators.h>

#include <cnl/_impl/duplex_integer/operators.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_equal {
//        static_assert(
//                cnl::_impl::duplex_integer<int, unsigned>{0} == cnl::_impl::duplex_integer<int, unsigned>{},
//                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123} ==
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123} ==
                  cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123} ==
                  cnl::_impl::duplex_integer<short, unsigned short>{-124}),
                "");
    }

    namespace test_not_equal {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0} != cnl::_impl::duplex_integer<int, unsigned>{}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123} !=
                  cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{512} !=
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{1024},
                "");
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123} !=
                  cnl::_impl::duplex_integer<short, unsigned short>{-124},
                "");
    }

    namespace test_less {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0} < cnl::_impl::duplex_integer<int, unsigned>{}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123} <
                  cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123} <
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124},
                "");
//        static_assert(
//                !(cnl::_impl::duplex_integer<short, unsigned short>{123} <
//                  cnl::_impl::duplex_integer<short, unsigned short>{-124}),
//                "");
    }
}
