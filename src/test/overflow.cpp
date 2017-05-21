
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// disable warnings which warn against errors for whose avoidance overflow is designed
#if defined(_MSC_VER)
#pragma warning(disable: 4018)
#elif defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include <sg14/auxiliary/overflow.h>

// TODO: remove ASAP
#if defined(_MSC_VER)
#pragma warning(disable: 4307)
#endif

namespace {
    using sg14::_impl::identical;

    using sg14::convert;
    using sg14::add;
    using sg14::subtract;
    using sg14::multiply;

    namespace test_impl {
        using namespace sg14::_overflow_impl;
        
        ////////////////////////////////////////////////////////////////////////////////
        // positive_digits

        static_assert(positive_digits<int8_t>::value==7,
                "positive_digits test failed");
        static_assert(positive_digits<uint8_t>::value==8,
                "positive_digits test failed");
        static_assert(positive_digits<int16_t>::value==15,
                "positive_digits test failed");
        static_assert(positive_digits<uint16_t>::value==16,
                "positive_digits test failed");
        static_assert(positive_digits<int32_t>::value==31,
                "positive_digits test failed");
        static_assert(positive_digits<uint32_t>::value==32,
                "positive_digits test failed");
        static_assert(positive_digits<int64_t>::value==63,
                "positive_digits test failed");
        static_assert(positive_digits<uint64_t>::value==64,
                "positive_digits test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // negative_digits

        static_assert(negative_digits<int8_t>::value==7,
                "negative_digits test failed");
        static_assert(negative_digits<uint8_t>::value==0,
                "negative_digits test failed");
        static_assert(negative_digits<int16_t>::value==15,
                "negative_digits test failed");
        static_assert(negative_digits<uint16_t>::value==0,
                "negative_digits test failed");
        static_assert(negative_digits<int32_t>::value==31,
                "negative_digits test failed");
        static_assert(negative_digits<uint32_t>::value==0,
                "negative_digits test failed");
        static_assert(negative_digits<int64_t>::value==63,
                "negative_digits test failed");
        static_assert(negative_digits<uint64_t>::value==0,
                "negative_digits test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // is_positive_overflow

        static_assert(!is_positive_overflow<uint8_t, int8_t>(-1),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int8_t>(127),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, uint16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, uint16_t>(255),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<uint8_t, uint16_t>(256),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(-1),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(255),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<uint8_t, int16_t>(256),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(-1),
                "is_positive_overflow test failed");

        static_assert(!is_positive_overflow<int8_t, uint8_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, uint8_t>(127),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<int8_t, uint8_t>(128),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, uint16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, uint16_t>(127),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<int8_t, uint16_t>(128),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, int16_t>(-1),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, int16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, int16_t>(127),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<int8_t, int16_t>(128),
                "is_positive_overflow test failed");

        static_assert(!is_positive_overflow<uint16_t, uint8_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, uint8_t>(255),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int8_t>(-128),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int8_t>(127),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int16_t>(-32768),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int16_t>(32767),
                "is_positive_overflow test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // is_negative_overflow

        static_assert(is_negative_overflow<uint8_t, int8_t>(-1),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int8_t>(127),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, uint16_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, uint16_t>(255),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, uint16_t>(256),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint8_t, int16_t>(-1),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int16_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int16_t>(255),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int16_t>(256),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint8_t, int16_t>(-1),
                "sg14::_integer_impl::is_negative_overflow test failed");

        static_assert(!is_negative_overflow<int8_t, uint8_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint8_t>(127),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint8_t>(128),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint16_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint16_t>(127),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint16_t>(128),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<int8_t, int16_t>(-129),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, int16_t>(-128),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, int16_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");

        static_assert(!is_negative_overflow<uint16_t, uint8_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, uint8_t>(255),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint16_t, int8_t>(-1),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, int8_t>(0),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, int8_t>(127),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint16_t, int16_t>(-32768),
                "sg14::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, int16_t>(32767),
                "sg14::_integer_impl::is_negative_overflow test failed");

        static_assert(sg14::_overflow_impl::is_multiply_overflow(2000000000, 2), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(21, 2), "safe_elastic_integer operator*");
        static_assert(sg14::_overflow_impl::is_multiply_overflow(-2000000000, 2), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(-21, 2), "safe_elastic_integer operator*");
        static_assert(sg14::_overflow_impl::is_multiply_overflow(2000000000, -2), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(21, -2), "safe_elastic_integer operator*");
        static_assert(sg14::_overflow_impl::is_multiply_overflow(-2000000000, -2), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(-21, -2), "safe_elastic_integer operator*");

        static_assert(sg14::_overflow_impl::is_multiply_overflow(2, 2000000000), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(2, 21), "safe_elastic_integer operator*");
        static_assert(sg14::_overflow_impl::is_multiply_overflow(2, -2000000000), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(2, -21), "safe_elastic_integer operator*");
        static_assert(sg14::_overflow_impl::is_multiply_overflow(-2, 2000000000), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(-2, 21), "safe_elastic_integer operator*");
        static_assert(sg14::_overflow_impl::is_multiply_overflow(-2, -2000000000), "safe_elastic_integer operator*");
        static_assert(!sg14::_overflow_impl::is_multiply_overflow(-2, -21), "safe_elastic_integer operator*");
    }

    namespace test_native_overflow {
        using sg14::native_overflow;

        // convert
        static_assert(identical(convert<std::uint8_t>(native_overflow, 259), std::uint8_t{3}), "sg14::convert test failed");
        static_assert(identical(convert<std::uint16_t>(native_overflow, -123), std::uint16_t{65413}), "sg14::convert test failed");
        static_assert(identical(convert<std::int32_t>(native_overflow, std::uint8_t{55}), 55), "sg14::convert test failed");

        // add
        static_assert(identical(add(native_overflow, UINT32_C(0xFFFFFFFF), UINT32_C(0x12345678)), UINT32_C(0xFFFFFFFF)+UINT32_C(0x12345678)), "sg14::add test failed");

        // subtract
        static_assert(identical(sg14::_overflow_impl::operate<sg14::native_overflow_tag, sg14::_impl::subtract_tag_t>()(INT8_C(0), INT8_C(0)), 0), "sg14::subtract test failed");
        static_assert(identical(subtract(native_overflow, INT8_C(0), INT8_C(0)), 0), "sg14::subtract test failed");

        // multiply
        static_assert(identical(multiply(native_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "sg14::multiply test failed");

        // compare
        static_assert(sg14::_overflow_impl::operate<sg14::native_overflow_tag, sg14::_impl::less_than_tag_t>()(-1, 1u) == (-1 < 1u), "sg14::_overflow_impl::operate test failed");
    }

    namespace test_throwing_overflow {
        using sg14::throwing_overflow;

        // subtract
        static_assert(identical(subtract(throwing_overflow, INT8_C(0), INT8_C(0)), 0), "sg14::add test failed");

        // multiply
        static_assert(identical(multiply(throwing_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "sg14::add test failed");
    }

    namespace test_saturated {
        using sg14::saturated_overflow;

        // convert
        static_assert(identical(convert<std::uint8_t>(saturated_overflow, 259), std::uint8_t{255}), "sg14::convert test failed");
        static_assert(identical(convert<std::uint16_t>(saturated_overflow, -123), std::uint16_t{0}), "sg14::convert test failed");
        static_assert(identical(convert<std::int32_t>(saturated_overflow, std::uint8_t{55}), 55), "sg14::convert test failed");

        // subtract
        static_assert(identical(subtract(saturated_overflow, INT8_C(0), INT8_C(0)), 0), "sg14::add test failed");

        // multiply
        static_assert(identical(multiply(saturated_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "sg14::add test failed");

        // compare
        static_assert(sg14::_overflow_impl::operate<sg14::saturated_overflow_tag, sg14::_impl::less_than_tag_t>()(-1, 1u), "sg14::_overflow_impl::operate test failed");
    }
}
