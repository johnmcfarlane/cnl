
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/bits/common.h` definitions

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cstdint.h>

namespace {
    using cnl::_impl::identical;

    namespace test_cnl_intmax_c {
        static_assert(identical(cnl::intmax{1}, CNL_INTMAX_C(1)));
        static_assert(identical(cnl::intmax{-5}, CNL_INTMAX_C(-0b101)));
        static_assert(identical(cnl::intmax{32}, CNL_INTMAX_C(+0x20)));
        static_assert(identical(cnl::intmax{-83}, CNL_INTMAX_C(-0123)));
        static_assert(
                identical(cnl::intmax{1233456789012345678LL}, CNL_INTMAX_C(1233456789012345678)));
        static_assert(
                identical(cnl::intmax{-1233456789012345678LL}, CNL_INTMAX_C(-1233456789012345678)));
    }

    namespace test_parse {
        using cnl::_impl::parse;

        static_assert(
                identical(parse<cnl::intmax, '0'>(), CNL_INTMAX_C(0)));
        static_assert(
                identical(parse<cnl::intmax, '1'>(), CNL_INTMAX_C(1)));
        static_assert(identical(
                parse<cnl::intmax, '9', '0', '8', '1', '7', '2', '6', '3', '5', '4'>(),
                CNL_INTMAX_C(9081726354)));
        static_assert(identical(
                parse<cnl::intmax, '0', 'x', '9', '0', '8', '1', '7', '2', '6', '3', '5', '4'>(),
                CNL_INTMAX_C(0x9081726354)));
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                parse<cnl::intmax, '0', 'x', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'>(),
                CNL_INTMAX_C(0x123456789ABCDEF0123456789ABCDEF)));
        static_assert(identical(
                cnl::uint128{parse<cnl::intmax, '0', 'x', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'>()},
                CNL_UINTMAX_C(0x123456789ABCDEF0123456789ABCDEF)));
#endif
        static_assert(identical(
                parse<cnl::intmax, '0', '7', '7', '7', '7', '0', '4', '1', '7', '2', '6', '3', '5', '4'>(),
                CNL_INTMAX_C(07777041726354)));
        static_assert(identical(
                parse<cnl::intmax, '0', 'b', '0', '1', '1', '0', '1', '0', '0', '0', '0', '1', '1', '0', '1', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1', '1', '0', '1', '1', '0', '1', '0', '1'>(),
                CNL_INTMAX_C(0b011010000110100000011111101000000010110110101)));
        static_assert(
                identical(parse<cnl::intmax, '0', 'X', 'a', 'A'>(), CNL_INTMAX_C(0xAa)));
    }
}
