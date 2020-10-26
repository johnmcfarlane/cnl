
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/num_traits.h>

#include <gtest/gtest.h>

#if defined(CNL_BOOST_ENABLED)
#include <boost/integer.hpp>
#endif

using std::is_same;

namespace sample1 {
    // range of a*b is UCHAR_MAX*UCHAR_MAX but range of return value is UCHAR_MAX
    uint8_t multiply(uint8_t a, uint8_t b)
    {
        return a*b;
    }

    // sample 1 tests
    using single_width = uint8_t;

    static_assert(UCHAR_MAX==255, "incorrect assumption about value of UCHAR_MAX");
    static_assert(UCHAR_MAX*UCHAR_MAX==65025, "incorrect assumption about value of UCHAR_MAX");

    TEST(p0381, multiply_uint8_ok)  // NOLINT
    {
        ASSERT_EQ(100, multiply(10, 10));
    }

    TEST(p0381, multiply_uint8_overflow)  // NOLINT
    {
        ASSERT_NE(400, multiply(20, 20));
    }
}

using std::declval;

namespace sample2 {
    // range of a*b is UINT_MAX*UINT_MAX but range of return value is UINT_MAX
    auto multiply(unsigned a, unsigned b)
    {
        return a*b;
    }

    // sample 2 tests
    using wide_type = unsigned long long;

    static_assert(sizeof(wide_type)>=sizeof(unsigned)*2,
            "the following tests assume unsigned long long is twice the size of unsigned");
    static_assert(is_same<decltype(declval<unsigned>()*declval<unsigned>()), unsigned>::value,
            "incorrect assumption about type of result of unsigned * unsigned");

    TEST(p0381, multiply_unsigned_ok)  // NOLINT
    {
        ASSERT_EQ(400U, multiply(20U, 20U));
    }

    TEST(p0381, multiply_unsigned_overflow)  // NOLINT
    {
        ASSERT_NE(static_cast<wide_type>(UINT_MAX)*static_cast<wide_type>(UINT_MAX),
                static_cast<wide_type>(multiply(UINT_MAX, UINT_MAX)));
    }
}

namespace sample3 {
    auto multiply(uint32_t a, uint32_t b)
    {
        using result_type = uint64_t;
        return result_type{a}*result_type{b};
    }

    // sample 3 tests
    static_assert(is_same<uint64_t, decltype(multiply(declval<uint32_t>(), declval<uint32_t>()))>::value,
            "incorrect assumption about result of multiply function");

    TEST(p0381, multiply_unsigned_ok)  // NOLINT
    {
        ASSERT_EQ(400U, multiply(20U, 20U));
    }

    TEST(p0381, multiply_unsigned_still_ok)  // NOLINT
    {
        ASSERT_EQ(static_cast<uint64_t>(UINT_MAX)*static_cast<uint64_t>(UINT_MAX),
                static_cast<uint64_t>(multiply(UINT_MAX, UINT_MAX)));
    }
}

namespace sample4 {
    // Sample 4 intentionally does not exist. If it did, there would be no need for P0381!
}

#if defined(CNL_BOOST_ENABLED)
namespace sample5 {
    template<class Operand>
    auto multiply(Operand a, Operand b)
    {
        constexpr auto operand_width = sizeof(Operand)*CHAR_BIT*2;
        using result_type = typename boost::uint_t<operand_width>::fast;
        return result_type{a}*result_type{b};
    }

    // sample 5 tests are a lot like sample 3 tests

    // they are more generic
#if ! defined(__APPLE__)    // uint64_t is a different type depending on the version of XCode
    static_assert(is_same<uint64_t, decltype(multiply(declval<uint32_t>(), declval<uint32_t>()))>::value,
            "incorrect assumption about result of multiply function");
#endif
    static_assert(is_same<uint32_t, decltype(multiply(declval<uint16_t>(), declval<uint16_t>()))>::value,
            "incorrect assumption about result of multiply function");

    // but don't do so well with signed types
//    static_assert(is_same<int64_t, decltype(multiply(declval<int32_t>(), declval<int32_t>()))>::value,
//            "incorrect assumption about result of multiply function");
//    static_assert(is_same<int32_t, decltype(multiply(declval<int16_t>(), declval<int16_t>()))>::value,
//            "incorrect assumption about result of multiply function");

    TEST(p0381, multiply_unsigned_ok)  // NOLINT
    {
        ASSERT_EQ(400U, multiply(20U, 20U));
    }

    TEST(p0381, multiply_unsigned_still_ok)  // NOLINT
    {
        ASSERT_EQ(static_cast<uint64_t>(UINT_MAX)*static_cast<uint64_t>(UINT_MAX),
                static_cast<uint64_t>(multiply(UINT_MAX, UINT_MAX)));
    }
}
#endif  // defined(CNL_BOOST_ENABLED)

namespace determining {
    using cnl::digits;

    static_assert(digits<uint16_t>::value == 16, "the digits of uint16_t is exactly 16 bits");
    static_assert(digits<long long>::value >= 63, "long long has a digits of at least 64 bits");
    static_assert(digits<long>::value >= digits<short>::value, "short is no longer than long");
    // NOLINTNEXTLINE(misc-redundant-expression)
    static_assert(digits<wchar_t>::value >= digits<char>::value, "a wide character is at least as wide as a character");
}

namespace specifiying {
    using cnl::set_digits_t;
    using cnl::digits;

    static_assert(is_same<set_digits_t<signed, 7>, int8_t>::value, "int8_t is a signed 8-bit integer");
    static_assert(is_same<set_digits_t<unsigned, 32>, uint32_t>::value, "uint32_t is an unsigned 32-bit integer");
    static_assert(is_same<set_digits_t<uint64_t, 16>, uint16_t>::value, "a 64-bit unsigned integer was narrowed to 16-bits");
    static_assert(is_same<set_digits_t<char, 63>, int64_t>::value || is_same<set_digits_t<char, 63>, uint64_t>::value, "char may or may not be signed so the result may be uint64_t or int64_t");
    static_assert(digits<set_digits_t<int, 10>>::value >= 10, "result must be at least 10 bits wide");
}
