
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// sg14::safe_integer<> is intended to render warnings such as 4389 (signed/unsigned mismatch) moot
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4389)
#endif

#include <sg14/auxiliary/safe_integer.h>

#if defined(_MSC_VER)
#pragma warning(pop) 
#endif

#include "number_test.h"

using sg14::_impl::identical;
using sg14::_impl::is_integer_or_float;
using sg14::_integer_impl::is_safe_integer;
using sg14::_impl::make_signed;
using sg14::make_unsigned;
using sg14::safe_integer;
using std::declval;
using std::is_same;
using std::numeric_limits;

////////////////////////////////////////////////////////////////////////////////
// aliases for different partial instantiations of sg14::safe_integer

template<typename Rep = int>
using native_integer = safe_integer<Rep, sg14::native_overflow_tag>;

template<typename Rep = int>
using throwing_integer = safe_integer<Rep, sg14::throwing_overflow_tag>;

template<typename Rep = int>
using saturated_integer = safe_integer<Rep, sg14::saturated_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// sg14::safe_integer template parameters default

static_assert(
        is_same<sg14::safe_integer<>, sg14::safe_integer<int, sg14::throwing_overflow_tag>>::value,
        "wrong default template parameters for sg14::safe_integer");

////////////////////////////////////////////////////////////////////////////////
// std::common_type with sg14::safe_integer parameter

static_assert(is_same<
        typename std::common_type<int32_t, saturated_integer<int16_t>>::type,
        saturated_integer<int32_t>>::value, "sg14::_integer_impl::common_type test failure");

static_assert(is_same<
        std::common_type<saturated_integer<int8_t>, saturated_integer<int8_t>>::type,
        saturated_integer<int8_t>>::value, "sg14::_integer_impl::common_type test failure");

static_assert(is_same<
        std::common_type<saturated_integer<uint32_t>, float>::type,
        float>::value, "incorrect assumption about promotion");

static_assert(is_same<
        std::common_type<saturated_integer<uint32_t>, saturated_integer<int16_t>>::type,
        saturated_integer<uint32_t >>::value, "std::common_type test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_integer_impl::is_safe_integer

static_assert(!is_safe_integer<uint8_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<int8_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<uint16_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<int16_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<uint32_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<int32_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<uint64_t>::value, "sg14::is_safe_integer test failed");
static_assert(!is_safe_integer<int64_t>::value, "sg14::is_safe_integer test failed");

static_assert(is_safe_integer<saturated_integer<uint8_t>>::value,
        "sg14::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int8_t>>::value,
        "sg14::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<uint16_t>>::value,
        "sg14::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int16_t>>::value,
        "sg14::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<uint32_t>>::value,
        "sg14::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int32_t>>::value,
        "sg14::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<uint64_t>>::value,
        "sg14::_integer_impl::is_safe_integer test failed");
static_assert(is_safe_integer<saturated_integer<int64_t>>::value,
        "sg14::_integer_impl::is_safe_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::encompasses

static_assert(is_same<sg14::_impl::unsigned_or_float_t<std::int8_t>, std::uint8_t>::value, "");
static_assert(is_same<sg14::_impl::unsigned_or_float_t<std::uint8_t>, std::uint8_t>::value, "");
static_assert(is_same<sg14::_impl::unsigned_or_float_t<float>, float>::value, "");

static_assert(sg14::_impl::encompasses<short, short>::value, "sg14::_impl::encompasses test failed");
static_assert(!sg14::_impl::encompasses<short, int>::value, "sg14::_impl::encompasses test failed");
static_assert(sg14::_impl::encompasses<int, short>::value, "sg14::_impl::encompasses test failed");
static_assert(!sg14::_impl::encompasses<std::int8_t, std::uint8_t>::value, "sg14::_impl::encompasses test failed");
static_assert(!sg14::_impl::encompasses<std::uint8_t, std::int8_t>::value, "sg14::_impl::encompasses test failed");
static_assert(!sg14::_impl::encompasses<std::uint64_t, std::int64_t>::value, "sg14::_impl::encompasses test failed");
static_assert(!sg14::_impl::encompasses<std::int64_t, std::uint64_t>::value, "sg14::_impl::encompasses test failed");
static_assert(sg14::_impl::encompasses<long, unsigned short>::value, "sg14::_impl::encompasses test failed");
static_assert(!sg14::_impl::encompasses<unsigned short, long>::value, "sg14::_impl::encompasses test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::is_integer_or_float

static_assert(is_integer_or_float<saturated_integer<int64_t>>::value, "sg14::_integer_impl::is_safe_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::saturated_integer

// equality

static_assert(identical(saturated_integer<int16_t>(32767), saturated_integer<int16_t>(5000000000L)), "sg14::saturated_integer equality test failed");
static_assert(saturated_integer<uint8_t>(-1)==0, "sg14::saturated_integer equality test failed");
static_assert(saturated_integer<int64_t>(5000000000L)!=saturated_integer<uint32_t>(5000000000L),
        "sg14::saturated_integer equality test failed");
static_assert(saturated_integer<signed>(-1)==saturated_integer<unsigned>(0),
        "sg14::saturated_integer equality test failed");

// assignment

static_assert(saturated_integer<uint8_t>(-1)==0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(-.1)==0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(0)==0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1)==1, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254)==254, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254.99)==254, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(255)==255, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(256)==255, "sg14::saturated_integer test failed");

static_assert(saturated_integer<int8_t>(-129)==-128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-128)==-128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-127)==-127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(126)==126, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(127)==127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(128)==127, "sg14::saturated_integer test failed");

static_assert(saturated_integer<uint8_t>(-1e38f)==0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(0.)==0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1.f)==1, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254.)==254, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(255.f)==255, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1e50)==255, "sg14::saturated_integer test failed");

static_assert(saturated_integer<int8_t>(-1e50)==-128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-128.f)==-128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-127.)==-127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(126.f)==126, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(127.)==127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(1e38f)==127, "sg14::saturated_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::numeric_base<saturated_integer<>>

namespace {
    using namespace sg14::_impl;

    static_assert(identical(
            to_rep(saturated_integer<short>(1234)),
            short(1234)), "sg14::to_rep("
            "<>) test failed");

    static_assert(identical(
            sg14::_impl::operate(saturated_integer<short>(1234), 2., sg14::_impl::multiply_tag),
            2468.), "sg14::saturated_integer test failed");

    static_assert(operate(saturated_integer<int16_t>(32767), saturated_integer<int16_t>(5000000000L), equal_tag), "");

    static_assert(identical(
        sg14::make_safe_integer<sg14::saturated_overflow_tag>(sg14::_overflow_impl::operate
                <sg14::saturated_overflow_tag, multiply_op>()(
            sg14::safe_integer<signed char, sg14::saturated_overflow_tag>{30}.data(),
            sg14::safe_integer<signed char, sg14::saturated_overflow_tag>{40}.data())),
        sg14::safe_integer<int, sg14::saturated_overflow_tag>{1200}), "");

    static_assert(identical(
            operate_common_policy(
                    sg14::saturated_overflow,
                    multiply_tag,
                    sg14::safe_integer<signed char, sg14::saturated_overflow_tag>{30},
                    sg14::safe_integer<signed char, sg14::saturated_overflow_tag>{40}),
            sg14::safe_integer<int, sg14::saturated_overflow_tag>{1200}), "");
    
    static_assert(identical(
            operate(
                    sg14::safe_integer<signed char, sg14::saturated_overflow_tag>{30},
                    sg14::safe_integer<signed char, sg14::saturated_overflow_tag>{40},
                    multiply_tag),
            sg14::safe_integer<int, sg14::saturated_overflow_tag>{1200}), "");
}

////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert(saturated_integer<int8_t>(saturated_integer<int8_t>(100)+saturated_integer<int8_t>(100))==numeric_limits<int8_t>::max(),
        "sg14::saturated_integer test failed");

// subtraction
static_assert(saturated_integer<uint8_t>(100)-saturated_integer<int16_t>(100000)==100-numeric_limits<int16_t>::max(),
        "sg14::saturated_integer test failed");
static_assert(throwing_integer<char>{0}-throwing_integer<char>{0}==throwing_integer<char>{0}, "");

// multiplication
static_assert(identical(
        saturated_integer<int8_t>(-5)*saturated_integer<int32_t>(-14),
        saturated_integer<int>(70)), "sg14::saturated_integer test failed");

static_assert(identical(
        saturated_integer<short>(1234)*2.,
        2468.), "sg14::saturated_integer test failed");

// division
#if !defined(_MSC_VER)
static_assert(saturated_integer<uint32_t>(1)/saturated_integer<int16_t>(-10)==0U, "sg14::saturated_integer test failed");
#endif
static_assert(saturated_integer<int16_t>(77)/saturated_integer<int8_t>(-11)==-7, "sg14::saturated_integer test failed");
static_assert(int16_t(31)/saturated_integer<int8_t>(-2)==-15, "sg14::saturated_integer test failed");

static_assert(is_same<decltype(declval<saturated_integer<>>() / declval<double>()), double>::value, "");

////////////////////////////////////////////////////////////////////////////////
// traits

// std::numeric_limits<sg14::safe_integer<>>::is_integer
static_assert(numeric_limits<sg14::safe_integer<int8_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<uint8_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<int16_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<uint16_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<int32_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<uint32_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<int64_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<uint64_t, sg14::saturated_overflow_tag>>::is_integer,
        "std::numeric_limits<sg14::safe_integer<>> test failed");

// std::numeric_limits<sg14::safe_integer<>>::is_signed
static_assert(numeric_limits<sg14::safe_integer<int8_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(!numeric_limits<sg14::safe_integer<uint8_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<int16_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(!numeric_limits<sg14::safe_integer<uint16_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<int32_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(!numeric_limits<sg14::safe_integer<uint32_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(numeric_limits<sg14::safe_integer<int64_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");
static_assert(!numeric_limits<sg14::safe_integer<uint64_t, sg14::saturated_overflow_tag>>::is_signed,
        "std::numeric_limits<sg14::safe_integer<>> test failed");

// sg14::make_signed<sg14::saturated_integer<>>
static_assert(is_same<make_signed<saturated_integer<int8_t >>::type, saturated_integer<int8_t >>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<uint8_t >>::type, saturated_integer<int8_t >>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<int16_t>>::type, saturated_integer<int16_t>>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<uint16_t>>::type, saturated_integer<int16_t>>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<int32_t>>::type, saturated_integer<int32_t>>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<uint32_t>>::type, saturated_integer<int32_t>>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<int64_t>>::type, saturated_integer<int64_t>>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_signed<saturated_integer<uint64_t>>::type, saturated_integer<int64_t>>::value,
        "sg14::make_signed<sg14::saturated_integer<>> test failed");

// sg14::make_unsigned<sg14::saturated_integer<>>
static_assert(is_same<make_unsigned<saturated_integer<int8_t >>::type, saturated_integer<uint8_t >>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<uint8_t >>::type, saturated_integer<uint8_t >>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<int16_t>>::type, saturated_integer<uint16_t>>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<uint16_t>>::type, saturated_integer<uint16_t>>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<int32_t>>::type, saturated_integer<uint32_t>>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<uint32_t>>::type, saturated_integer<uint32_t>>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<int64_t>>::type, saturated_integer<uint64_t>>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");
static_assert(is_same<make_unsigned<saturated_integer<uint64_t>>::type, saturated_integer<uint64_t>>::value,
        "sg14::make_unsigned<sg14::saturated_integer<>> test failed");

////////////////////////////////////////////////////////////////////////////////
// common safe_integer characteristics

namespace {
    template<class SafeInteger>
    struct test_safe_integer {
        using safe_integer = SafeInteger;
        
        using rep = typename safe_integer::rep;
        using overflow_tag = typename safe_integer::overflow_tag;
        static_assert(is_same<sg14::safe_integer<rep, overflow_tag>, safe_integer>::value, "sg14::safe_integer test failed");

        static constexpr auto default_initialized = safe_integer{0};
        static_assert(default_initialized == 0, "sg14::safe_integer test failed");

        static_assert(+default_initialized == default_initialized, "sg14::safe_integer test failed");
#if !defined(_MSC_VER)
        static_assert(-default_initialized == default_initialized, "sg14::safe_integer test failed");
#endif
        static_assert(default_initialized+default_initialized == default_initialized, "sg14::safe_integer test failed");
        static_assert(default_initialized-default_initialized == default_initialized, "sg14::safe_integer test failed");
        static_assert(default_initialized*default_initialized == default_initialized, "sg14::safe_integer test failed");
        static_assert(default_initialized/1 == default_initialized, "sg14::safe_integer test failed");
    };
}

template struct number_test_by_rep_by_policy<safe_integer, sg14::native_overflow_tag, test_safe_integer>;
template struct number_test_by_rep_by_policy<safe_integer, sg14::saturated_overflow_tag, test_safe_integer>;
#if defined(SG14_EXCEPTIONS_ENABLED)
template struct number_test_by_rep_by_policy<safe_integer, sg14::throwing_overflow_tag, test_safe_integer>;
#endif
