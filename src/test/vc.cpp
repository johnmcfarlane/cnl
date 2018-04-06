#define Vc_EXPERIMENTAL 1
#include <Vc/simd>
#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

// necessary plumbing to integrate Vc::simd with cnl::fixed_point
namespace std {
    template<typename T, class Abi>
    struct numeric_limits<Vc::simd<T, Abi>> : numeric_limits<T> {
    };
}

namespace cnl {
    template<typename T, class Abi, class Value>
    struct from_value<Vc::simd<T, Abi>, Value> {
        using type = Vc::simd<Value, Abi>;
    };

    template<typename ToT, class FromT, class Abi>
    struct from_value<Vc::simd<ToT, Abi>, Vc::simd<FromT, Abi>> {
        using type = Vc::simd<ToT, Abi>;
    };

    template<int Digits, typename T, class Abi>
    struct shift<Digits, 2, Vc::simd<T, Abi>> : cnl::_impl::default_shift<Digits, 2, Vc::simd<T, Abi>> {
    };
}

namespace {
    ////////////////////////////////////////////////////////////////////////////////
    // test helper

    template<class Expected, class Actual>
    constexpr bool simd_identical(Expected const &e, Actual const &a) {
        static_assert(std::is_same<Expected, Actual>::value, "expected and actual types are not the same");
        auto equality_result = e == a;
        return Vc::all_of(equality_result);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Vc::simd tests

    TEST(vc, from_value_of_i) {
        auto i = 123;
        auto a = cnl::_impl::from_value<Vc::simd<std::uint16_t>>(i);
        EXPECT_TRUE(simd_identical(Vc::simd<int>{123}, a));
    }

    TEST(vc, from_value_of_simd_of_i) {
        auto s = Vc::simd<int>{123};
        auto a = cnl::_impl::from_value<Vc::simd<int>>(s);
        EXPECT_TRUE(simd_identical(Vc::simd<int>{123}, a));
    }

    TEST(vc, bitwise_shift_by_unsigned) {
        auto a = Vc::simd<int>(123) << 1u;
        EXPECT_TRUE(simd_identical(Vc::simd<int>{246}, a));
    }

    TEST(vc, bitwise_shift_by_constant) {
        auto a = Vc::simd<int>(123) << cnl::constant<1>{};
        EXPECT_TRUE(simd_identical(Vc::simd<int>{246}, a));
    }

    TEST(vc, named_shift) {
        auto a = cnl::shift<1, 2, Vc::simd<int>>()(Vc::simd<int>(123));
        EXPECT_TRUE(simd_identical(Vc::simd<int>{246}, a));
    }

    TEST(vc, equal) {
        auto a = Vc::simd<std::uint16_t>(Vc::simd<std::uint16_t>{123});
        EXPECT_TRUE(simd_identical(Vc::simd<std::uint16_t>{123}, a));
    }

    template<typename T> using simd = Vc::simd<T, Vc::simd_abi::fixed_size<2>>;
    TEST(vc, fixed_size) {
        using simd_float = simd<float>;
        using simd_uint32_t = simd<std::uint32_t>;
        auto f = simd_float{std::array<float, 2>{1.5, 16768.0000152587890625}};
        auto i = simd_uint32_t(f);
    }
//
//    ////////////////////////////////////////////////////////////////////////////////
//    // cnl::fixed_point<Vc::simd> tests
//
//    // fixed-point SIMD type
//    template<typename T = int, int Exponent = 0, class Abi = typename Vc::simd<T>::abi_type>
//    using fp_simd = cnl::fixed_point<Vc::simd<T, Abi>, Exponent>;
//
//    TEST(fp_vc, equal) {
//        auto a = fp_simd<std::uint16_t>(static_cast<std::uint16_t>(123));
//        auto b = fp_simd<std::uint16_t>(static_cast<std::uint16_t>(123));
//        EXPECT_TRUE(simd_identical(b, a));
//    }
//
//    TEST(fp_vc, not_equal) {
//        auto a = fp_simd<std::uint16_t>(static_cast<std::uint16_t>(123));
//        auto b = fp_simd<std::uint16_t>(static_cast<std::uint16_t>(124));
//        EXPECT_TRUE(Vc::all_of(b != a));
//    }
//
//    TEST(fp_vc, add) {
//        std::int16_t a = 1234, b = 8765;
//        auto expected = fp_simd<std::int16_t>(static_cast<std::int16_t>(a*b));
//        auto actual = fp_simd<std::int16_t>(a)*fp_simd<std::int16_t>(b);
//        EXPECT_TRUE(simd_identical(expected, actual));
//    }
//
//    TEST(fp_vc, multiply) {
//        std::int16_t a = 1234, b = 8765;
//        auto expected = fp_simd<std::int16_t>(static_cast<std::int16_t>(a*b));
//        auto actual = fp_simd<std::int16_t>(a)*fp_simd<std::int16_t>(b);
//        EXPECT_TRUE(simd_identical(expected, actual));
//    }
//
//    TEST(fp_vc, constant_shift) {
//        using namespace cnl::literals;
//        std::int64_t a = 1234;
//        auto expected = fp_simd<std::int64_t, 2>(a<<1);
//        auto actual = fp_simd<std::int64_t, 1>(a) << 1_c;
//        EXPECT_TRUE(simd_identical(expected, actual));
//    }
//
//    TEST(fp_vc, fixed_size) {
//        auto a = fp_simd<std::uint32_t, -16, Vc::simd_abi::fixed_size<2>>{{1.5, 16768.0000152587890625}};
//    }
}
