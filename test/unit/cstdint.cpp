
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
        static_assert(identical(cnl::intmax{1}, CNL_INTMAX_C(1)), "");
        static_assert(identical(cnl::intmax{-5}, CNL_INTMAX_C(-0b101)), "");
        static_assert(identical(cnl::intmax{32}, CNL_INTMAX_C(+0x20)), "");
        static_assert(identical(cnl::intmax{-83}, CNL_INTMAX_C(-0123)), "");
        static_assert(identical(cnl::intmax{1233456789012345678LL}, CNL_INTMAX_C(1233456789012345678)), "");
        static_assert(identical(cnl::intmax{-1233456789012345678LL}, CNL_INTMAX_C(-1233456789012345678)), "");
    }

    namespace test_parse {
        using cnl::_cnlint_impl::parse;

        static_assert(identical(parse("0"), CNL_INTMAX_C(0)),
                "cnl::_cnlint_impl::parse test failed");
        static_assert(identical(parse("1"), CNL_INTMAX_C(1)),
                "cnl::_cnlint_impl::parse test failed");
        static_assert(identical(parse("9081726354"), CNL_INTMAX_C(9081726354)),
                "cnl::_cnlint_impl::parse test failed");
        static_assert(identical(parse("0x9081726354"), CNL_INTMAX_C(0x9081726354)),
                "cnl::_cnlint_impl::parse test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                parse("0x123456789ABCDEF0123456789ABCDEF"),
                CNL_INTMAX_C(0x123456789ABCDEF0123456789ABCDEF)),
                "cnl::_cnlint_impl::parse test failed");
        static_assert(identical(
                cnl::uint128{parse("0x123456789ABCDEF0123456789ABCDEF")},
                CNL_UINTMAX_C(0x123456789ABCDEF0123456789ABCDEF)),
                "cnl::_cnlint_impl::parse test failed");
#endif
        static_assert(identical(parse("07777041726354"), CNL_INTMAX_C(07777041726354)),
                "cnl::_cnlint_impl::parse test failed");
        static_assert(identical(
                parse("0b011010000110100000011111101000000010110110101"),
                CNL_INTMAX_C(0b011010000110100000011111101000000010110110101)),
                "cnl::_cnlint_impl::parse test failed");
        static_assert(parse("0XaA") == CNL_INTMAX_C(0xAa),
                "cnl::_cnlint_impl::digits_to_integral test failed");
    }
}
