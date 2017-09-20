
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/integral_constant.h` definitions

#include <cnl/bits/type_traits.h>
#include <cnl/integral_constant.h>

namespace {
    using cnl::_impl::identical;

    namespace test_const_integer_impl {

        namespace test_digits_to_integral {
            using cnl::_const_integer_impl::parse;

            static_assert(identical(parse("0"), INTMAX_C(0)),
                "cnl::_const_integer_impl::parse test failed");
            static_assert(identical(parse("1"), INTMAX_C(1)),
                "cnl::_const_integer_impl::parse test failed");
            static_assert(identical(parse("9081726354"), INTMAX_C(9081726354)),
                "cnl::_const_integer_impl::parse test failed");
            static_assert(identical(parse("0x9081726354"), INTMAX_C(0x9081726354)),
                "cnl::_const_integer_impl::parse test failed");
            static_assert(parse("0XaA") == INTMAX_C(0xAa),
                "cnl::_const_integer_impl::digits_to_integral test failed");
        }
    }

    namespace test_literals {
        using namespace cnl::literals;
        using std::integral_constant;

        static_assert(identical(0_c, integral_constant<intmax_t, INTMAX_C(0)>()),
            "cnl::literals test failed");
        static_assert(identical(1_c, integral_constant<intmax_t, INTMAX_C(1)>()),
            "cnl::literals test failed");
        static_assert(identical(2_c, integral_constant<intmax_t, INTMAX_C(2)>()),
            "cnl::literals test failed");
        static_assert(3_c == integral_constant<std::int8_t, INTMAX_C(3)>(),
            "cnl::literals test failed");
        static_assert(identical(13971581_c, integral_constant<intmax_t, INTMAX_C(13971581)>()),
            "cnl::literals test failed");
        static_assert(identical(9223372036854775807_c, integral_constant<intmax_t, INT64_MAX>()),
            "cnl::literals test failed");

        static_assert(identical(0x10000_c, integral_constant<intmax_t, 65536>()),
                      "cnl::literals test failed");
        static_assert(identical(0x91827364564738_c, integral_constant<intmax_t, 0x91827364564738>()),
                      "cnl::literals test failed");
    }
}
