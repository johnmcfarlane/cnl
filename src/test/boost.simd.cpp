
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(SG14_BOOST_SIMD_ENABLED)

#include <sg14/auxiliary/boost.simd.h>

#include <sg14/fixed_point>

#include <boost/simd/function/all.hpp>
#include <boost/simd/function/compare_equal.hpp>

#include <gtest/gtest.h>

namespace {
    using sg14::fixed_point;
    using boost::simd::pack;

    template<class T, std::size_t N, int Exponent>
    using fpp = fixed_point<pack<T, N>, Exponent>;

    template<class U, class FPP>
    struct rebind_s;

    template<class U, class T, std::size_t N, int Exponent>
    struct rebind_s<U, fpp<T, N, Exponent>> {
        using type = pack<U, N>;
    };

    template<class U, class FPP>
    using rebind = typename rebind_s<U, FPP>::type;

    template<class FPP>
    using initializer = rebind<float, FPP>;
}

namespace sg14 {
    template<class T, std::size_t N, int Exponent>
    bool operator==(const fpp<T, N, Exponent> &lhs, const fpp<T, N, Exponent> &rhs) noexcept {
        return boost::simd::compare_equal(lhs.data(), rhs.data());
    }

    template<class T, std::size_t N, int Exponent>
    bool operator!=(const fpp<T, N, Exponent> &lhs, const fpp<T, N, Exponent> &rhs) noexcept {
        return !(lhs==rhs);
    }
}

namespace {
    namespace test_numeric_limits {
        static_assert(std::numeric_limits<pack<int>>::is_integer, "");
        static_assert(!std::numeric_limits<pack<int>>::is_iec559, "");

        static_assert(!std::numeric_limits<pack < float>>::is_integer, "");
        static_assert(std::numeric_limits<pack < float>>::is_iec559, "");
    }

    namespace test_set_digits {
        using actual_type = sg14::set_digits_t<boost::simd::pack<signed, 8>, 15>;
        using expected_type = boost::simd::pack<std::int16_t, 8>;
        static_assert(std::is_same<actual_type, expected_type>::value, "");
    }

    TEST(boost_simd, scale) {
        using pack = boost::simd::pack<int, 2>;
        auto input = pack{65535, 0};
        auto output = sg14::scale<pack>()(input, 2, 5);
        auto expected = pack{65535*32, 0};
        ASSERT_TRUE(boost::simd::compare_equal(expected, output));
    }

    TEST(boost_simd, shift_left) {
        using output_type = boost::simd::pack<std::int64_t, 2>;
        using input_type = boost::simd::pack<std::uint16_t, 2>;
        auto input = input_type{65535, 0};
        auto output = sg14::_impl::shift_left<5, output_type>(input);
        auto expected = output_type{65535*32, 0};
        ASSERT_TRUE(boost::simd::compare_equal(expected, output));
    }

    TEST(boost_simd, equality) {
        using fpp = fpp<short, 4, -4>;
        using init = initializer<fpp>;
        auto lhs = fpp(init(6.25, -8., 0., 1.625));
        auto rhs = fpp(init(6.25, -8., 0., 1.625));
        ASSERT_EQ(lhs, rhs);
    }

    TEST(boost_simd, inequality) {
        using fpp = fpp<std::uint64_t, 2, -32>;
        using init = initializer<fpp>;
        auto lhs = fpp{init{8237416.17618, 3278928.9012393}};
        auto rhs = fpp{init{8237416.17618, 4278928.9012393}};
        ASSERT_NE(lhs, rhs);
    }

    TEST(boost_simd, conversion) {
        using wide_type = pack<std::int64_t, 2>;
        using narrow_type = pack<std::uint16_t, 2>;
        auto w = wide_type{pack<std::int64_t, 2>{INT64_C(5000000000), INT64_C(-42000000000000)}};
        auto n = narrow_type{pack<std::uint16_t, 2>{INT16_C(32000), INT64_C(-7)}};

        w = static_cast<wide_type>(n);    // requires new explicit operator
        n = static_cast<narrow_type>(w);    // requires new explicit conversion operator
        //w = n;    // doesn't compile with or without explicit conversion operator
        //n = w;    // doesn't compile with or without explicit conversion operator

        auto narrow_implicit(w);    // happens already
        auto wide_implicit(n);  // happens already
        auto narrow_explicit{w};    // happens already
        auto wide_explicit{n};  // happens already
    }

    TEST(boost_simd, multiply) {
        using operand_type = fpp<short, 2, -4>;
        using result_type = fpp<short, 2, -8>;
        using initializer = initializer<operand_type>;

        auto expected = result_type{initializer{-7.9375, -1.}};
        auto multiplier = operand_type{initializer{7.9375, -8.}};
        auto multiplicand = operand_type{initializer{-1, .125}};
        auto product = multiplier * multiplicand;
        ASSERT_EQ(expected, product);
    }

    TEST(boost_simd, add) {
        using operand_type = fpp<int, 4, -16>;
        using result_type = operand_type;
        using initializer = initializer<operand_type>;

        auto expected = result_type{initializer{7.9375+-1, -8.+.125, 0+-5, 3.5+-3.5}};
        auto augend = operand_type{initializer{7.9375, -8., 0, 3.5}};
        auto addend = operand_type{initializer{-1, .125, -5, -3.5}};
        auto sum = augend + addend;
        ASSERT_EQ(expected, sum);
    }
}

#endif
