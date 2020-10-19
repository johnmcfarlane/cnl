
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of C++17-only examples listed in paper, P0675

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/num_traits.h>
#include <cnl/rounding_integer.h>
#include <cnl/scaled_integer.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::_impl::identical;

namespace acme {
    template<class Rep> struct smart_integer;
    template<class Rep> CNL_NODISCARD constexpr Rep to_rep(smart_integer<Rep> const& number);

    // example type, smart_integer, shares characteristics with cnl::elastic_integer
    template<class Rep>
    struct smart_integer {
        explicit constexpr smart_integer(Rep const& rhs) : _value(rhs) {}
        friend constexpr Rep to_rep<>(smart_integer<Rep> const& number);
    private:
        Rep _value;
    };

    template<typename Rep>
    CNL_NODISCARD constexpr Rep to_rep(smart_integer<Rep> const& number)
    {
        return number._value;
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
        // NOLINTNEXTLINE(bugprone-suspicious-semicolon,hicpp-braces-around-statements,misc-suspicious-semicolon,readability-braces-around-statements,google-readability-braces-around-statements)
        if constexpr (std::numeric_limits<LhsRep>::is_signed == std::numeric_limits<RhsRep>::is_signed) {
            auto product_rep = lhs_rep * rhs_rep;
            return smart_integer{product_rep};
        }

        auto lhs_rep_signed = std::make_signed_t<LhsRep>(lhs_rep);
        auto rhs_rep_signed = std::make_signed_t<RhsRep>(rhs_rep);
        auto product_rep = lhs_rep_signed * rhs_rep_signed;
        return smart_integer{product_rep};
    }
}

namespace cnl {
    using acme::smart_integer;
    template<class Rep>
    struct numeric_limits<smart_integer<Rep>> : numeric_limits<Rep> {};
}

namespace {
    // example type, scaled_integer, is taken directy from cnl::scaled_integer
    using cnl::power;
    using cnl::scaled_integer;
    using acme::smart_integer;

    // example type, rounded_integer, is based off of cnl::rounding_integer
    template<class Rep>
    using rounded_integer = cnl::rounding_integer<Rep>;

    TEST(P0675, compose_from_fundamental) {  // NOLINT
        // use an unsigned 16-bit integer to approximate a real number with 2 integer and 14 fraction digits
        auto pi = scaled_integer<uint16_t, power<-14>>{3.141};
        ASSERT_TRUE(pi > 3.1 && pi < 3.2);

        // use int to store value gained using accurate rounding mode
        auto num_children = rounded_integer<int>{2.6};
        ASSERT_TRUE(num_children == 3);
    }

    TEST(P0675, compose_from_components) {  // NOLINT
        auto num = scaled_integer<rounded_integer<uint8_t>, power<-4>>{15.9375};
        ASSERT_TRUE((cnl::from_rep<decltype(num), int>{}(1) == 1. / 16));
    }

    TEST(P0675, smart_multiply) {  // NOLINT
        // smart_integer chooses appropriate signedness for results of arithmetic operations
        auto a = smart_integer{7U};
        auto b = smart_integer{-3};
        auto c = a * b;  // smart_integer<int>{-21}
        ASSERT_TRUE(identical(smart_integer<int>{-21}, c));
    }

    TEST(P0675, smart_add) {  // NOLINT
        auto m = smart_integer{5U};
        auto s = smart_integer{10U};
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
        static_assert(is_composite_v<scaled_integer<short>>);

        // to_rep
        using cnl::_impl::to_rep;
        static_assert(identical(1L, to_rep(1L)));
        static_assert(identical(1L, to_rep(smart_integer{1L})));

        // from_rep
        using cnl::from_rep;
        static_assert(identical(7, from_rep<int, int>{}(7)));
        static_assert(identical(
                scaled_integer<int, power<-1>>{49.5},
                from_rep<scaled_integer<int, power<-1>>, int>{}(99)));

        // from_value
        using cnl::from_value_t;
        static_assert(identical(
                scaled_integer<unsigned long, power<>>{99},
                from_value_t<scaled_integer<int16_t, power<-1>>, unsigned long>{99UL}));
    }
}
