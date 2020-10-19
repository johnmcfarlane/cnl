
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_scaled_integer.h>

namespace {
    using cnl::power;
    using cnl::scaled_integer;
    using cnl::_impl::identical;

    namespace bare_metal {
        // sample 1 - bare-metal fixed-point arithmetic
        constexpr auto a{int8_t(7.F * 8)};      // the value 7 stored in a byte with 3 fraction bits
        constexpr auto b{int8_t(3.125F * 16)};  // the value 3.125 stored in a byte with 4 fraction bits
        constexpr auto c = a * b;                  // the value 21.875 stored in an `int` with 7 fraction bits
        constexpr auto d{float{c} / 128};         // 21.875f
        static_assert(identical(d, 21.875F), "position_paper test failed");
    }

    namespace type_safe {
        // sample 2 - type-safe fixed-point arithmetic
        constexpr auto a = scaled_integer<int8_t, power<-3>>(7.F);     // the value 7 stored in a byte with 3 fraction bits
        constexpr auto b = scaled_integer<int8_t, power<-4>>(3.125F);  // the value 3.125 stored in a byte with 4 fraction bits
        constexpr auto c = a * b;                            // the value 21.875 stored in an `int` with 7 fraction bits
        constexpr auto d = float(c);                         // 21.875f
        static_assert(identical(d, 21.875F), "position_paper test failed");
    }

    namespace division {
        using cnl::elastic_scaled_integer;
        constexpr elastic_scaled_integer<7, -6> numerator = 0.5, denominator = 1.0;
        constexpr auto quotient = numerator / denominator;
        static_assert(identical(quotient, elastic_scaled_integer<7, 0>{0}), "position_paper test failed");
    }

    namespace division2 {
        using cnl::elastic_scaled_integer;
        using cnl::scaled_integer;  // NOLINT(misc-unused-using-decls)
        using cnl::fraction;  // NOLINT(misc-unused-using-decls)
        constexpr elastic_scaled_integer<7, -6> numerator = 1.125; // 1:6
        constexpr elastic_scaled_integer<5, -3> denominator = 4.;  // 2:3
        constexpr auto quotient = scaled_integer{fraction{numerator, denominator}};
        static_assert(identical(quotient, elastic_scaled_integer<12, -8>{0.28125}), "position_paper test failed");
    }
}
