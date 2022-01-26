
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of cnl::elastic_scaled_integer alias

#include "../../number_test.h"

#include <cnl/elastic_scaled_integer.h>
#include <cnl/fraction.h>

#include <gtest/gtest.h>

#include <limits>
#include <type_traits>

using std::is_same;

using cnl::elastic_scaled_integer;
using cnl::_impl::identical;
using namespace cnl::literals;
using cnl::make_elastic_scaled_integer;

////////////////////////////////////////////////////////////////////////////////
// fast tests of cnl::elastic_scaled_integer<> at its limits;
// if something broke it may show up here first

namespace {
    using cnl::digits_v;
    using cnl::elastic_integer;
    using cnl::scaled_integer;
    using cnl::set_digits_t;

    static_assert(cnl::elastic_integer<64, unsigned>(0) == 0U);

    static_assert(
            identical(
                    cnl::elastic_integer<32, int>{246},
                    cnl::_impl::scale<1>(cnl::elastic_integer<31, int>{123})));
    static_assert(
            identical(
                    cnl::elastic_integer<33, unsigned>{246},
                    cnl::_impl::scale<1>(cnl::elastic_integer<32, unsigned>{123})));
#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(
                    cnl::elastic_integer<64, int>{246},
                    cnl::_impl::scale<1>(cnl::elastic_integer<63, int>{123})));
    static_assert(
            identical(
                    cnl::elastic_integer<65, unsigned>{246},
                    cnl::_impl::scale<1>(cnl::elastic_integer<64, unsigned>{123})));
#endif

    static_assert(
            digits_v<set_digits_t<elastic_integer<15, uint8_t>, 22>> == 22,
            "cnl::elastic_integer test failed");

    static_assert(
            identical(
                    set_digits_t<elastic_integer<15, uint8_t>, 22>{10000},
                    elastic_integer<22, uint8_t>{10000}),
            "cnl::elastic_integer test failed");

    static_assert(
            identical(elastic_scaled_integer<2, cnl::power<-2>>{1.5} << 1, elastic_scaled_integer<2, cnl::power<-2>>{3}));

    static_assert(
            identical(elastic_scaled_integer<2, cnl::power<-2>>{1.5} << 1, elastic_scaled_integer<2, cnl::power<-2>>{3}));

    static_assert(
            identical(elastic_scaled_integer<2, cnl::power<-2>>{1.5} >> 1, elastic_scaled_integer<2, cnl::power<-2>>{0.75}));
}

namespace test_from_value {
    static_assert(
            identical(
                    cnl::scaled_integer<unsigned>{42},
                    cnl::_impl::from_value<elastic_scaled_integer<20, cnl::power<10>>>(42U)));
    static_assert(
            identical(
                    elastic_scaled_integer<20, cnl::power<0>>{cnl::elastic_integer<20>{42}},
                    cnl::_impl::from_value<elastic_scaled_integer<20, cnl::power<10>>>(cnl::elastic_integer<20>{
                            42})));
}

namespace test_ctor {
#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(
                    uint32_t{0x76543210},
                    uint32_t(elastic_scaled_integer<64, cnl::power<-32>, unsigned>{
                            elastic_scaled_integer<32, cnl::power<>, unsigned>{0x76543210LL}})));
    static_assert(
            identical(uint32_t{1}, uint32_t(elastic_scaled_integer<64, cnl::power<-32>, unsigned>{1})));

    static_assert(
            identical(
                    cnl::scaled_integer<cnl::elastic_integer<62, int>, cnl::power<-40>>{321},
                    cnl::scaled_integer<cnl::elastic_integer<62, int>, cnl::power<-40>>{
                            scaled_integer<cnl::elastic_integer<62, int>, cnl::power<-20>>{321}}),
            "cnl::scaled_integer ctor");
    static_assert(
            identical(
                    cnl::scaled_integer<cnl::elastic_integer<62, int>, cnl::power<-40>>{
                            2097151.99999904632568359375},
                    cnl::scaled_integer<cnl::elastic_integer<62, int>, cnl::power<-40>>{
                            scaled_integer<cnl::elastic_integer<62, int>, cnl::power<-20>>{
                                    2097151.99999904632568359375}}),
            "cnl::scaled_integer ctor");
#endif
}

namespace test_addition {
    static constexpr auto lhs = cnl::elastic_scaled_integer<31>{1};
    static constexpr auto rhs = cnl::elastic_scaled_integer<40, cnl::power<-31>>{1};
    static constexpr auto expected = cnl::elastic_scaled_integer<63, cnl::power<-31>>{2};
    static constexpr auto sum = lhs + rhs;
    static_assert(identical(expected, sum), "cnl::elastic_scaled_integer addition");
}

namespace test_division {
    using cnl::elastic_integer;
    using cnl::scaled_integer;

    static_assert(
            identical(
                    elastic_scaled_integer<62, cnl::power<-31>>{.5},
                    cnl::quotient(
                            cnl::elastic_scaled_integer<31>{1},
                            cnl::elastic_scaled_integer<31>{2})));
    static_assert(
            identical(
                    elastic_scaled_integer<62, cnl::power<-31>>{.5},
                    make_scaled_integer(cnl::make_fraction(
                            elastic_scaled_integer<31>{1}, elastic_scaled_integer<31>{2}))));
#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(
                    elastic_scaled_integer<124, cnl::power<-62>>{.5},
                    make_scaled_integer(cnl::make_fraction(
                            elastic_scaled_integer<62>{1}, elastic_scaled_integer<62>{2}))));
#endif
}

namespace test_set_signedness {
    static_assert(
            cnl::numbers::signedness_v<cnl::numbers::set_signedness_t<elastic_scaled_integer<1, cnl::power<0>, unsigned>, true>>);
}

namespace test_fraction_make {
    using namespace cnl::literals;

    constexpr auto third = cnl::make_fraction(1_cnl2, 3_cnl2);

    constexpr auto named = cnl::quotient(third.numerator, third.denominator);
    static_assert(identical(cnl::elastic_scaled_integer<3, cnl::power<-2>>{0.25}, named));

    constexpr auto deduced = cnl::make_scaled_integer(third);
    static_assert(identical(named, deduced));
}

namespace test_fraction_specific_byte {
    using namespace cnl::literals;

    constexpr auto third = cnl::make_fraction(1_cnl2, 3_cnl2);

    constexpr auto specific = elastic_scaled_integer<7, cnl::power<-6>>{third};
    static_assert(identical(cnl::elastic_scaled_integer<7, cnl::power<-6>>{0.328125}, specific));
}

namespace test_fraction_specific_long {
    using namespace cnl::literals;

    constexpr auto third = cnl::make_fraction(1_cnl2, 3_cnl2);

    constexpr auto specific = cnl::elastic_scaled_integer<63, cnl::power<-60>>{third};
#if defined(_MSC_VER) || defined(__arm__)
    // MSVC's long double is less precise than 63 digits
    static_assert(
            std::is_same<
                    cnl::elastic_scaled_integer<63, cnl::power<-60>>,
                    cnl::elastic_scaled_integer<63, cnl::power<-60>>>::value);
    static_assert(specific > .333333333333333);
    static_assert(specific < .333333333333334);
#else
    static_assert(identical(cnl::elastic_scaled_integer<63, cnl::power<-60>>{1.L / 3}, specific));
#endif
}

namespace test_sqrt {
    static_assert(identical(
            elastic_scaled_integer<16, cnl::power<-10>>(0),
            sqrt(elastic_scaled_integer<31, cnl::power<-20>>(0))));
    static_assert(identical(
            elastic_scaled_integer<16, cnl::power<-10>>(1.4140625),
            sqrt(elastic_scaled_integer<31, cnl::power<-20>>(2.0))));
    static_assert(identical(
            elastic_scaled_integer<16, cnl::power<-10>>(2.0),
            sqrt(elastic_scaled_integer<31, cnl::power<-20>>(4.0))));
}

namespace test_floor {
    static_assert(
            identical(
                    elastic_scaled_integer<5>{13},
                    cnl::floor(elastic_scaled_integer<10, cnl::power<-5>>{13.625})));
    static_assert(
            identical(
                    elastic_scaled_integer<5>{-14},
                    cnl::floor(elastic_scaled_integer<10, cnl::power<-5>>{-13.625})));
    static_assert(
            identical(
                    elastic_scaled_integer<5, cnl::power<5>>{192},
                    cnl::floor(elastic_scaled_integer<5, cnl::power<5>>{192})));
}

namespace test_abs {
    static_assert(
            identical(
                    elastic_scaled_integer<24, cnl::power<-20>>{0.5},
                    cnl::abs(elastic_scaled_integer<24, cnl::power<-20>>{0.5})));
    static_assert(
            identical(
                    elastic_scaled_integer<31, cnl::power<-30>>{0.5},
                    cnl::abs(elastic_scaled_integer<31, cnl::power<-30>>{0.5})));
#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(
                    elastic_scaled_integer<63, cnl::power<-30>>{0.5},
                    cnl::abs(elastic_scaled_integer<63, cnl::power<-30>>{0.5})));
    static_assert(
            identical(
                    elastic_scaled_integer<63, cnl::power<-31>>{0.5},
                    cnl::abs(elastic_scaled_integer<63, cnl::power<-31>>{0.5})));
#endif
}

////////////////////////////////////////////////////////////////////////////////
// useful constants

template<class T, bool IsSigned>
struct test_traits {
    static_assert(
            std::numeric_limits<T>::is_signed == IsSigned,
            "std::numeric_limits<T>::is_signed fails for give type, T");
    static_assert(
            cnl::numbers::signedness_v<T> == IsSigned);
    static_assert(
            cnl::numbers::signedness_v<cnl::numbers::set_signedness_t<T, true>>);
    static_assert(
            !cnl::numbers::signedness_v<cnl::numbers::set_signedness_t<T, false>>);
};

template struct test_traits<std::uint8_t, false>;

template struct test_traits<std::uint16_t, false>;

template struct test_traits<std::uint32_t, false>;

template struct test_traits<std::uint64_t, false>;

template struct test_traits<std::int8_t, true>;

template struct test_traits<std::int16_t, true>;

template struct test_traits<std::int32_t, true>;

template struct test_traits<std::int64_t, true>;

////////////////////////////////////////////////////////////////////////////////
// useful functions

template<typename T>
[[nodiscard]] constexpr auto bit_count(T const& n) -> int
{
    return n ? 1 + bit_count(n & (n - 1)) : 0;
}

static_assert(bit_count(0) == 0, "bit_count test failed");
static_assert(bit_count(1) == 1, "bit_count test failed");
static_assert(bit_count(2) == 1, "bit_count test failed");
static_assert(bit_count(3) == 2, "bit_count test failed");
static_assert(bit_count(5) == 2, "bit_count test failed");
static_assert(bit_count(8) == 1, "bit_count test failed");
static_assert(bit_count(13) == 3, "bit_count test failed");
static_assert(bit_count(21) == 3, "bit_count test failed");
static_assert(bit_count(34) == 2, "bit_count test failed");
static_assert(bit_count(55) == 5, "bit_count test failed");
static_assert(bit_count(89) == 4, "bit_count test failed");
static_assert(bit_count(144) == 2, "bit_count test failed");

// http://stackoverflow.com/a/5775825/671509
template<size_t Size>
struct print_num_as_error {
    explicit operator char()
    {
        return Size + 256;
    }
};  // always overflow

namespace test_literal_decimal_cnl2 {
    static_assert(identical(00.125_cnl2, elastic_scaled_integer<1, cnl::power<-3>>{00.125}));
    static_assert(identical(-.125_cnl2, elastic_scaled_integer<1, cnl::power<-3>>{-.125}));

    static_assert(identical(.25_cnl2, elastic_scaled_integer<1, cnl::power<-2>>{.25}));
    static_assert(identical(-0.25_cnl2, elastic_scaled_integer<1, cnl::power<-2>>{-0.25}));

    static_assert(identical(00.5_cnl2, elastic_scaled_integer<1, cnl::power<-1>>{00.5}));
    static_assert(identical(-.5_cnl2, elastic_scaled_integer<1, cnl::power<-1>>{-.5}));

    static_assert(identical(.625_cnl2, elastic_scaled_integer<3, cnl::power<-3>>{.625}));
    static_assert(identical(-0.625_cnl2, elastic_scaled_integer<3, cnl::power<-3>>{-0.625}));

    static_assert(identical(0.65625_cnl2, elastic_scaled_integer<5, cnl::power<-5>>{0.65625}));
    static_assert(identical(-00.65625_cnl2, elastic_scaled_integer<5, cnl::power<-5>>{-00.65625}));

    static_assert(identical(0.75_cnl2, elastic_scaled_integer<2, cnl::power<-2>>{0.75}));
    static_assert(identical(-000.75_cnl2, elastic_scaled_integer<2, cnl::power<-2>>{-000.75}));

    static_assert(identical(0_cnl2, elastic_scaled_integer<0, cnl::power<>>{0}));

    static_assert(identical(1_cnl2, elastic_scaled_integer<1, cnl::power<>>{1}));
    static_assert(identical(-1_cnl2, elastic_scaled_integer<1, cnl::power<>>{-1}));

    static_assert(identical(2_cnl2, elastic_scaled_integer<1, cnl::power<1>>{2}));
    static_assert(identical(-2_cnl2, elastic_scaled_integer<1, cnl::power<1>>{-2}));

    static_assert(identical(03_cnl2, elastic_scaled_integer<2, cnl::power<>>{03}));
    static_assert(identical(-3_cnl2, elastic_scaled_integer<2, cnl::power<>>{-3}));

    static_assert(identical(4_cnl2, elastic_scaled_integer<1, cnl::power<2>>{4}));
    static_assert(identical(-0b100_cnl2, elastic_scaled_integer<1, cnl::power<2>>{-0b100}));

    static_assert(identical(6_cnl2, elastic_scaled_integer<2, cnl::power<1>>{6}));
    static_assert(identical(-06_cnl2, elastic_scaled_integer<2, cnl::power<1>>{-06}));

    static_assert(identical(42_cnl2, elastic_scaled_integer<5, cnl::power<1>>{42}));
    static_assert(identical(-42_cnl2, elastic_scaled_integer<5, cnl::power<1>>{-42}));

    static_assert(identical(0xAA_cnl2, elastic_scaled_integer<7, cnl::power<1>>{0xaa}));
    static_assert(identical(-0xaa_cnl2, elastic_scaled_integer<7, cnl::power<1>>{-0xaa}));

    static_assert(identical(897341888_cnl2, elastic_scaled_integer<24, cnl::power<6>>{897341888}));
    static_assert(identical(-897341888_cnl2, elastic_scaled_integer<24, cnl::power<6>>{-897341888}));
}

namespace test_literal_decimal_cnl {
    static_assert(identical(.001_cnl, elastic_scaled_integer<1, cnl::power<-3, 10>>{0.001}));
    static_assert(identical(-0.001_cnl, elastic_scaled_integer<1, cnl::power<-3, 10>>{-0.001}));

    static_assert(identical(00.1_cnl, elastic_scaled_integer<1, cnl::power<-1, 10>>{00.1}));
    static_assert(identical(-.1_cnl, elastic_scaled_integer<1, cnl::power<-1, 10>>{-.1}));

    static_assert(identical(00.125_cnl, elastic_scaled_integer<7, cnl::power<-3, 10>>{00.125}));
    static_assert(identical(-.125_cnl, elastic_scaled_integer<7, cnl::power<-3, 10>>{-.125}));

    static_assert(identical(.25_cnl, elastic_scaled_integer<5, cnl::power<-2, 10>>{.25}));
    static_assert(identical(-0.25_cnl, elastic_scaled_integer<5, cnl::power<-2, 10>>{-0.25}));

    static_assert(identical(00.5_cnl, elastic_scaled_integer<3, cnl::power<-1, 10>>{00.5}));
    static_assert(identical(-.5_cnl, elastic_scaled_integer<3, cnl::power<-1, 10>>{-.5}));

    static_assert(identical(.625_cnl, elastic_scaled_integer<10, cnl::power<-3, 10>>{.625}));
    static_assert(identical(-0.625_cnl, elastic_scaled_integer<10, cnl::power<-3, 10>>{-0.625}));

    static_assert(identical(0.65625_cnl, elastic_scaled_integer<17, cnl::power<-5, 10>>{0.65625}));
    static_assert(identical(-00.65625_cnl, elastic_scaled_integer<17, cnl::power<-5, 10>>{-00.65625}));

    static_assert(identical(0.75_cnl, elastic_scaled_integer<7, cnl::power<-2, 10>>{0.75}));
    static_assert(identical(-000.75_cnl, elastic_scaled_integer<7, cnl::power<-2, 10>>{-000.75}));

    static_assert(identical(0_cnl, elastic_scaled_integer<0, cnl::power<0, 10>>{0}));

    static_assert(identical(1_cnl, elastic_scaled_integer<1, cnl::power<0, 10>>{1}));
    static_assert(identical(-1_cnl, elastic_scaled_integer<1, cnl::power<0, 10>>{-1}));

    static_assert(identical(2_cnl, elastic_scaled_integer<2, cnl::power<0, 10>>{2}));
    static_assert(identical(-02_cnl, elastic_scaled_integer<2, cnl::power<0, 8>>{-2}));

    static_assert(identical(07_cnl, elastic_scaled_integer<3, cnl::power<0, 8>>{07}));
    static_assert(identical(-07_cnl, elastic_scaled_integer<3, cnl::power<0, 8>>{-07}));

    static_assert(identical(010_cnl, elastic_scaled_integer<1, cnl::power<1, 8>>{010}));
    static_assert(identical(-010_cnl, elastic_scaled_integer<1, cnl::power<1, 8>>{-010}));

    static_assert(identical(9_cnl, elastic_scaled_integer<4, cnl::power<0, 10>>{9}));
    static_assert(identical(-9_cnl, elastic_scaled_integer<4, cnl::power<0, 10>>{-9}));

    static_assert(identical(10_cnl, elastic_scaled_integer<1, cnl::power<1, 10>>{10}));
    static_assert(identical(-10_cnl, elastic_scaled_integer<1, cnl::power<1, 10>>{-10}));

    static_assert(identical(0Xf_cnl, elastic_scaled_integer<4, cnl::power<0, 16>>{0Xf}));
    static_assert(identical(-0xF_cnl, elastic_scaled_integer<4, cnl::power<0, 16>>{-0xF}));

    static_assert(identical(0x10_cnl, elastic_scaled_integer<1, cnl::power<1, 16>>{0x10}));
    static_assert(identical(-0X10_cnl, elastic_scaled_integer<1, cnl::power<1, 16>>{-0X10}));

    static_assert(identical(0B101010_cnl, elastic_scaled_integer<5, cnl::power<1>>{0B101010}));
    static_assert(identical(-0b101010_cnl, elastic_scaled_integer<5, cnl::power<1>>{-0b101010}));

    static_assert(identical(0xAA_cnl, elastic_scaled_integer<8, cnl::power<0, 16>>{0xaa}));
    static_assert(identical(-0xaa_cnl, elastic_scaled_integer<8, cnl::power<0, 16>>{-0xaa}));

    static_assert(identical(897341888_cnl, elastic_scaled_integer<30, cnl::power<0, 10>>{897341888}));
    static_assert(identical(-897341888_cnl, elastic_scaled_integer<30, cnl::power<0, 10>>{-897341888}));

    static_assert(0.1 + 0.2 != 0.3);
    static_assert(0.1_cnl + 0.2_cnl == 0.3_cnl);
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic_scaled_integer handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test : number_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using rep = cnl::_impl::rep_of_t<elastic_type>;
    using numeric_limits = std::numeric_limits<elastic_type>;

    using signed_type = cnl::numbers::set_signedness_t<elastic_type, true>;
    using unsigned_type = cnl::numbers::set_signedness_t<elastic_type, false>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};
    static constexpr elastic_type negative_zero{-zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    static constexpr int integer_digits = cnl::_impl::integer_digits_v<elastic_type>;
    static constexpr int fractional_digits = cnl::_impl::fractional_digits_v<elastic_type>;
    static constexpr int digits = integer_digits + fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(
            std::numeric_limits<elastic_type>::is_signed == std::numeric_limits<rep>::is_signed,
            "signedness of elastic_scaled_integer type differs from underlying fixed-point type");
    static_assert(
            std::numeric_limits<signed_type>::is_signed,
            "signed version of elastic_scaled_integer type is not signed");
    static_assert(
            !std::numeric_limits<unsigned_type>::is_signed,
            "unsigned version of elastic_scaled_integer type is signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test elastic_integer type

    static_assert(
            cnl::digits_v<rep> >= digits,
            "not enough digits in rep type to represent elastic_scaled_integer values");

    ////////////////////////////////////////////////////////////////////////////////
    // test std::numeric_limits<elastic_scaled_integer>

#if !defined(_MSC_VER)
    static_assert(
            min == cnl::_impl::from_rep<elastic_type>(rep{1}), "std::numeric_limits test failed");
#endif
    static_assert(!is_less_than(max, min), "std::numeric_limits test failed");
    static_assert(is_less_than(zero, min), "std::numeric_limits test failed");
    static_assert(!is_less_than(zero, lowest), "std::numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "std::numeric_limits test failed");
    static_assert(
            std::numeric_limits<elastic_type>::is_signed == numeric_limits::is_signed,
            "std::numeric_limits test failed");
    static_assert(
            !numeric_limits::is_integer || (elastic_type{.5} != .5),
            "std::numeric_limits test failed");

    static constexpr rep max_integer{to_rep(max)};
#if !defined(_MSC_VER)
    static_assert(
            bit_count<cnl::_impl::rep_of_t<rep>>(to_rep(max_integer)) == digits,
            "std::numeric_limits test failed");
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between zero and negative zero
    static_assert(is_equal_to(zero, negative_zero), "comparison test error");

    // zero vs min
    static_assert(is_less_than<elastic_type>(zero, min), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test negate operators

    using negate_result = decltype(-zero);
    static_assert(
            cnl::_impl::integer_digits_v<negate_result> == cnl::_impl::integer_digits_v<elastic_type>,
            "negative of positive value has wrong number of integer digits");
    static_assert(
            cnl::_impl::fractional_digits_v<negate_result> == cnl::_impl::fractional_digits_v<elastic_type>,
            "negative of positive value has wrong number of fraction digits");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(
            std::numeric_limits<decltype(zero + zero)>::is_signed
                    == std::numeric_limits<elastic_type>::is_signed,
            "signedness is lost during add");
    static_assert(
            std::numeric_limits<decltype(signed_type{zero} + unsigned_type{zero})>::is_signed,
            "signedness is lost during add");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_less_than(max - min, max), "operator- test failed");

    static_assert(
            // NOLINTNEXTLINE(misc-redundant-expression)
            std::numeric_limits<decltype(zero - zero)>::is_signed,
            "signedness is lost during subtract");
    static_assert(
            std::numeric_limits<decltype(signed_type{zero} - unsigned_type{zero})>::is_signed,
            "signedness is lost during subtract");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator*

    static_assert(
            is_equal_to(min * make_elastic_scaled_integer(0_c), zero), "operator* test failed");
    static_assert(
            is_equal_to(min * make_elastic_scaled_integer(1_c), min), "operator* test failed");
    static_assert(
            is_equal_to(min * make_elastic_scaled_integer(2_c), min + min),
            "operator* test failed");
    static_assert(
            is_equal_to(min * make_elastic_scaled_integer(3_c), min + min + min),
            "operator* test failed");

    static_assert(
            std::numeric_limits<decltype(zero * zero)>::is_signed
                    == std::numeric_limits<decltype(zero)>::is_signed,
            "signedness is lost during multiply");
    static_assert(
            std::numeric_limits<decltype(zero * zero)>::is_signed
                    == std::numeric_limits<elastic_type>::is_signed,
            "signedness is lost during multiply");
    static_assert(
            std::numeric_limits<decltype(signed_type{zero} * unsigned_type{zero})>::is_signed,
            "signedness is lost during multiply");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator/

    static_assert(
            !is_less_than(min, min / make_elastic_scaled_integer(2_c)), "operator/ test failed");
    static_assert(
            is_equal_to(min / make_elastic_scaled_integer(1_c), min), "operator/ test failed");
    static_assert(
            // NOLINTNEXTLINE(misc-redundant-expression)
            is_equal_to((min + min) / make_elastic_scaled_integer(2_c), min),
            "operator/ test failed");
    static_assert(
            is_equal_to((min + min + min) / make_elastic_scaled_integer(3_c), min),
            "operator/ test failed");
    static_assert(
            // NOLINTNEXTLINE(misc-redundant-expression)
            std::numeric_limits<decltype(zero / zero)>::is_signed
                    == std::numeric_limits<elastic_type>::is_signed,
            "signedness is lost during multiply");
    static_assert(
            std::numeric_limits<decltype(signed_type{zero} / unsigned_type{zero})>::is_signed,
            "signedness is lost during multiply");
#if !defined(_MSC_VER)
    static_assert(
            identical(
                    cnl::elastic_scaled_integer<12, cnl::power<-7>>{3. / 4},
                    cnl::make_scaled_integer(cnl::make_fraction(
                            cnl::elastic_scaled_integer<10, cnl::power<-5>>{1.5}, cnl::elastic_integer<2>{2}))),
            "operator/ test failed");
    static_assert(
            identical(
                    cnl::elastic_scaled_integer<12, cnl::power<-5>>{4. / 3},
                    cnl::make_scaled_integer(cnl::make_fraction(
                            cnl::elastic_integer<2>{2}, cnl::elastic_scaled_integer<10, cnl::power<-5>>{1.5}))),
            "operator/ test failed");
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::from_rep

    static_assert(
            !cnl::_impl::from_rep<
                    cnl::scaled_integer<cnl::elastic_integer<31, unsigned int>, cnl::power<-33>>>(
                    0),
            "cnl::from_rep<scaled_integer<elastic_integer>>(int)");
};

TEST(elastic_scaled_integer, over_int)  // NOLINT
{
    auto q = cnl::elastic_scaled_integer<10, cnl::power<-5>>{1.5} / elastic_integer<2>{2};
    auto e = cnl::elastic_scaled_integer<12, cnl::power<-7>>{3. / 4};
    EXPECT_EQ(e, q);
}

TEST(elastic_scaled_integer, int_over)  // NOLINT
{
    auto f = cnl::make_fraction(elastic_integer<2>{2}, elastic_scaled_integer<10, cnl::power<-5>>{1.5});
    auto q = cnl::make_scaled_integer(f);
    auto e = elastic_scaled_integer<12, cnl::power<-5>>{4. / 3};
    EXPECT_EQ(e, q);
}

TEST(elastic_scaled_integer, issue_88)  // NOLINT
{
    using fix_t = cnl::elastic_scaled_integer<30, cnl::power<-16>>;
    fix_t a = 2.0F;
    fix_t b = 1.0F;
    fix_t c = 1.0F;
    EXPECT_EQ(static_cast<float>(a), 2.0F);
    EXPECT_EQ(static_cast<float>(b), 1.0F);
    EXPECT_EQ(static_cast<float>(c), 1.0F);
    fix_t d = c + a * b;
    EXPECT_EQ(static_cast<float>(d), 3.0F);
}

TEST(elastic_scaled_integer, to_string_thousand)  // NOLINT
{
    auto const n{1000_cnl};
    static_assert(5 == cnl::_impl::to_chars_capacity<std::remove_cvref_t<decltype(n)>>{}());

    auto const expected{"1000"};
    auto const actual{cnl::to_string(1000_cnl)};
    ASSERT_EQ(expected, actual);
}

TEST(elastic_scaled_integer, to_string_thousandth)  // NOLINT
{
    auto const n{.001_cnl};
    static_assert(5 == cnl::_impl::to_chars_capacity<std::remove_cvref_t<decltype(n)>>{}());

    auto const expected{".001"};
    auto const actual{cnl::to_string(n)};
    ASSERT_EQ(expected, actual);
}

TEST(elastic_scaled_integer, to_string_123_456)  // NOLINT
{
    auto const n{123.456_cnl};
    static_assert(10 == cnl::_impl::to_chars_capacity<std::remove_cvref_t<decltype(n)>>{}());

    auto const expected{"123.456"};
    auto const actual{cnl::to_string(n)};
    ASSERT_EQ(expected, actual);
}

TEST(elastic_scaled_integer, to_string_quite_wide)  // NOLINT
{
    auto const n{-12345.67890_cnl};
    static_assert(13 == cnl::_impl::to_chars_capacity<std::remove_cvref_t<decltype(n)>>{}());

    auto const expected{"-12345.6789"};
    auto const actual{cnl::to_string(n)};
    ASSERT_EQ(expected, actual);
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic_scaled_integer handles negative values;
// should pass for all signed specializations

template<class Elastic>
struct signed_elastic_test
    :
    // test type traits given this type is signed
    test_traits<Elastic, true>
    ,

    // perform positive value tests against signed elastic_scaled_integer specialization
    positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using rep = cnl::_impl::rep_of_t<elastic_type>;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};
    static constexpr elastic_type negative_min{-min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    // not much point testing negative value properties of unsigned type, eh?
    static_assert(
            std::numeric_limits<elastic_type>::is_signed,
            "subject of test class is not reported as signed");
    static_assert(
            is_same<cnl::numbers::set_signedness_t<elastic_type, true>, elastic_type>::value);

    ////////////////////////////////////////////////////////////////////////////////
    // test std::numeric_limits<elastic_scaled_integer>

    static_assert(is_less_than(negative_min, min), "std::numeric_limits test failed");
    static_assert(is_equal_to(-max, lowest), "comparison test error");
    // static_assert(is_equal_to(elastic_type{min+max+lowest}, elastic_type{1}), "comparison test
    // error");
    static_assert(numeric_limits::is_signed, "std::numeric_limits test failed");
    static_assert(
            !numeric_limits::is_integer || elastic_type{-.5} != -.5,
            "std::numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between negative_min
    static_assert(is_equal_to(negative_min, negative_min), "comparison test error");

    // min vs negative_min
    static_assert(is_less_than<elastic_type>(negative_min, min), "comparison test error");

    // negative_min vs zero
    static_assert(is_less_than<elastic_type>(negative_min, zero), "comparison test error");

    // negative_min vs lowest
    static_assert(
            numeric_limits::is_signed ? !is_less_than(negative_min, lowest)
                                      : is_less_than(negative_min, lowest),
            "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(is_equal_to(min + max + lowest, min), "operator+ test failed");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic_scaled_integer handles positive values;
// should pass for all unsigned specializations

template<class Elastic>
struct unsigned_elastic_test
    :
    // test type traits given this type is not signed
    test_traits<Elastic, false>
    ,

    // perform positive value tests against unsigned elastic_scaled_integer specialization
    positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    ////////////////////////////////////////////////////////////////////////////////
    // test std::numeric_limits<elastic_scaled_integer>

    static_assert(is_equal_to(lowest, zero), "std::numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "std::numeric_limits test failed");
};

////////////////////////////////////////////////////////////////////////////////
// given values for IntegerDigits and FractionalDigits parameters,
// triggers elastic_scaled_integer<> tests with signed and unsigned specializations

template<int IntegerDigits, int FractionalDigits>
struct elastic_test
    :
    // perform unsigned-specific value tests against unsigned elastic_scaled_integer specialization
    unsigned_elastic_test<
            elastic_scaled_integer<IntegerDigits + FractionalDigits, cnl::power<-FractionalDigits>, unsigned>>
    ,

    // perform negative value tests against signed elastic_scaled_integer specialization
    signed_elastic_test<
            elastic_scaled_integer<IntegerDigits + FractionalDigits, cnl::power<-FractionalDigits>, signed>> {
};

////////////////////////////////////////////////////////////////////////////////
// given a value for IntegerDigits parameter,
// triggers elastic_scaled_integer<> tests against a range of values for FractionalDigits parameter

template<int IntegerDigits>
struct elastic_test_with_integer_digits
    : elastic_test<IntegerDigits, -IntegerDigits + 1>
    , elastic_test<IntegerDigits, -IntegerDigits + 2>
    , elastic_test<IntegerDigits, -IntegerDigits + 31> {
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic_scaled_integer tests against a range of values for IntegerDigits parameter

template struct elastic_test_with_integer_digits<1>;

template struct elastic_test_with_integer_digits<-2>;

template struct elastic_test_with_integer_digits<5>;

template struct elastic_test_with_integer_digits<-16>;

template struct elastic_test_with_integer_digits<63>;
