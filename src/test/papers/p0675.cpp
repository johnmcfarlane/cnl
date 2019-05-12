
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of C++17-only examples listed in paper, P0675

#if defined(__cpp_deduction_guides)

#include <cnl/rounding_integer.h>
#include <cnl/num_traits.h>
#include <cnl/fixed_point.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::_impl::identical;

namespace acme {
    template<class Rep> struct smart_integer;
    template<class Rep> CNL_NODISCARD constexpr Rep to_rep(smart_integer<Rep> const& number);

    // example type, smart_integer, shares characteristics with cnl::elastic_integer
    template<class Rep>
    struct smart_integer {
        explicit constexpr smart_integer(Rep const& rhs) : value(rhs) {}
        friend constexpr Rep to_rep<>(smart_integer<Rep> const& number);
    private:
        Rep value;
    };

    template<typename Rep>
    CNL_NODISCARD constexpr Rep to_rep(smart_integer<Rep> const& number)
    {
        return number.value;
    }

    template<class LhsRep, class RhsRep>
    CNL_NODISCARD constexpr auto operator==(smart_integer<LhsRep> const& lhs, smart_integer<RhsRep> const& rhs)
    {
        return to_rep(lhs)==to_rep(rhs);
    }

    template<class LhsRep, class RhsRep>
    CNL_NODISCARD constexpr auto operator-(smart_integer<LhsRep> const& lhs, smart_integer<RhsRep> const& rhs)
    {
        auto lhs_rep_signed = std::make_signed_t<LhsRep>(to_rep(lhs));
        auto rhs_rep_signed = std::make_signed_t<RhsRep>(to_rep(rhs));
        auto difference = lhs_rep_signed - rhs_rep_signed;
        return smart_integer{difference};
    }

    template<class LhsRep, class RhsRep>
    CNL_NODISCARD constexpr auto operator*(smart_integer<LhsRep> const& lhs, smart_integer<RhsRep> const& rhs)
    {
        auto const lhs_rep = to_rep(lhs);
        auto const rhs_rep = to_rep(rhs);
        if constexpr (std::numeric_limits<LhsRep>::is_signed == std::numeric_limits<RhsRep>::is_signed) {
            auto product_rep = lhs_rep * rhs_rep;
            return smart_integer{product_rep};
        }
        else {
            auto lhs_rep_signed = std::make_signed_t<LhsRep>(lhs_rep);
            auto rhs_rep_signed = std::make_signed_t<RhsRep>(rhs_rep);
            auto product_rep = lhs_rep_signed * rhs_rep_signed;
            return smart_integer{product_rep};
        }
    }
}

namespace cnl {
    using acme::smart_integer;
    template<class Rep>
    struct numeric_limits<smart_integer<Rep>> : numeric_limits<Rep> {};
}

namespace {
    // example type, fixed_point, is taken directy from cnl::fixed_point
    using cnl::fixed_point;
    using acme::smart_integer;

    // example type, rounded_integer, is based off of cnl::rounding_integer
    template<class Rep>
    using rounded_integer = cnl::rounding_integer<Rep>;

    TEST(P0675, compose_from_fundamental) {
        // use an unsigned 16-bit integer to approximate a real number with 2 integer and 14 fraction digits
        auto pi = fixed_point<uint16_t, -14>{3.141};
        ASSERT_TRUE(pi > 3.1 && pi < 3.2);

        // use int to store value gained using accurate rounding mode
        auto num_children = rounded_integer<int>{2.6};
        ASSERT_TRUE(num_children == 3);
    }

    TEST(P0675, compose_from_components) {
        auto num = fixed_point<rounded_integer<uint8_t>, -4>{15.9375};
        ASSERT_TRUE((cnl::from_rep<decltype(num), int>{}(1) == 1. / 16));
    }

    TEST(P0675, smart_multiply) {
        // smart_integer chooses appropriate signedness for results of arithmetic operations
        auto a = smart_integer{7u};
        auto b = smart_integer{-3};
        auto c = a * b;  // smart_integer<int>{-21}
        ASSERT_TRUE(identical(smart_integer<int>{-21}, c));
    }

    TEST(P0675, smart_add) {
        auto m = smart_integer{5u};
        auto s = smart_integer{10u};
        auto d = m - s;  // smart_integer<int>{-5}
        ASSERT_TRUE(identical(smart_integer<int>{-5}, d));
    }

    namespace desirata {
        // num_digits
        template<class T> constexpr auto num_digits_v = cnl::digits_v<T>;
        static_assert(num_digits_v<int64_t> == 63);

        // set_num_digits
        template<class T, int B> using set_num_digits_t = cnl::set_digits_t<T, B>;
        static_assert(std::is_same<set_num_digits_t<unsigned, 8>, std::uint8_t>::value);

        // is_composite
        using cnl::is_composite_v;
        static_assert(!is_composite_v<short>);
        static_assert(is_composite_v<fixed_point<short>>);

        // to_rep
        using cnl::_impl::to_rep;
        static_assert(identical(1L, to_rep(1L)));
        static_assert(identical(1L, to_rep(smart_integer{1L})));

        // from_rep
        using cnl::from_rep;
        static_assert(identical(7, from_rep<int, int>{}(7)));
        static_assert(identical(fixed_point<int, -1>{49.5}, from_rep<fixed_point<int, -1>, int>{}(99)));

        // from_value
        using cnl::from_value_t;
        static_assert(identical(fixed_point<unsigned long, 0>{99}, from_value_t<fixed_point<int16_t, -1>, unsigned long>{99UL}));
    }
}

#endif  // defined(__cpp_deduction_guides)
