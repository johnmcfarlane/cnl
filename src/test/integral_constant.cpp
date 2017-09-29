
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

    namespace test_literals {
        using cnl::intmax;
        using namespace cnl::literals;
        using std::integral_constant;

        static_assert(identical(
                0b011010000110100000011111101000000010110110101_c,
                integral_constant<intmax, 0b011010000110100000011111101000000010110110101>()),
                "cnl::literals test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                0b1101000011010000001111110100000001011011010101101000011010000001111110100000001011011010101101000011010000001111110100000001011_c,
                integral_constant<intmax, CNL_INTMAX_C(0b1101000011010000001111110100000001011011010101101000011010000001111110100000001011011010101101000011010000001111110100000001011)>()),
                "cnl::literals test failed");
#endif

        static_assert(identical(07777041726354_c, integral_constant<intmax, 07777041726354>()),
                "cnl::literals test failed");

        static_assert(identical(0_c, integral_constant<intmax, CNL_INTMAX_C(0)>()),
            "cnl::literals test failed");
        static_assert(identical(1_c, integral_constant<intmax, CNL_INTMAX_C(1)>()),
            "cnl::literals test failed");
        static_assert(identical(2_c, integral_constant<intmax, CNL_INTMAX_C(2)>()),
            "cnl::literals test failed");
        static_assert(3_c == integral_constant<cnl::int8, CNL_INTMAX_C(3)>(),
            "cnl::literals test failed");
        static_assert(identical(13971581_c, integral_constant<intmax, CNL_INTMAX_C(13971581)>()),
            "cnl::literals test failed");
        static_assert(identical(9081726354_c, integral_constant<intmax, CNL_INTMAX_C(9081726354)>()),
                "cnl::literals test failed");
        static_assert(identical(9223372036854775807_c, integral_constant<intmax, INT64_MAX>()),
            "cnl::literals test failed");

        static_assert(identical(0x10000_c, integral_constant<intmax, 65536>()),
                      "cnl::literals test failed");
        static_assert(identical(0x9081726354_c, integral_constant<intmax, CNL_INTMAX_C(0x9081726354)>()),
                "cnl::literals test failed");
        static_assert(identical(0x91827364564738_c, integral_constant<intmax, 0x91827364564738>()),
                      "cnl::literals test failed");
    }

    namespace test_integral_constant {
        using namespace cnl::literals;
        using std::integral_constant;
        using cnl::intmax;

        static_assert(identical(128_c, integral_constant<intmax, 128>{}), "integral_constant test failed");
        static_assert(identical(127_c, integral_constant<intmax, 127>{}), "integral_constant test failed");
        static_assert(identical(-128_c, integral_constant<intmax, -128>{}), "integral_constant test failed");
        static_assert(identical(-127_c, integral_constant<intmax, -127>{}), "integral_constant test failed");
    }
}
