
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/type.h>

#include <cnl/_impl/duplex_integer/type.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_parameters {
//        static_assert(
//                identical(123, cnl::_impl::duplex_integer<int, 123>::size),
//                "cnl::_impl::multiword_integer::size");
    }

    namespace test_upper {
        static_assert(
                identical(
                        cnl::int8{0},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{}.upper()),
                "");
        static_assert(
                identical(
                        cnl::int8{0},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{0}.upper()),
                "");
        static_assert(
                identical(
                        cnl::int8{0},
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{650}.upper()),
                "");
//        static_assert(
//                identical(
//                        cnl::int8{-1},
//                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{-650}.upper()),
//                "");
//        static_assert(
//                identical(
//                        short{0},
//                        cnl::_impl::duplex_integer<short, unsigned short>{123}.upper()),
//                "");
    }

    namespace test_lower {
        static_assert(
                identical(
                        0u,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{}.lower()),
                "");
        static_assert(
                identical(
                        0u,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{0}.lower()),
                "");
        static_assert(
                identical(
                        650u,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{650}.lower()),
                "");
        static_assert(
                identical(
                        650u,
                        cnl::_impl::duplex_integer<cnl::int8, unsigned>{-650}.lower()),
                "");
    }

    namespace test_bool {
        static_assert(!cnl::_impl::duplex_integer<int, unsigned>{}, "");
        static_assert(!cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0}, "");
        static_assert(cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0x1234}, "");
        static_assert(cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>{0x1234}, "");
    }
}
