
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// cnl::safe_integer<> is intended to render warnings such as 4389 (signed/unsigned mismatch) moot
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4389)
#endif

#include <cnl/safe_integer.h>

#if defined(_MSC_VER)
#pragma warning(pop) 
#endif

#include "../number_test.h"

using cnl::_impl::identical;
using cnl::_impl::is_integer_or_float;
using cnl::_integer_impl::is_safe_integer;
using cnl::safe_integer;
using std::declval;
using std::is_same;

////////////////////////////////////////////////////////////////////////////////
// aliases for different partial instantiations of cnl::safe_integer

template<typename Rep = int>
using native_integer = safe_integer<Rep, cnl::native_overflow_tag>;

template<typename Rep = int>
using throwing_integer = safe_integer<Rep, cnl::throwing_overflow_tag>;

template<typename Rep = int>
using saturated_integer = safe_integer<Rep, cnl::saturated_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// cnl::safe_integer template parameters default

static_assert(
        is_same<cnl::safe_integer<>, cnl::safe_integer<int, cnl::throwing_overflow_tag>>::value,
        "wrong default template parameters for cnl::safe_integer");

////////////////////////////////////////////////////////////////////////////////
// std::common_type with cnl::safe_integer parameter

static_assert(is_same<
        typename std::common_type<int32_t, saturated_integer<int16_t>>::type,
        saturated_integer<int32_t>>::value, "cnl::_integer_impl::common_type test failure");

static_assert(is_same<
        std::common_type<saturated_integer<int8_t>, saturated_integer<int8_t>>::type,
        saturated_integer<int8_t>>::value, "cnl::_integer_impl::common_type test failure");

static_assert(is_same<
        std::common_type<saturated_integer<uint32_t>, float>::type,
        float>::value, "incorrect assumption about promotion");

static_assert(is_same<
        std::common_type<saturated_integer<uint32_t>, saturated_integer<int16_t>>::type,
        saturated_integer<uint32_t >>::value, "std::common_type test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_integer_impl::is_safe_integer

static_assert(!is_safe_integer<uint8_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<int8_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<uint16_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<int16_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<uint32_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<int32_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<uint64_t>::value, "cnl::is_safe_integer test failed");
static_assert(!is_safe_integer<int64_t>::value, "cnl::is_safe_integer test failed");

static_assert(is_safe_integer<saturated_integer<uint8_t>>::value,
        "cnl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int8_t>>::value,
        "cnl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<uint16_t>>::value,
        "cnl::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int16_t>>::value,
        "cnl::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<uint32_t>>::value,
        "cnl::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int32_t>>::value,
        "cnl::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<uint64_t>>::value,
        "cnl::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int64_t>>::value,
        "cnl::_integer_impl::is_safe_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::encompasses

static_assert(is_same<cnl::_impl::unsigned_or_float_t<cnl::int8>, cnl::uint8>::value, "");
static_assert(is_same<cnl::_impl::unsigned_or_float_t<cnl::uint8>, cnl::uint8>::value, "");
static_assert(is_same<cnl::_impl::unsigned_or_float_t<float>, float>::value, "");

static_assert(cnl::_impl::encompasses<short, short>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<short, int>::value, "cnl::_impl::encompasses test failed");
static_assert(cnl::_impl::encompasses<int, short>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::int8, cnl::uint8>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::uint8, cnl::int8>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::uint64, cnl::int64>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::int64, cnl::uint64>::value, "cnl::_impl::encompasses test failed");
static_assert(cnl::_impl::encompasses<long, unsigned short>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<unsigned short, long>::value, "cnl::_impl::encompasses test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::is_integer_or_float

static_assert(is_integer_or_float<saturated_integer<int64_t>>::value, "cnl::_integer_impl::is_safe_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::saturated_integer

// equality

static_assert(identical(saturated_integer<int>(1234), cnl::_impl::from_value<saturated_integer<uint8_t>>(1234)),
              "cnl::from_value<cnl::saturated_integer> test failed");
static_assert(cnl::_impl::operate(saturated_integer<uint8_t>(-1), 0, cnl::_impl::equal_tag),
              "cnl::saturated_integer equality test failed");

static_assert(identical(saturated_integer<int16_t>(32767), saturated_integer<int16_t>(5000000000L)), "cnl::saturated_integer equality test failed");
static_assert(saturated_integer<uint8_t>(-1)==0, "cnl::saturated_integer equality test failed");
static_assert(saturated_integer<int64_t>(5000000000L)!=saturated_integer<uint32_t>(5000000000L),
        "cnl::saturated_integer equality test failed");
static_assert(saturated_integer<signed>(-1)==saturated_integer<unsigned>(0),
        "cnl::saturated_integer equality test failed");

// assignment

static_assert(saturated_integer<uint8_t>(-1)==0, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(-.1)==0, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(0)==0, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1)==1, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254)==254, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254.99)==254, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(255)==255, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(256)==255, "cnl::saturated_integer test failed");

static_assert(saturated_integer<int8_t>(-129)==-128, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-128)==-128, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-127)==-127, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(126)==126, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(127)==127, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(128)==127, "cnl::saturated_integer test failed");

static_assert(saturated_integer<uint8_t>(-1e38f)==0, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(0.)==0, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1.f)==1, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254.)==254, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(255.f)==255, "cnl::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1e50)==255, "cnl::saturated_integer test failed");

static_assert(saturated_integer<int8_t>(-1e50)==-128, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-128.f)==-128, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-127.)==-127, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(126.f)==126, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(127.)==127, "cnl::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(1e38f)==127, "cnl::saturated_integer test failed");

static_assert(identical(throwing_integer<int32_t>{throwing_integer<uint16_t>{50005}}, throwing_integer<int32_t>{50005}),
              "throwing_integer test failed");
static_assert(identical(static_cast<throwing_integer<int32_t>>(throwing_integer<uint16_t>{50005}),
                        throwing_integer<int32_t>{50005}), "throwing_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::safe_integer<Rep, cnl::saturated_overflow_tag>

namespace {
    using cnl::_impl::equal_tag;
    using cnl::_impl::multiply_op;
    using cnl::_impl::multiply_tag;

    static_assert(cnl::_impl::is_derived_from_number_base<saturated_integer<short>>::value, "");

    static_assert(identical(
            to_rep(saturated_integer<short>(1234)),
            short(1234)), "cnl::to_rep(saturated_integer<>) test failed");

    static_assert(identical(
            cnl::_impl::operate(saturated_integer<short>(1234), 2., cnl::_impl::multiply_tag),
            2468.), "cnl::saturated_integer test failed");

    static_assert(operate(saturated_integer<int16_t>(32767), saturated_integer<int16_t>(5000000000L), equal_tag), "");

    static_assert(identical(
        cnl::make_safe_integer<cnl::saturated_overflow_tag>(cnl::_overflow_impl::operate
                <cnl::saturated_overflow_tag, multiply_op>()(
            cnl::_impl::to_rep(cnl::safe_integer<signed char, cnl::saturated_overflow_tag>{30}),
            cnl::_impl::to_rep(cnl::safe_integer<signed char, cnl::saturated_overflow_tag>{40}))),
        cnl::safe_integer<int, cnl::saturated_overflow_tag>{1200}), "");

    static_assert(identical(
            operate_common_tag(
                    cnl::saturated_overflow,
                    multiply_tag,
                    cnl::safe_integer<signed char, cnl::saturated_overflow_tag>{30},
                    cnl::safe_integer<signed char, cnl::saturated_overflow_tag>{40}),
            cnl::safe_integer<int, cnl::saturated_overflow_tag>{1200}), "");
    
    static_assert(identical(
            operate(
                    cnl::safe_integer<signed char, cnl::saturated_overflow_tag>{30},
                    cnl::safe_integer<signed char, cnl::saturated_overflow_tag>{40},
                    multiply_tag),
            cnl::safe_integer<int, cnl::saturated_overflow_tag>{1200}), "");

    static_assert(
            identical(operate(3u, throwing_integer<cnl::uint8>{4}, multiply_tag), throwing_integer<unsigned>{12}),
            "cnl::_impl::operate test failed");
}

////////////////////////////////////////////////////////////////////////////////
// constructor

namespace test_constructor {
    using namespace cnl::literals;

    static_assert(throwing_integer<int>(7_c) > throwing_integer<int>(6_c), "");
}

////////////////////////////////////////////////////////////////////////////////
// comparison

namespace test_equal {
    using cnl::_impl::equal_tag;
    using cnl::_impl::operate;

    static_assert(operate(throwing_integer<short>{0}, 0., equal_tag), "");
}


////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert(saturated_integer<int8_t>(saturated_integer<int8_t>(100)+saturated_integer<int8_t>(100))==cnl::numeric_limits<int8_t>::max(),
        "cnl::saturated_integer test failed");

// subtraction
static_assert(saturated_integer<uint8_t>(100)-saturated_integer<int16_t>(100000)==100-cnl::numeric_limits<int16_t>::max(),
        "cnl::saturated_integer test failed");
static_assert(throwing_integer<char>{0}-throwing_integer<char>{0}==throwing_integer<char>{0}, "");

// multiplication
static_assert(identical(
        saturated_integer<int8_t>(-5)*saturated_integer<int32_t>(-14),
        saturated_integer<int>(70)), "cnl::saturated_integer test failed");

static_assert(identical(
        saturated_integer<short>(1234)*2.,
        2468.), "cnl::saturated_integer test failed");

// division
#if !defined(_MSC_VER)
static_assert(saturated_integer<uint32_t>(1)/saturated_integer<int16_t>(-10)==0U, "cnl::saturated_integer test failed");
#endif
static_assert(saturated_integer<int16_t>(77)/saturated_integer<int8_t>(-11)==-7, "cnl::saturated_integer test failed");
static_assert(int16_t(31)/saturated_integer<int8_t>(-2)==-15, "cnl::saturated_integer test failed");

static_assert(is_same<decltype(declval<saturated_integer<>>() / declval<double>()), double>::value, "");

////////////////////////////////////////////////////////////////////////////////
// cnl::numeric_limits

// cnl::numeric_limits<cnl::safe_integer<>>::is_integer
static_assert(cnl::numeric_limits<cnl::safe_integer<int8_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<uint8_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int16_t, cnl::saturated_overflow_tag> const>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<uint16_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int32_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<uint32_t, cnl::saturated_overflow_tag> const>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int64_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<uint64_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(cnl::numeric_limits<cnl::int128>::is_specialized,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int, cnl::saturated_overflow_tag> const>::is_specialized,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_specialized,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<cnl::uint128, cnl::saturated_overflow_tag> const>::is_integer,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
#endif

// cnl::numeric_limits<cnl::safe_integer<>>::is_signed
static_assert(cnl::numeric_limits<cnl::safe_integer<int8_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::safe_integer<uint8_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int16_t, cnl::saturated_overflow_tag> const>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::safe_integer<uint16_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int32_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::safe_integer<uint32_t, cnl::saturated_overflow_tag> const>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::safe_integer<int64_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::safe_integer<uint64_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(cnl::numeric_limits<cnl::safe_integer<cnl::int128, cnl::saturated_overflow_tag> const>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::safe_integer<cnl::uint128, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::safe_integer<>> test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// std::numeric_limits

// std::numeric_limits<cnl::safe_integer<>>::is_integer
static_assert(std::numeric_limits<cnl::safe_integer<int8_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint8_t, cnl::saturated_overflow_tag> const>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int16_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint16_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int32_t, cnl::saturated_overflow_tag> const>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint32_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int64_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint64_t, cnl::saturated_overflow_tag> const>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(std::numeric_limits<cnl::safe_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<cnl::uint128, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
#endif

// std::numeric_limits<cnl::safe_integer<>>::is_integer
static_assert(std::numeric_limits<cnl::safe_integer<int8_t, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint8_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int16_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint16_t, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int32_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint32_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int64_t, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<uint64_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(std::numeric_limits<cnl::safe_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<cnl::uint128, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
#endif

// std::numeric_limits<cnl::safe_integer<>>::is_signed
static_assert(std::numeric_limits<cnl::safe_integer<int8_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::safe_integer<uint8_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int16_t, cnl::saturated_overflow_tag> const>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::safe_integer<uint16_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int32_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::safe_integer<uint32_t, cnl::saturated_overflow_tag> const>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(std::numeric_limits<cnl::safe_integer<int64_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::safe_integer<uint64_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(std::numeric_limits<cnl::safe_integer<cnl::int128, cnl::saturated_overflow_tag> const>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::safe_integer<cnl::uint128, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::safe_integer<>> test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// traits

namespace test_digits {
    using cnl::digits;
    using cnl::set_digits_t;

    static_assert(digits<safe_integer<int8_t>>::value == 7, "cnl::digits / cnl::set_digits test failed");
    static_assert(identical(set_digits_t<safe_integer<cnl::uint8>, 12>{2000}, safe_integer<cnl::uint16>{2000}),
                  "cnl::digits / cnl::set_digits test failed");
}

namespace test_make_signed {
    using cnl::make_signed;

    // cnl::make_unsigned<cnl::saturated_integer<>>
    static_assert(is_same<make_signed<saturated_integer<int8_t >>::type, saturated_integer<int8_t >>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<uint8_t >>::type, saturated_integer<int8_t >>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<int16_t>>::type, saturated_integer<int16_t>>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<uint16_t>>::type, saturated_integer<int16_t>>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<int32_t>>::type, saturated_integer<int32_t>>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<uint32_t>>::type, saturated_integer<int32_t>>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<int64_t>>::type, saturated_integer<int64_t>>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
    static_assert(is_same<make_signed<saturated_integer<uint64_t>>::type, saturated_integer<int64_t>>::value,
                  "cnl::make_signed<cnl::saturated_integer<>> test failed");
}

namespace test_make_unsigned {
    using cnl::make_unsigned;

    // cnl::make_unsigned<cnl::saturated_integer<>>
    static_assert(is_same<make_unsigned<saturated_integer<int8_t >>::type, saturated_integer<uint8_t >>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<uint8_t >>::type, saturated_integer<uint8_t >>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<int16_t>>::type, saturated_integer<uint16_t>>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<uint16_t>>::type, saturated_integer<uint16_t>>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<int32_t>>::type, saturated_integer<uint32_t>>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<uint32_t>>::type, saturated_integer<uint32_t>>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<int64_t>>::type, saturated_integer<uint64_t>>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
    static_assert(is_same<make_unsigned<saturated_integer<uint64_t>>::type, saturated_integer<uint64_t>>::value,
                  "cnl::make_unsigned<cnl::saturated_integer<>>::type test failed");
}

namespace test_to_rep {
    using cnl::_impl::to_rep;

    static_assert(identical(
            to_rep(native_integer<cnl::uint8>{3}) * cnl::_num_traits_impl::pow<native_integer<cnl::uint8>>(2, 3),
            native_integer<int>{24}), "");
}

namespace test_from_rep {
    using cnl::from_rep;

    static_assert(identical(from_rep<native_integer<int>>()(746352), native_integer<int>{746352}), "");
    static_assert(from_rep<native_integer<short>>()(1), "");
    static_assert(from_rep<throwing_integer<short>>()(1), "");
}

namespace test_impl_from_rep {
    using cnl::_impl::from_rep;

    static_assert(identical(from_rep<native_integer<int>>(746352), native_integer<int>{746352}), "");
    static_assert(from_rep<native_integer<short>>(1), "");
    static_assert(from_rep<throwing_integer<short>>(1), "");
}

namespace test_scale {
    using cnl::scale;

    static_assert(identical(
            scale<throwing_integer<cnl::int32>>()(throwing_integer<cnl::int32>{1}, 2, 15),
            throwing_integer<cnl::int32>{32768}),
                  "scale<safe_integer<>> test failed");
    static_assert(identical(
            scale<throwing_integer<cnl::int32>>()(throwing_integer<cnl::uint16>{1}, 2, 15),
            throwing_integer<cnl::int32>{32768}),
                  "scale<safe_integer<>> test failed");

    static_assert(identical(
            scale<saturated_integer<cnl::uint16>>()(saturated_integer<cnl::uint8>{0x1234}, 2, 8),
            saturated_integer<int>{0xff00}),
                  "scale<safe_integer<>> test failed");
}

namespace test_impl_scale {
    using cnl::_impl::scale;

    static_assert(identical(scale(native_integer<cnl::uint8>{3}, 2, 4), native_integer<int>{48}),
                  "scale<safe_integer<>> test failed");
}

////////////////////////////////////////////////////////////////////////////////
// common safe_integer characteristics

namespace {
    template<class SafeInteger>
    struct test_safe_integer {
        using safe_integer = SafeInteger;
        
        using rep = typename safe_integer::rep;
        using overflow_tag = typename safe_integer::overflow_tag;
        static_assert(is_same<cnl::safe_integer<rep, overflow_tag>, safe_integer>::value, "cnl::safe_integer test failed");

        static constexpr auto default_initialized = safe_integer{0};
        static_assert(!default_initialized, "cnl::safe_integer test failed");

        static_assert(+default_initialized == default_initialized, "cnl::safe_integer test failed");
#if !defined(_MSC_VER)
        static_assert(-default_initialized == default_initialized, "cnl::safe_integer test failed");
#endif
        static_assert(default_initialized+default_initialized == default_initialized, "cnl::safe_integer test failed");
        static_assert(default_initialized-default_initialized == default_initialized, "cnl::safe_integer test failed");
        static_assert(default_initialized*default_initialized == default_initialized, "cnl::safe_integer test failed");
        static_assert(default_initialized/1 == default_initialized, "cnl::safe_integer test failed");
    };
}

template struct number_test_by_rep_by_tag<safe_integer, cnl::native_overflow_tag, test_safe_integer>;
template struct number_test_by_rep_by_tag<safe_integer, cnl::saturated_overflow_tag, test_safe_integer>;
#if defined(CNL_EXCEPTIONS_ENABLED)
template struct number_test_by_rep_by_tag<safe_integer, cnl::throwing_overflow_tag, test_safe_integer>;
#endif
