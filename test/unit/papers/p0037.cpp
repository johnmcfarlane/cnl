
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/scaled_integer.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::_impl::identical;
using cnl::power;

template<class T, class U>
inline constexpr bool is_same_v = std::is_same<T, U>::value;

namespace {
    static_assert(cnl::numeric_limits<cnl::scaled_integer<>>::is_specialized, "");
    static_assert(cnl::_impl::wants_generic_ops<cnl::scaled_integer<>>::value, "");
    static_assert(cnl::_impl::enable_binary<cnl::scaled_integer<>, double>::value, "");
}

////////////////////////////////////////////////////////////////////////////////
// Tests of Examples in P0037

namespace design_decisions {
    namespace conversions {
        using cnl::scaled_integer;

        static_assert(scaled_integer<int, power<-1>>{.499}==0.0);
    }

    namespace access_to_rep_value {
        using cnl::scaled_integer;
        using cnl::from_rep;

        constexpr auto a = from_rep<scaled_integer<int, power<-8>>, int>{}(320);
        static_assert(a == 1.25);

        constexpr auto b = to_rep(a);
        static_assert(b == 320);    // 1.25*(1<<8)
    }

    namespace deduction {
        using cnl::constant;
        using cnl::scaled_integer;

        auto a = scaled_integer(0UL);
        static_assert(is_same_v<decltype(a), scaled_integer<unsigned long, power<>>>);

        constexpr auto b = scaled_integer(constant<0xFF00000000L>{});
        static_assert(is_same_v<decltype(b), const scaled_integer<int, power<32>>>);
        static_assert(to_rep(b) == 0xFF);
    }

    namespace operators {
        using cnl::constant;  // NOLINT(misc-unused-using-decls)
        using cnl::scaled_integer;

        auto a = scaled_integer<long long>(3) + 4.F;
        static_assert(is_same_v<decltype(a), decltype(3.F + 4.F)>);

#if ! defined(__clang__)
        // TODO: either this must compile on Clang or the operator must be written
        auto b = scaled_integer(200U) - constant<100L>{};
        static_assert(is_same_v<decltype(b), decltype(scaled_integer<unsigned>(200) - scaled_integer<int>(100))>);
#endif

        auto c = scaled_integer<>(5) * 6UL;
        static_assert(is_same_v<decltype(c), decltype(scaled_integer<>(5) * scaled_integer<unsigned long>(6))>);

        constexpr auto d = scaled_integer<uint8_t, power<-7>>{1.25} * scaled_integer<uint8_t, power<-3>>{8};
        static_assert(is_same_v<decltype(d), const scaled_integer<int, power<-10>>>);
        static_assert(d == 10);

        constexpr auto e = scaled_integer<short, power<-5>>{1.5} / scaled_integer<short, power<-3>>{2.5};
        static_assert(is_same_v<decltype(e), const scaled_integer<int, power<-2>>>);
        static_assert(e == .5);

        constexpr auto f = scaled_integer<short, power<-5>>{1.5} % scaled_integer<short, power<-3>>{2.5};
        static_assert(is_same_v<decltype(f), const scaled_integer<int, power<-5>>>);
        static_assert(f == .25);

        // numerator = quotient * denominator + remainder
        constexpr auto f2 = e * scaled_integer<short, power<-3>>{2.5} + f;
        static_assert(is_same_v<decltype(f2), const scaled_integer<int, power<-5>>>);
        static_assert(f2 == scaled_integer<short, power<-5>>{1.5});

        constexpr auto g = scaled_integer<int8_t, power<-2>>{12.5} - scaled_integer<short, power<>>{8};
        static_assert(is_same_v<decltype(g), const scaled_integer<int, power<-2>>>);
        static_assert(g == 4.5);

        constexpr auto h = scaled_integer<int8_t, power<-2>>{12.5} <= scaled_integer<short, power<>>{8};
        static_assert(is_same_v<decltype(h), const bool>);
        static_assert(!h);
    }

    namespace division {
        using cnl::scaled_integer;
        using cnl::fraction;  // NOLINT(misc-unused-using-decls)
        using std::int64_t;

        constexpr auto i = scaled_integer{fraction{1, 3}};
        static_assert(i == 0.333333333022892475128173828125L);
        static_assert(is_same_v<decltype(i), const scaled_integer<int64_t, power<-31>>>);

        constexpr auto j = scaled_integer<int, power<-16>>{fraction{1, 3}};
        static_assert(j == 0.3333282470703125);
        static_assert(is_same_v<decltype(j), const scaled_integer<int, power<-16>>>);
    }
}

namespace example {
    using cnl::scaled_integer;

    template<class Fp>
    CNL_NODISCARD constexpr auto magnitude(Fp x, Fp y, Fp z)
    -> decltype(sqrt(x*x+y*y+z*z))
    {
        return sqrt(x*x+y*y+z*z);
    }

    TEST(proposal, examples)  // NOLINT
    {
        constexpr auto m = magnitude(
                scaled_integer<uint16_t, power<-12>>(1),
                scaled_integer<uint16_t, power<-12>>(4),
                scaled_integer<uint16_t, power<-12>>(9));
        static_assert(
                identical(m, scaled_integer<int, power<-24>>{9.8994948863983154}),
                "Incorrect information in proposal section, Examples");
    }
}

namespace open_issues {
    namespace extended_comparison_range {
        using cnl::scaled_integer;

        constexpr auto lhs = scaled_integer<uint8_t, power<>>{0};
        static_assert(!lhs);

        constexpr auto rhs = scaled_integer<uint8_t, power<128>>{4.e38};
        static_assert(rhs);

        //static_assert(lhs < rhs);
    }
}
