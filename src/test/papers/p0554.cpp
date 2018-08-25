
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_number.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::fixed_point;
    using cnl::_impl::identical;

    namespace bare_metal {
        // sample 1 - bare-metal fixed-point arithmetic
        constexpr auto a = (int8_t)(7.f * 8);      // the value 7 stored in a byte with 3 fraction bits
        constexpr auto b = (int8_t)(3.125f * 16);  // the value 3.125 stored in a byte with 4 fraction bits
        constexpr auto c = a * b;                  // the value 21.875 stored in an `int` with 7 fraction bits
        constexpr auto d = (float)c / 128;         // 21.875f
        static_assert(identical(d, 21.875f), "position_paper test failed");
    }

    namespace type_safe {
        // sample 2 - type-safe fixed-point arithmetic
        constexpr auto a = fixed_point<int8_t, -3>(7.f);     // the value 7 stored in a byte with 3 fraction bits
        constexpr auto b = fixed_point<int8_t, -4>(3.125f);  // the value 3.125 stored in a byte with 4 fraction bits
        constexpr auto c = a * b;                            // the value 21.875 stored in an `int` with 7 fraction bits
        constexpr auto d = (float)c;                         // 21.875f
        static_assert(identical(d, 21.875f), "position_paper test failed");
    }

    namespace division {
        using cnl::elastic_number;
        constexpr elastic_number<7, -6> numerator = 0.5, denominator = 1.0;
        constexpr auto quotient = numerator / denominator;
        static_assert(identical(quotient, elastic_number<7, 0>{0}), "position_paper test failed");
    }

#if defined(__cpp_deduction_guides)
    namespace division2 {
        using cnl::elastic_number;
        using cnl::fixed_point;
        using cnl::fraction;
        constexpr elastic_number<7, -6> numerator = 1.125; // 1:6
        constexpr elastic_number<5, -3> denominator = 4.;  // 2:3
        constexpr auto quotient = fixed_point{fraction{numerator, denominator}};
        static_assert(identical(quotient, elastic_number<12, -8>{0.28125}), "position_paper test failed");
    }
#endif
}
