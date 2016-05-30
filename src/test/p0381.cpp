#include <sg14/type_traits.h>

#include <gtest/gtest.h>

#include <boost/integer.hpp>

static_assert(__cplusplus>=201402L, "C++14 features required to compile this unit");

using std::declval;
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

    TEST(p0381, multiply_uint8_ok)
    {
        ASSERT_EQ(100, multiply(10, 10));
    }

    TEST(p0381, multiply_uint8_overflow)
    {
        ASSERT_NE(400, multiply(20, 20));
    }
}

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

    TEST(p0381, multiply_unsigned_ok)
    {
        ASSERT_EQ(400u, multiply(20u, 20u));
    }

    TEST(p0381, multiply_unsigned_overflow)
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

    TEST(p0381, multiply_unsigned_ok)
    {
        ASSERT_EQ(400u, multiply(20u, 20u));
    }

    TEST(p0381, multiply_unsigned_still_ok)
    {
        ASSERT_EQ(static_cast<uint64_t>(UINT_MAX)*static_cast<uint64_t>(UINT_MAX),
                static_cast<uint64_t>(multiply(UINT_MAX, UINT_MAX)));
    }
}

namespace sample4 {
    // Sample 4 intentionally does not exist. If it did, there would be no need for P0381!
}

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
    static_assert(is_same<uint64_t, decltype(multiply(declval<uint32_t>(), declval<uint32_t>()))>::value,
            "incorrect assumption about result of multiply function");
    static_assert(is_same<uint32_t, decltype(multiply(declval<uint16_t>(), declval<uint16_t>()))>::value,
            "incorrect assumption about result of multiply function");

    // but don't do so well with signed types
//    static_assert(is_same<int64_t, decltype(multiply(declval<int32_t>(), declval<int32_t>()))>::value,
//            "incorrect assumption about result of multiply function");
//    static_assert(is_same<int32_t, decltype(multiply(declval<int16_t>(), declval<int16_t>()))>::value,
//            "incorrect assumption about result of multiply function");

    TEST(p0381, multiply_unsigned_ok)
    {
        ASSERT_EQ(400u, multiply(20u, 20u));
    }

    TEST(p0381, multiply_unsigned_still_ok)
    {
        ASSERT_EQ(static_cast<uint64_t>(UINT_MAX)*static_cast<uint64_t>(UINT_MAX),
                static_cast<uint64_t>(multiply(UINT_MAX, UINT_MAX)));
    }
}

namespace determining {
    using sg14::width_v;

    static_assert(width_v<uint16_t> == 16, "the width of uint16_t is exactly 16 bits");
    static_assert(width_v<long long> >= 64, "long long has a width of at least 64 bits");
    static_assert(width_v<long> >= width_v<short>, "short is not longer than long");
    static_assert(width_v<wchar_t> >= width_v<char>, "a wide character is at least wide as a character");
}

namespace specifiying {
    using sg14::set_width_t;
    static_assert(is_same<set_width_t<signed, 8>, int8_t>::value, "int8_t is a signed 8-bit integer");
    static_assert(is_same<set_width_t<unsigned, 32>, uint32_t>::value, "uint32_t is an unsigned 32-bit integer");
    static_assert(is_same<set_width_t<uint64_t, 16>, uint16_t>::value, "a 64-bit unsigned integer was narrowed to 16-bits");
    static_assert(is_same<set_width_t<char, 64>, int64_t>::value || is_same<set_width_t<char, 64>, uint64_t>::value, "char may or may not be signed so the result may be uint64_t or int64_t");
    static_assert(is_same<set_width_t<int, 10>, int16_t>::value, "result must be at least 10 bits wide");
}
