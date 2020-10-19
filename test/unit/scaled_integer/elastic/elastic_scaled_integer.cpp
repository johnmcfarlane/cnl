
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

using std::is_same;

using cnl::elastic_scaled_integer;
using namespace cnl::literals;
using cnl::make_elastic_scaled_integer;

////////////////////////////////////////////////////////////////////////////////
// fast tests of cnl::elastic_scaled_integer<> at its limits;
// if something broke it may show up here first

namespace {
    using cnl::digits;
    using cnl::elastic_integer;
    using cnl::is_signed;
    using cnl::scaled_integer;
    using cnl::set_digits_t;

    static_assert(cnl::elastic_integer<64, unsigned>(0)==0U, "");

    static_assert(identical(cnl::elastic_integer<32, int>{246}, cnl::_impl::scale<1>(cnl::elastic_integer<31, int>{123})), "");
    static_assert(identical(cnl::elastic_integer<33, unsigned>{246}, cnl::_impl::scale<1>(cnl::elastic_integer<32, unsigned>{123})), "");
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(cnl::elastic_integer<64, int>{246}, cnl::_impl::scale<1>(cnl::elastic_integer<63, int>{123})), "");
    static_assert(identical(cnl::elastic_integer<65, unsigned>{246}, cnl::_impl::scale<1>(cnl::elastic_integer<64, unsigned>{123})), "");
#endif

    static_assert(digits<set_digits_t<elastic_integer<15, uint8_t>, 22>>::value==22,
            "cnl::elastic_integer test failed");

    static_assert(identical(
            set_digits_t<elastic_integer<15, uint8_t>, 22>{10000},
            elastic_integer<22, uint8_t>{10000}), "cnl::elastic_integer test failed");

    static_assert(identical(
            elastic_scaled_integer<2, -2>{1.5} << 1,
            elastic_scaled_integer<2, -2>{3}), "cnl::elastic_scaled_integer test failed");

    static_assert(identical(
            elastic_scaled_integer<2, -2>{1.5} << 1,
            elastic_scaled_integer<2, -2>{3}), "cnl::elastic_scaled_integer test failed");

    static_assert(identical(
            elastic_scaled_integer<2, -2>{1.5} >> 1,
            elastic_scaled_integer<2, -2>{0.75}), "cnl::elastic_scaled_integer test failed");
}

namespace test_from_value {
    static_assert(
            identical(
                    cnl::scaled_integer<unsigned>{42},
                    cnl::_impl::from_value<elastic_scaled_integer<20, 10>>(42U)),
            "");
    static_assert(
            identical(
                    elastic_scaled_integer<20, 0>{cnl::elastic_integer<20>{42}},
                    cnl::_impl::from_value<elastic_scaled_integer<20, 10>>(cnl::elastic_integer<20>{42})),
            "");
}

namespace test_ctor {
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(uint32_t{0x76543210}, uint32_t(elastic_scaled_integer<64, -32, unsigned>{elastic_scaled_integer<32, 0, unsigned>{0x76543210LL}})), "cnl::elastic_scaled_integer ctor");
    static_assert(identical(uint32_t{1}, uint32_t(elastic_scaled_integer<64, -32, unsigned>{1})), "cnl::elastic_scaled_integer ctor");

    static_assert(
            identical(
                    cnl::scaled_integer<
                            cnl::elastic_integer<62, int>,
                            cnl::power<-40>>{
                        321},
                    cnl::scaled_integer<
                            cnl::elastic_integer<62, int>,
                            cnl::power<-40>>{
                        scaled_integer<
                                cnl::elastic_integer<62, int>,
                                cnl::power<-20>>{321}}),
            "cnl::scaled_integer ctor");
    static_assert(
            identical(
                    cnl::scaled_integer<
                            cnl::elastic_integer<62, int>,
                            cnl::power<-40>>{
                        2097151.99999904632568359375},
                    cnl::scaled_integer<
                            cnl::elastic_integer<62, int>,
                            cnl::power<-40>>{
                        scaled_integer<
                                cnl::elastic_integer<62, int>,
                                cnl::power<-20>>{2097151.99999904632568359375}}),
            "cnl::scaled_integer ctor");
#endif
}

namespace test_addition {
    static constexpr auto lhs = cnl::elastic_scaled_integer<31>{1};
    static constexpr auto rhs = cnl::elastic_scaled_integer<40, -31>{1};
    static constexpr auto expected = cnl::elastic_scaled_integer<63, -31>{2};
    static constexpr auto sum = lhs + rhs;
    static_assert(identical(expected, sum), "cnl::elastic_scaled_integer addition");
}

namespace test_division {
    using cnl::elastic_integer;
    using cnl::scaled_integer;

    static_assert(identical(
            elastic_scaled_integer<62, - 31>{.5},
            cnl::quotient(cnl::elastic_scaled_integer<31, 0>{1}, cnl::elastic_scaled_integer<31, 0>{2})),
            "cnl::quotient(cnl::elastic_scaled_integer, cnl::elastic_scaled_integer)");
    static_assert(identical(
            elastic_scaled_integer<62, - 31>{.5},
            make_scaled_integer(cnl::make_fraction(elastic_scaled_integer<31, 0>{1}, elastic_scaled_integer<31, 0>{2}))),
            "cnl::elastic_scaled_integer division");
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(
            elastic_scaled_integer<124, -62>{.5},
            make_scaled_integer(cnl::make_fraction(elastic_scaled_integer<62, 0>{1}, elastic_scaled_integer<62, 0>{2}))),
            "cnl::elastic_scaled_integer division");
#endif
}

namespace test_set_signedness {
    static_assert(
            is_signed<cnl::add_signedness_t<elastic_scaled_integer<1, 0, unsigned>>>::value,
            "");
}

namespace test_fraction_deduced {
    using namespace cnl::literals;

    constexpr auto third = cnl::make_fraction(1_elastic, 3_elastic);

    constexpr auto named = cnl::quotient(third.numerator, third.denominator);
    static_assert(identical(cnl::elastic_scaled_integer<3, -2>{0.25}, named), "");

    constexpr auto deduced = scaled_integer{third};
    static_assert(identical(named, deduced));
}

namespace test_fraction_specific_byte {
    using namespace cnl::literals;

    constexpr auto third = cnl::make_fraction(1_elastic, 3_elastic);

    constexpr auto specific = elastic_scaled_integer<7, -6>{third};
    static_assert(identical(cnl::elastic_scaled_integer<7, -6>{0.328125}, specific), "");
}

namespace test_fraction_specific_long {
    using namespace cnl::literals;

    constexpr auto third = cnl::make_fraction(1_elastic, 3_elastic);

    constexpr auto specific = cnl::elastic_scaled_integer<63, -60>{third};
#if defined(_MSC_VER) || defined(__arm__)
    // MSVC's long double is less precise than 63 digits
    static_assert(std::is_same<cnl::elastic_scaled_integer<63, -60>, cnl::elastic_scaled_integer<63, -60>>::value, "");
    static_assert(specific > .333333333333333, "");
    static_assert(specific < .333333333333334, "");
#else
    static_assert(identical(cnl::elastic_scaled_integer<63, -60>{1.L/3}, specific), "");
#endif
}

namespace test_sqrt {
    static_assert(static_cast<float>(sqrt(elastic_scaled_integer<31, -20>(0))) == 0.0F, "sqrt<elastic_scaled_integer>");
    static_assert(static_cast<float>(sqrt(elastic_scaled_integer<31, -20>(2.0))) > 1.414213F, "sqrt<elastic_scaled_integer>");
    static_assert(static_cast<float>(sqrt(elastic_scaled_integer<31, -20>(2.0))) < 1.414214F, "sqrt<elastic_scaled_integer>");
    static_assert(static_cast<float>(sqrt(elastic_scaled_integer<31, -20>(4.0))) == 2.0F, "sqrt<elastic_scaled_integer>");
}

namespace test_floor {
    static_assert(identical(
            elastic_scaled_integer<5, 0>{13},
            cnl::floor(elastic_scaled_integer<10, -5>{13.625})), "floor(elastic_scaled_integer)");
    static_assert(identical(
            elastic_scaled_integer<5, 0>{-14},
            cnl::floor(elastic_scaled_integer<10, -5>{-13.625})), "floor(elastic_scaled_integer)");
    static_assert(identical(
            elastic_scaled_integer<5, 5>{192},
            cnl::floor(elastic_scaled_integer<5, 5>{192})), "floor(elastic_scaled_integer)");
}

namespace test_abs {
    static_assert(identical(
            elastic_scaled_integer<24, -20>{0.5},
            cnl::abs(elastic_scaled_integer<24, -20>{0.5})), "abs(elastic_scaled_integer)");
    static_assert(identical(
            elastic_scaled_integer<31, -30>{0.5},
            cnl::abs(elastic_scaled_integer<31, -30>{0.5})), "abs(elastic_scaled_integer)");
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(
            elastic_scaled_integer<63, -30>{0.5},
            cnl::abs(elastic_scaled_integer<63, -30>{0.5})), "abs(elastic_scaled_integer)");
    static_assert(identical(
            elastic_scaled_integer<63, -31>{0.5},
            cnl::abs(elastic_scaled_integer<63, -31>{0.5})), "abs(elastic_scaled_integer)");
#endif
}

////////////////////////////////////////////////////////////////////////////////
// useful constants

template<class T, bool IsSigned>
struct test_traits {
    static_assert(cnl::numeric_limits<T>::is_signed==IsSigned,
                  "cnl::numeric_limits<T>::is_signed fails for give type, T");
    static_assert(is_signed<T>::value==IsSigned,
                  "cnl::add_signedness failed cnl::numeric_limits test");
    static_assert(is_signed<cnl::add_signedness_t<T>>::value,
                  "cnl::add_signedness failed cnl::numeric_limits test");
    static_assert(!is_signed<cnl::remove_signedness_t<T>>::value,
                  "cnl::remove_signedness failed cnl::numeric_limits test");
};

template
struct test_traits<cnl::uint8, false>;

template
struct test_traits<cnl::uint16, false>;

template
struct test_traits<cnl::uint32, false>;

template
struct test_traits<cnl::uint64, false>;

template
struct test_traits<cnl::int8, true>;

template
struct test_traits<cnl::int16, true>;

template
struct test_traits<cnl::int32, true>;

template
struct test_traits<cnl::int64, true>;

////////////////////////////////////////////////////////////////////////////////
// useful functions

template<typename T>
CNL_NODISCARD constexpr int bit_count(T const& n)
{
    return n ? 1+bit_count(n & (n-1)) : 0;
}

static_assert(bit_count(0)==0, "bit_count test failed");
static_assert(bit_count(1)==1, "bit_count test failed");
static_assert(bit_count(2)==1, "bit_count test failed");
static_assert(bit_count(3)==2, "bit_count test failed");
static_assert(bit_count(5)==2, "bit_count test failed");
static_assert(bit_count(8)==1, "bit_count test failed");
static_assert(bit_count(13)==3, "bit_count test failed");
static_assert(bit_count(21)==3, "bit_count test failed");
static_assert(bit_count(34)==2, "bit_count test failed");
static_assert(bit_count(55)==5, "bit_count test failed");
static_assert(bit_count(89)==4, "bit_count test failed");
static_assert(bit_count(144)==2, "bit_count test failed");

// http://stackoverflow.com/a/5775825/671509
template<size_t Size>
struct print_num_as_error {
    explicit operator char() { return Size+256; }
}; //always overflow

namespace test_elastic_constant_literal {
    using cnl::_impl::identical;
    static_assert(identical(0_elastic, elastic_scaled_integer<1, 0>{0}), "");

    static_assert(identical(1_elastic, elastic_scaled_integer<1, 0>{1}), "");
    static_assert(identical(-1_elastic, elastic_scaled_integer<1, 0>{-1}), "");

    static_assert(identical(2_elastic, elastic_scaled_integer<1, 1>{2}), "");
    static_assert(identical(-2_elastic, elastic_scaled_integer<1, 1>{-2}), "");

    static_assert(identical(3_elastic, elastic_scaled_integer<2, 0>{3}), "");
    static_assert(identical(-3_elastic, elastic_scaled_integer<2, 0>{-3}), "");

    static_assert(identical(4_elastic, elastic_scaled_integer<1, 2>{4}), "");
    static_assert(identical(-4_elastic, elastic_scaled_integer<1, 2>{-4}), "");

    static_assert(identical(6_elastic, elastic_scaled_integer<2, 1>{6}), "");
    static_assert(identical(-6_elastic, elastic_scaled_integer<2, 1>{-6}), "");

    static_assert(identical(0xAA_elastic, elastic_scaled_integer<7, 1>{0xaa}), "");
    static_assert(identical(-0xaa_elastic, elastic_scaled_integer<7, 1>{-0xaa}), "");

    static_assert(identical(897341888_elastic, elastic_scaled_integer<24, 6>{897341888}), "");
    static_assert(identical(-897341888_elastic, elastic_scaled_integer<24, 6>{-897341888}), "");
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic_scaled_integer handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test
        : number_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using rep = cnl::_impl::rep_t<elastic_type>;
    using numeric_limits = cnl::numeric_limits<elastic_type>;

    using signed_type = cnl::add_signedness_t<elastic_type>;
    using unsigned_type = cnl::remove_signedness_t<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};
    static constexpr elastic_type negative_zero{-zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    static constexpr int integer_digits = cnl::_impl::integer_digits<elastic_type>::value;
    static constexpr int fractional_digits = cnl::_impl::fractional_digits<elastic_type>::value;
    static constexpr int digits = integer_digits+fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(cnl::numeric_limits<elastic_type>::is_signed==cnl::numeric_limits<rep>::is_signed,
                  "signedness of elastic_scaled_integer type differs from underlying fixed-point type");
    static_assert(cnl::numeric_limits<signed_type>::is_signed,
                  "signed version of elastic_scaled_integer type is not signed");
    static_assert(!cnl::numeric_limits<unsigned_type>::is_signed,
                  "unsigned version of elastic_scaled_integer type is signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test elastic_integer type

    static_assert(cnl::digits<rep>::value>=digits,
                  "not enough digits in rep type to represent elastic_scaled_integer values");

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::numeric_limits<elastic_scaled_integer>

#if !defined(_MSC_VER)
    static_assert(min==cnl::_impl::from_rep<elastic_type>(rep{1}), "cnl::numeric_limits test failed");
#endif
    static_assert(!is_less_than(max, min), "cnl::numeric_limits test failed");
    static_assert(is_less_than(zero, min), "cnl::numeric_limits test failed");
    static_assert(!is_less_than(zero, lowest), "cnl::numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "cnl::numeric_limits test failed");
    static_assert(cnl::numeric_limits<elastic_type>::is_signed==numeric_limits::is_signed,
                  "cnl::numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || (elastic_type{.5} != .5), "cnl::numeric_limits test failed");

    static constexpr rep max_integer{to_rep(max)};
#if ! defined(_MSC_VER)
    static_assert(bit_count<cnl::_impl::rep_t<rep>>(to_rep(max_integer))==digits, "cnl::numeric_limits test failed");
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
            cnl::_impl::integer_digits<negate_result>::value==cnl::_impl::integer_digits<elastic_type>::value,
            "negative of positive value has wrong number of integer digits");
    static_assert(
            cnl::_impl::fractional_digits<negate_result>::value==cnl::_impl::fractional_digits<elastic_type>::value,
            "negative of positive value has wrong number of fraction digits");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(cnl::numeric_limits<decltype(zero+zero)>::is_signed
                  ==cnl::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during add");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}+unsigned_type{zero})>::is_signed,
                  "signedness is lost during add");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_less_than(max-min, max), "operator- test failed");

    // NOLINTNEXTLINE(misc-redundant-expression)
    static_assert(cnl::numeric_limits<decltype(zero-zero)>::is_signed,
                  "signedness is lost during subtract");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}-unsigned_type{zero})>::is_signed,
                  "signedness is lost during subtract");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator*

    static_assert(is_equal_to(min*make_elastic_scaled_integer(0_c), zero), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_scaled_integer(1_c), min), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_scaled_integer(2_c), min+min), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_scaled_integer(3_c), min+min+min), "operator* test failed");

    static_assert(cnl::numeric_limits<decltype(zero*zero)>::is_signed
                  ==cnl::numeric_limits<decltype(zero)>::is_signed,
                  "signedness is lost during multiply");
    static_assert(cnl::numeric_limits<decltype(zero*zero)>::is_signed
                  ==cnl::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}*unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator/

    static_assert(!is_less_than(min, min/make_elastic_scaled_integer(2_c)), "operator/ test failed");
    static_assert(is_equal_to(min/make_elastic_scaled_integer(1_c), min), "operator/ test failed");
    // NOLINTNEXTLINE(misc-redundant-expression)
    static_assert(is_equal_to((min+min)/make_elastic_scaled_integer(2_c), min), "operator/ test failed");
    static_assert(is_equal_to((min+min+min)/make_elastic_scaled_integer(3_c), min), "operator/ test failed");
    // NOLINTNEXTLINE(misc-redundant-expression)
    static_assert(cnl::numeric_limits<decltype(zero/zero)>::is_signed==cnl::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}/unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");
#if ! defined(_MSC_VER)
    static_assert(identical(
            cnl::elastic_scaled_integer<12, -7>{3./4},
            cnl::make_scaled_integer(cnl::make_fraction(
                    cnl::elastic_scaled_integer<10, -5>{1.5},
                    cnl::elastic_integer<2>{2}))),
                  "operator/ test failed");
    static_assert(identical(
            cnl::elastic_scaled_integer<12, -5>{4./3},
            cnl::make_scaled_integer(cnl::make_fraction(
                    cnl::elastic_integer<2>{2},
                    cnl::elastic_scaled_integer<10, -5>{1.5}))),
                  "operator/ test failed");
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::from_rep

    static_assert(
            !cnl::_impl::from_rep<cnl::scaled_integer<cnl::elastic_integer<31, unsigned int>, cnl::power<-33>>>(0),
            "cnl::from_rep<scaled_integer<elastic_integer>>(int)");
};

TEST(elastic_scaled_integer, over_int) {  // NOLINT
    auto q = cnl::elastic_scaled_integer<10, -5>{1.5}/elastic_integer<2>{2};
    auto e = cnl::elastic_scaled_integer<12, -7>{3./4};
    EXPECT_EQ(e, q);
}

TEST(elastic_scaled_integer, int_over) {  // NOLINT
    auto f = cnl::make_fraction(elastic_integer<2>{2}, elastic_scaled_integer<10, -5>{1.5});
    auto q = cnl::make_scaled_integer(f);
    auto e = elastic_scaled_integer<12, -5>{4./3};
    EXPECT_EQ(e, q);
}

TEST(elastic_scaled_integer, issue_88)  // NOLINT
{
    using fix_t = cnl::elastic_scaled_integer<30, -16>;
    fix_t a = 2.0F;
    fix_t b = 1.0F;
    fix_t c = 1.0F;
    EXPECT_EQ(static_cast<float>(a),2.0F);
    EXPECT_EQ(static_cast<float>(b),1.0F);
    EXPECT_EQ(static_cast<float>(c),1.0F);
    fix_t d = c + a*b;
    EXPECT_EQ(static_cast<float>(d),3.0F);
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic_scaled_integer handles negative values;
// should pass for all signed specializations

template<class Elastic>
struct signed_elastic_test :
        // test type traits given this type is signed
        test_traits<Elastic, true>,

        // perform positive value tests against signed elastic_scaled_integer specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using rep = cnl::_impl::rep_t<elastic_type>;
    using numeric_limits = cnl::numeric_limits<elastic_type>;

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
    static_assert(cnl::numeric_limits<elastic_type>::is_signed, "subject of test class is not reported as signed");
    static_assert(is_same<cnl::add_signedness_t<elastic_type>, elastic_type>::value,
                  "subject of test class is not reported as signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::numeric_limits<elastic_scaled_integer>

    static_assert(is_less_than(negative_min, min), "cnl::numeric_limits test failed");
    static_assert(is_equal_to(-max, lowest), "comparison test error");
    //static_assert(is_equal_to(elastic_type{min+max+lowest}, elastic_type{1}), "comparison test error");
    static_assert(numeric_limits::is_signed, "cnl::numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || elastic_type{-.5} != -.5, "cnl::numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between negative_min
    static_assert(is_equal_to(negative_min, negative_min), "comparison test error");

    // min vs negative_min
    static_assert(is_less_than<elastic_type>(negative_min, min), "comparison test error");

    // negative_min vs zero
    static_assert(is_less_than<elastic_type>(negative_min, zero), "comparison test error");

    // negative_min vs lowest
    static_assert(numeric_limits::is_signed
                  ? !is_less_than(negative_min, lowest)
                  : is_less_than(negative_min, lowest), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(is_equal_to(min+max+lowest, min), "operator+ test failed");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic_scaled_integer handles positive values;
// should pass for all unsigned specializations

template<class Elastic>
struct unsigned_elastic_test :
        // test type traits given this type is not signed
        test_traits<Elastic, false>,

        // perform positive value tests against unsigned elastic_scaled_integer specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using numeric_limits = cnl::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::numeric_limits<elastic_scaled_integer>

    static_assert(is_equal_to(lowest, zero), "cnl::numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "cnl::numeric_limits test failed");
};

////////////////////////////////////////////////////////////////////////////////
// given values for IntegerDigits and FractionalDigits parameters,
// triggers elastic_scaled_integer<> tests with signed and unsigned specializations

template<int IntegerDigits, int FractionalDigits>
struct elastic_test :
        // perform unsigned-specific value tests against unsigned elastic_scaled_integer specialization
        unsigned_elastic_test<elastic_scaled_integer<IntegerDigits+FractionalDigits, -FractionalDigits, unsigned>>,

        // perform negative value tests against signed elastic_scaled_integer specialization
        signed_elastic_test<elastic_scaled_integer<IntegerDigits+FractionalDigits, -FractionalDigits, signed>> {
};

////////////////////////////////////////////////////////////////////////////////
// given a value for IntegerDigits parameter,
// triggers elastic_scaled_integer<> tests against a range of values for FractionalDigits parameter

template<int IntegerDigits>
struct elastic_test_with_integer_digits
        : elastic_test<IntegerDigits, -IntegerDigits+1>
                , elastic_test<IntegerDigits, -IntegerDigits+2>
                , elastic_test<IntegerDigits, -IntegerDigits+31>
{
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic_scaled_integer tests against a range of values for IntegerDigits parameter

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<-2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<-16>;

template
struct elastic_test_with_integer_digits<63>;
