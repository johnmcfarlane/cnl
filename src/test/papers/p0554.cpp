
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>

namespace {
    using cnl::fixed_point;
    using cnl::_impl::identical;

    namespace bare_metal {
        // sample 1 - bare-metal fixed-point arithmetic
        constexpr auto a = (int8_t)(7.f * 8);      // the value 7 stored in a byte with 3 fractional bits
        constexpr auto b = (int8_t)(3.125f * 16);  // the value 3.125 stored in a byte with 4 fractional bits
        constexpr auto c = a * b;                  // the value 21.875 stored in an `int` with 7 fractional bits
        constexpr auto d = (float)c / 128;         // 21.875f
        static_assert(identical(d, 21.875f), "position_paper test failed");
    }

    namespace type_safe {
        // sample 2 - type-safe fixed-point arithmetic
        constexpr auto a = fixed_point<int8_t, -3>(7.f);     // the value 7 stored in a byte with 3 fractional bits
        constexpr auto b = fixed_point<int8_t, -4>(3.125f);  // the value 3.125 stored in a byte with 4 fractional bits
        constexpr auto c = a * b;                            // the value 21.875 stored in an `int` with 7 fractional bits
        constexpr auto d = (float)c;                         // 21.875f
        static_assert(identical(d, 21.875f), "position_paper test failed");
    }

    namespace division {
        using cnl::elastic_fixed_point;
        constexpr elastic_fixed_point<7, -6> numerator = 0.5, denominator = 1.0;
        constexpr auto quotient = numerator / denominator;
        static_assert(identical(quotient, elastic_fixed_point<14, -7>{.5}), "position_paper test failed");
    }

    namespace division2 {
        using cnl::elastic_fixed_point;
        constexpr elastic_fixed_point<7, -6> numerator = 1.125; // 1:6
        constexpr elastic_fixed_point<5, -3> denominator = 4.;  // 2:3
        constexpr auto quotient = numerator / denominator;
        static_assert(identical(quotient, elastic_fixed_point<12, -8>{0.28125}), "position_paper test failed");
    }
}
