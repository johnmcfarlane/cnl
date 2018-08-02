
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// cnl::overflow_integer<> is intended to render warnings such as 4389 (signed/unsigned mismatch) moot

#include <cnl/overflow_integer.h>

#include "../number_test.h"

#include <gtest/gtest.h>

using cnl::_impl::identical;
using cnl::_integer_impl::is_overflow_integer;
using cnl::overflow_integer;
using std::declval;
using std::is_same;

////////////////////////////////////////////////////////////////////////////////
// aliases for different partial instantiations of cnl::overflow_integer

template<typename Rep = int>
using native_integer = overflow_integer<Rep, cnl::native_overflow_tag>;

template<typename Rep = int>
using throwing_integer = overflow_integer<Rep, cnl::throwing_overflow_tag>;

template<typename Rep = int>
using saturated_integer = overflow_integer<Rep, cnl::saturated_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// cnl::overflow_integer template parameters default

static_assert(
        is_same<cnl::overflow_integer<>, cnl::overflow_integer<int, cnl::trapping_overflow_tag>>::value,
        "wrong default template parameters for cnl::overflow_integer");

////////////////////////////////////////////////////////////////////////////////
// cnl::_integer_impl::is_overflow_integer

static_assert(!is_overflow_integer<uint8_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<int8_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<uint16_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<int16_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<uint32_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<int32_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<uint64_t>::value, "cnl::is_overflow_integer test failed");
static_assert(!is_overflow_integer<int64_t>::value, "cnl::is_overflow_integer test failed");

static_assert(is_overflow_integer<saturated_integer<uint8_t>>::value,
        "cnl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<int8_t>>::value,
        "cnl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<uint16_t>>::value,
        "cnl::_integer_impl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<int16_t>>::value,
        "cnl::_integer_impl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<uint32_t>>::value,
        "cnl::_integer_impl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<int32_t>>::value,
        "cnl::_integer_impl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<uint64_t>>::value,
        "cnl::_integer_impl::is_overflow_integer test failed");
static_assert(is_overflow_integer<saturated_integer<int64_t>>::value,
        "cnl::_integer_impl::is_overflow_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::saturated_integer

// equality

static_assert(identical(saturated_integer<int>(1234), cnl::_impl::make_number<saturated_integer<uint8_t>>(1234)),
              "cnl::from_value<cnl::saturated_integer> test failed");
static_assert(cnl::_impl::binary_operator<cnl::_impl::equal_op, saturated_integer<uint8_t>, int>()(
        saturated_integer<uint8_t>(-1), 0), "cnl::saturated_integer equality test failed");

static_assert(identical(saturated_integer<int16_t>(32767), saturated_integer<int16_t>(5000000000L)), "cnl::saturated_integer equality test failed");
static_assert(saturated_integer<uint8_t>(-1)==0, "cnl::saturated_integer equality test failed");
static_assert(saturated_integer<int64_t>(5000000000L)!=saturated_integer<uint32_t>(5000000000L),
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

namespace saturated_binary_operator {
    using cnl::_impl::multiply_op;

    static_assert(cnl::_impl::is_derived_from_number_base<saturated_integer<short>>::value, "");

    static_assert(identical(
            cnl::_impl::to_rep(saturated_integer<short>(1234)),
            short(1234)), "to_rep(saturated_integer<>) test failed");

    static_assert(identical(cnl::_impl::binary_operator<cnl::_impl::multiply_op, saturated_integer<short>, float>()(
            saturated_integer<short>(1234), 2.), 2468.f), "cnl::saturated_integer test failed");

    static_assert(
            cnl::_impl::binary_operator<cnl::_impl::equal_op, saturated_integer<int16_t>, saturated_integer<int16_t>>()(
                    saturated_integer<int16_t>(32767), saturated_integer<int16_t>(5000000000L)), "");

    static_assert(identical(
            cnl::make_overflow_int<cnl::saturated_overflow_tag>(
                    cnl::_impl::tagged_binary_operator<cnl::saturated_overflow_tag, multiply_op>()(
                            cnl::_impl::to_rep(saturated_integer<signed char>{30}),
                            cnl::_impl::to_rep(saturated_integer<signed char>{40}))),
            saturated_integer<int>{1200}), "");

    static_assert(identical(
            cnl::_impl::binary_operator<multiply_op, saturated_integer<signed char>, saturated_integer<signed char>>()(
                    saturated_integer<signed char>{30}, saturated_integer<signed char>{40}),
            saturated_integer<int>{1200}), "");

    static_assert(identical(
            cnl::_impl::binary_operator<
                    cnl::_impl::multiply_op, saturated_integer<signed char>, saturated_integer<signed char>>()(30, 40),
            saturated_integer<int>{1200}), "");

    static_assert(identical(
            cnl::_impl::binary_operator<
                    cnl::_impl::multiply_op,
                    unsigned,
                    throwing_integer<cnl::uint8>>()(3u, throwing_integer<cnl::uint8>{4}),
            throwing_integer<unsigned>{12}),
            "cnl::_impl::binary_operator test failed");
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
    static_assert(cnl::_impl::binary_operator<cnl::_impl::equal_op, throwing_integer<short>, double>()(
            throwing_integer<short>{0}, 0.), "");
    static_assert(throwing_integer<short>{0}==0., "");
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
static_assert(saturated_integer<int16_t>(77)/saturated_integer<int8_t>(-11)==-7, "cnl::saturated_integer test failed");
static_assert(int16_t(31)/saturated_integer<int8_t>(-2)==-15, "cnl::saturated_integer test failed");

static_assert(is_same<decltype(declval<saturated_integer<>>() / declval<double>()), double>::value, "");

////////////////////////////////////////////////////////////////////////////////
// cnl::numeric_limits

// cnl::numeric_limits<cnl::overflow_integer<>>::is_integer
static_assert(cnl::numeric_limits<cnl::overflow_integer<int8_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<uint8_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int16_t, cnl::saturated_overflow_tag> const>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<uint16_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int32_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<uint32_t, cnl::saturated_overflow_tag> const>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int64_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<uint64_t, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(cnl::numeric_limits<cnl::int128>::is_specialized,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int, cnl::saturated_overflow_tag> const>::is_specialized,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_specialized,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<cnl::uint128, cnl::saturated_overflow_tag> const>::is_integer,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
#endif

// cnl::numeric_limits<cnl::overflow_integer<>>::is_signed
static_assert(cnl::numeric_limits<cnl::overflow_integer<int8_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::overflow_integer<uint8_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int16_t, cnl::saturated_overflow_tag> const>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::overflow_integer<uint16_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int32_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::overflow_integer<uint32_t, cnl::saturated_overflow_tag> const>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(cnl::numeric_limits<cnl::overflow_integer<int64_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::overflow_integer<uint64_t, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(cnl::numeric_limits<cnl::overflow_integer<cnl::int128, cnl::saturated_overflow_tag> const>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!cnl::numeric_limits<cnl::overflow_integer<cnl::uint128, cnl::saturated_overflow_tag>>::is_signed,
        "cnl::numeric_limits<cnl::overflow_integer<>> test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// std::numeric_limits

// std::numeric_limits<cnl::overflow_integer<>>::is_integer
static_assert(std::numeric_limits<cnl::overflow_integer<int8_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint8_t, cnl::saturated_overflow_tag> const>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int16_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint16_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int32_t, cnl::saturated_overflow_tag> const>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint32_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int64_t, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint64_t, cnl::saturated_overflow_tag> const>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(std::numeric_limits<cnl::overflow_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<cnl::uint128, cnl::saturated_overflow_tag>>::is_specialized,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
#endif

// std::numeric_limits<cnl::overflow_integer<>>::is_integer
static_assert(std::numeric_limits<cnl::overflow_integer<int8_t, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint8_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int16_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint16_t, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int32_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint32_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int64_t, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<uint64_t, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(std::numeric_limits<cnl::overflow_integer<cnl::int128, cnl::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<cnl::uint128, cnl::saturated_overflow_tag> const>::is_integer,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
#endif

// std::numeric_limits<cnl::overflow_integer<>>::is_signed
static_assert(std::numeric_limits<cnl::overflow_integer<int8_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::overflow_integer<uint8_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int16_t, cnl::saturated_overflow_tag> const>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::overflow_integer<uint16_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int32_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::overflow_integer<uint32_t, cnl::saturated_overflow_tag> const>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(std::numeric_limits<cnl::overflow_integer<int64_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::overflow_integer<uint64_t, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(std::numeric_limits<cnl::overflow_integer<cnl::int128, cnl::saturated_overflow_tag> const>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
static_assert(!std::numeric_limits<cnl::overflow_integer<cnl::uint128, cnl::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<cnl::overflow_integer<>> test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// traits

namespace test_digits {
    using cnl::digits;
    using cnl::set_digits_t;

    static_assert(digits<overflow_integer<int8_t>>::value == 7, "cnl::digits / cnl::set_digits test failed");
    static_assert(identical(set_digits_t<overflow_integer<cnl::uint8>, 12>{2000}, overflow_integer<cnl::uint16>{2000}),
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
    static_assert(identical(cnl::_impl::to_rep(cnl::overflow_integer<uint64_t>{54}), uint64_t{54}),
            "cnl::_impl::to_rep<overflow_integer<>>()");
    static_assert(identical(cnl::_impl::to_rep(native_integer<cnl::uint8>{3}), cnl::uint8{3}), "");
}

namespace test_from_rep {
    using cnl::from_rep;

    static_assert(identical(from_rep<native_integer<int>>()(746352), native_integer<int>{746352}), "");
    static_assert(from_rep<native_integer<short>>()(short{1}), "");
    static_assert(from_rep<throwing_integer<short>>()(short{1}), "");
}

namespace test_impl_from_rep {

    static_assert(identical(cnl::from_rep<native_integer<int>>{}(746352), native_integer<int>{746352}), "");
    static_assert(identical(cnl::from_rep<native_integer<short>>{}(1), native_integer<int>{1}), "");
    static_assert(identical(cnl::from_rep<throwing_integer<short>>{}(1), throwing_integer<int>{1}), "");
}

namespace test_from_value {
    static_assert(identical(
            native_integer<long long>{746352},
            cnl::_impl::make_number<native_integer<int>>(746352LL)), "");
    static_assert(identical(
            746352LL,
            cnl::_impl::make_number<int>(746352LL)), "");
    static_assert(identical(
            native_integer<long long>{746352},
            cnl::_impl::make_number<native_integer<int>>(native_integer<long long>{746352})), "");
}

namespace test_minus {
    static_assert(identical(-throwing_integer<short>(1), throwing_integer<int>(-1)), "");
}

namespace test_plus {
    static_assert(identical(+throwing_integer<short>(1), throwing_integer<int>(1)), "");
}

namespace test_shift_left {
    static_assert(
            identical(saturated_integer<std::uint8_t>(255) << 30u, cnl::numeric_limits<saturated_integer<int>>::max()),
            "");
    static_assert(identical(throwing_integer<std::int8_t>(1) << 10, throwing_integer<int>(1024)), "");
}

namespace test_scale {
    static_assert(identical(
            cnl::_impl::scale<15, 2>(throwing_integer<cnl::int32>{1}),
            throwing_integer<cnl::int32>{32768}),
                  "fixed_width_scale<overflow_integer<>> test failed");
    static_assert(identical(
            cnl::_impl::scale<15, 2>(throwing_integer<cnl::int32>{1}),
            throwing_integer<cnl::int32>{32768}),
                  "fixed_width_scale<overflow_integer<>> test failed");

    static_assert(identical(
            cnl::from_rep<saturated_integer<cnl::uint16>>{}(0x1234),
            saturated_integer<int>{0x1234}),
            "fixed_width_scale<overflow_integer<>> test failed");
    static_assert(identical(
            cnl::_impl::scale<8, 2>(saturated_integer<cnl::uint16>{0x1234}),
            saturated_integer<int>{0x123400}),
            "fixed_width_scale<overflow_integer<>> test failed");
    static_assert(identical(cnl::_impl::scale<4, 2>(native_integer<cnl::uint8>{3}), native_integer<int>{48}),
                  "fixed_width_scale<overflow_integer<>> test failed");
}

namespace test_used_digits {
    static_assert(cnl::used_digits(cnl::overflow_integer<unsigned int, cnl::native_overflow_tag>{0})==0,
            "used_digits test failed");
}

namespace {
    TEST(overflow_integer, pre_increment) {
        auto a = cnl::overflow_integer<>{INT_MAX-1};
        auto& b = ++ a;
        static_assert(
                std::is_same<decltype(b), cnl::overflow_integer<>&>::value,
                "static_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(INT_MAX, b) << "static_integer pre-increment";
    }

    TEST(overflow_integer, pre_decrement) {
        auto a = overflow_integer<>{INT_MIN+1};
        auto& b = -- a;
        static_assert(
                std::is_same<decltype(b), cnl::overflow_integer<>&>::value,
                "static_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(INT_MIN, b) << "static_integer pre-increment";
    }

    TEST(overflow_integer, post_increment) {
        auto a = cnl::overflow_integer<>{INT_MAX-1};
        auto const& b = a ++;
        static_assert(
                std::is_same<decltype(b), cnl::overflow_integer<> const&>::value,
                "static_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(INT_MAX, a) << "static_integer pre-increment";
        ASSERT_EQ(INT_MAX-1, b) << "static_integer pre-increment";
    }

    TEST(overflow_integer, post_decrement) {
        auto a = overflow_integer<>{INT_MIN+1};
        auto const& b = a --;
        static_assert(
                std::is_same<decltype(b), cnl::overflow_integer<> const&>::value,
                "static_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "static_integer pre-increment return address";
        ASSERT_EQ(INT_MIN, a) << "static_integer pre-increment";
        ASSERT_EQ(INT_MIN+1, b) << "static_integer pre-increment";
    }

    TEST(overflow_integer, pre_increment_overflow) {
        auto a = cnl::overflow_integer<>{INT_MAX};
        ASSERT_DEATH(++ a, "positive overflow");
    }

    TEST(overflow_integer, pre_decrement_overflow) {
        auto a = cnl::overflow_integer<>{INT_MIN};
        ASSERT_DEATH(-- a, "negative overflow");
    }

    TEST(overflow_integer, post_increment_overflow) {
        auto a = cnl::overflow_integer<>{INT_MAX};
        ASSERT_DEATH(a ++, "positive overflow");
    }

    TEST(overflow_integer, post_decrement_overflow) {
        auto a = cnl::overflow_integer<>{INT_MIN};
        ASSERT_DEATH(a --, "negative overflow");
    }
}

////////////////////////////////////////////////////////////////////////////////
// common overflow_integer characteristics

namespace {
    template<class OverflowInt>
    struct test_overflow_int {
        using overflow_integer = OverflowInt;

        using rep = typename overflow_integer::rep;
        using overflow_tag = typename overflow_integer::overflow_tag;
        static_assert(is_same<cnl::overflow_integer<rep, overflow_tag>, overflow_integer>::value, "cnl::overflow_integer test failed");

        static constexpr auto default_initialized = overflow_integer{0};
        static_assert(!default_initialized, "cnl::overflow_integer test failed");

        static_assert(+default_initialized == default_initialized, "cnl::overflow_integer test failed");
#if !defined(_MSC_VER)
        static_assert(-default_initialized == default_initialized, "cnl::overflow_integer test failed");
#endif
        static_assert(default_initialized+default_initialized == default_initialized, "cnl::overflow_integer test failed");
        static_assert(default_initialized-default_initialized == default_initialized, "cnl::overflow_integer test failed");
        static_assert(default_initialized*default_initialized == default_initialized, "cnl::overflow_integer test failed");
        static_assert(default_initialized/1 == default_initialized, "cnl::overflow_integer test failed");
    };
}

template struct number_test_by_rep_by_tag<overflow_integer, cnl::native_overflow_tag, test_overflow_int>;
template struct number_test_by_rep_by_tag<overflow_integer, cnl::saturated_overflow_tag, test_overflow_int>;
#if defined(CNL_EXCEPTIONS_ENABLED)
template struct number_test_by_rep_by_tag<overflow_integer, cnl::throwing_overflow_tag, test_overflow_int>;
#endif
