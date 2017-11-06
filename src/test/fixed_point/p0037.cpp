
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if __cpp_inline_variables>=201606

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::_impl::identical;
using std::declval;

template<class T, class U>
inline constexpr bool is_same_v = std::is_same<T, U>::value;

////////////////////////////////////////////////////////////////////////////////
// Tests of Examples in P0037

namespace design_decisions {
    namespace conversions {
        using cnl::fixed_point;

        static_assert(fixed_point<int, -1>{.499}==0.0);
    }

    namespace access_to_rep_value {
        using cnl::fixed_point;
        using cnl::from_rep;
        using cnl::to_rep;

        constexpr auto a = from_rep<fixed_point<int, -8>>()(320);
        static_assert(a == 1.25);

        constexpr auto b = to_rep<fixed_point<int, -8>>()(a);
        static_assert(b == 320);    // 1.25*(1<<8)
    }

    namespace deduction {
        using cnl::constant;
        using cnl::fixed_point;
        using cnl::to_rep;

        auto a = fixed_point(0ul);
        static_assert(is_same_v<decltype(a), fixed_point<unsigned long, 0>>);

        constexpr auto b = fixed_point(constant<0xFF00000000L>{});
        static_assert(is_same_v<decltype(b), const fixed_point<int, 32>>);
        static_assert(to_rep<fixed_point<int, 32>>()(b) == 0xFF);
    }

    namespace operators {
        using cnl::constant;
        using cnl::fixed_point;

        auto a = fixed_point<long long>(3) + 4.f;
        static_assert(is_same_v<decltype(a), decltype(3.f + 4.f)>);

#if ! defined(__clang__)
        // TODO: either this must compile on Clang or the operator must be written
        auto b = fixed_point(200U) - constant<100L>{};
        static_assert(is_same_v<decltype(b), decltype(fixed_point<unsigned>(200) - fixed_point<int>(100))>);
#endif

        auto c = fixed_point<>(5) * 6ul;
        static_assert(is_same_v<decltype(c), decltype(fixed_point<>(5) * fixed_point<unsigned long>(6))>);

        constexpr auto d = fixed_point<uint8_t, -7>{1.25} * fixed_point<uint8_t, -3>{8};
        static_assert(is_same_v<decltype(d), const fixed_point<int, -10>>);
        static_assert(d == 10);

        constexpr auto e = fixed_point<short, -5>{1.5} / fixed_point<short, -3>{2.5};
        static_assert(is_same_v<decltype(e), const fixed_point<int, -2>>);
        static_assert(e == .5);

        constexpr auto f = fixed_point<short, -5>{1.5} % fixed_point<short, -3>{2.5};
        static_assert(is_same_v<decltype(f), const fixed_point<int, -5>>);
        static_assert(f == .25);

        // numerator = quotient * denominator + remainder
        constexpr auto f2 = e * fixed_point<short, -3>{2.5} + f;
        static_assert(is_same_v<decltype(f2), const fixed_point<int, -5>>);
        static_assert(f2 == fixed_point<short, -5>{1.5});

        constexpr auto g = fixed_point<short, -2>{12.5} - fixed_point<uint8_t, 0>{8};
        static_assert(is_same_v<decltype(g), const fixed_point<int, -2>>);
        static_assert(g == 4.5);

        constexpr auto h = fixed_point<int8_t, -2>{12.5} <= fixed_point<short, 0>{8};
        static_assert(is_same_v<decltype(h), const bool>);
        static_assert(h == false);
    }

    namespace named_arithmetic {
        using cnl::fixed_point;
        using cnl::multiply;

        namespace named_arithmetic1 {
            constexpr auto f = fixed_point<uint8_t, -4>{15.9375};
            constexpr auto p = multiply(f, f);

            static_assert(identical(p, fixed_point<uint16_t, -8>{254.00390625}));
        }

        TEST(proposal, named_arithmetic2)
        {
            using cnl::fixed_point;
            auto f = fixed_point<unsigned, -28>{15.9375};
            auto p = f * f;

            static_assert(is_same_v<decltype(p), fixed_point<unsigned, -56>>);
            ASSERT_EQ(p, 0.);
        }

        namespace named_arithmetic3 {
            constexpr auto f = fixed_point<unsigned, -28>{15.9375};
            constexpr auto p = multiply(f, f);

            static_assert(identical(p, fixed_point<uint64_t, -56>{254.00390625}),
                    "Incorrect information in proposal section, Named Arithmetic Functions");
        }

        namespace named_arithmetic4 {
            constexpr auto n = fixed_point<uint32_t, -16>{1};
            constexpr auto d = fixed_point<uint32_t, -16>{2};

            // information appears to be lost by division operator
            constexpr auto q1 = n / d;
            static_assert(is_same_v<decltype(q1), const fixed_point<uint32_t, 0>>);
            static_assert(q1 == 0);

            // but divide preserves accurate results where practical
            constexpr auto q2 = divide(n, d);
            static_assert(is_same_v<decltype(q2), const fixed_point<uint64_t, -32>>);
            static_assert(q2 == 0.5);
        }
    }
}

namespace example {
    using cnl::fixed_point;

    template<class Fp>
    constexpr auto magnitude(Fp x, Fp y, Fp z)
    -> decltype(sqrt(x*x+y*y+z*z))
    {
        return sqrt(x*x+y*y+z*z);
    }

    TEST(proposal, examples)
    {
        constexpr auto m = magnitude(
                fixed_point<uint16_t, -12>(1),
                fixed_point<uint16_t, -12>(4),
                fixed_point<uint16_t, -12>(9));
        static_assert(identical(m, fixed_point<int, -24>{9.8994948864}), "Incorrect information in proposal section, Examples");
    }
}

namespace open_issues {
    namespace extended_comparison_range {
        using cnl::fixed_point;

        constexpr auto lhs = fixed_point<uint8_t, 0>{0};
        static_assert(!lhs);

        constexpr auto rhs = fixed_point<uint8_t, 128>{4.e38};
        static_assert(rhs);

        //static_assert(lhs < rhs);
    }
}

#endif  // __cpp_inline_variables>201606
