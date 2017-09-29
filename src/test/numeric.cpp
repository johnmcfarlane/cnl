
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/integral_constant.h>

#include <gtest/gtest.h>

#include <cmath>

namespace {
    using namespace cnl;
    using cnl::int8;
    using cnl::uint8;
    using cnl::int64;

#if defined(__cpp_inline_variables)

    namespace test_math_constants {
        using namespace math_constants;

        template<typename T>
        void test_type() {
            auto epsilon = std::numeric_limits<T>::epsilon();
            
            EXPECT_NEAR(std::exp(T{1}), e<T>, epsilon);
            EXPECT_EQ(std::log2(e<T>), log2e<T>);
            EXPECT_NEAR(std::log10(e<T>), log10e<T>, epsilon);
            EXPECT_NEAR(std::atan(T{1}) * T{4}, pi<T>, epsilon);
            EXPECT_NEAR(T{1} / pi<T>, invpi<T>, epsilon);
            EXPECT_NEAR(T{1} / std::sqrt(pi<T>), invsqrtpi<T>, epsilon);
            EXPECT_NEAR(std::log(T{2}), ln2<T>, epsilon);
            EXPECT_NEAR(std::log(T{10}), ln10<T>, epsilon);
            EXPECT_NEAR(std::sqrt(T{2}), sqrt2<T>, epsilon);
            EXPECT_NEAR(std::sqrt(T{3}), sqrt3<T>, epsilon);
            EXPECT_NEAR(T{1} / sqrt2<T>, invsqrt2<T>, epsilon);
            EXPECT_NEAR(T{1} / sqrt3<T>, invsqrt3<T>, epsilon);
            EXPECT_NEAR(180 / pi<T>, radian<T>, epsilon * 100);
            EXPECT_NEAR(0.5772156649015328606065120900824024L, egamma<T>, epsilon);
            EXPECT_NEAR(1.6180339887498948482045868343656381L, phi<T>, epsilon);
            EXPECT_NEAR(0.915965594177219015054603514932384110774L, catalan<T>, epsilon);
            EXPECT_NEAR(1.202056903159594285399738161511449990L, apery<T>, epsilon);
            EXPECT_NEAR(1.282427129100622636875342568869791727L, glaisher<T>, epsilon);
        }

        TEST(numeric, float_constants) {
            test_type<float>();
        }

        TEST(numeric, double_constants) {
            test_type<double>();
        }

        TEST(numeric, long_double_constants) {
            test_type<long double>();
        }
    }

#endif  // defined(__cpp_inline_variables)

    namespace test_numeric_impl {
        using namespace _numeric_impl;

        namespace test_used_bits_positive {
            static_assert(used_bits_positive(1)==1,
                    "cnl::_numeric_impl::used_bits_positive test failed");
            static_assert(used_bits_positive(2)==2,
                    "cnl::_numeric_impl::used_bits_positive test failed");
            static_assert(used_bits_positive(uint8_t{255})==8,
                    "cnl::_numeric_impl::used_bits_positive test failed");
            static_assert(used_bits_positive(int16_t{32767})==15,
                    "cnl::_numeric_impl::used_bits_positive test failed");
            static_assert(used_bits_positive(numeric_limits<int64_t>::max())==63,
                    "cnl::_numeric_impl::used_bits_positive test failed");
            static_assert(used_bits_positive(numeric_limits<uint64_t>::max())==64,
                    "cnl::_numeric_impl::used_bits_positive test failed");
        }

        namespace test_trailing_bits_positive {
            static_assert(trailing_bits_positive(1)==0,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(2)==1,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(3)==0,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(4)==2,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(5)==0,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(7)==0,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(8)==3,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(9)==0,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(96)==5,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(1023)==0,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
            static_assert(trailing_bits_positive(1024)==10,
                    "cnl::_numeric_impl::trailing_bits_positive test failed");
        }
    }

    namespace test_used_bits_symmetric {
        static_assert(_impl::used_bits_symmetric(0)==0, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(1)==1, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(2)==2, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(uint8_t{255})==8, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int16_t{32767})==15, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-1})==1, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-2})==2, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-3})==2, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-4})==3, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-5})==3, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-8})==4, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-9})==4, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(int8_t{-128})==1, "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(numeric_limits<int64_t>::lowest()+1)==63,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(numeric_limits<int64_t>::min()+1)==63,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(numeric_limits<int64_t>::max())==63,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(numeric_limits<uint64_t>::min())==0,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(numeric_limits<uint64_t>::max())==64,
                "cnl::_impl::used_bits_symmetric test failed");
    }

    namespace test_used_bits {
        static_assert(used_bits(0)==0, "cnl::used_bits test failed");
        static_assert(used_bits(1)==1, "cnl::used_bits test failed");
        static_assert(used_bits(2)==2, "cnl::used_bits test failed");
        static_assert(used_bits(uint8_t{255})==8, "cnl::used_bits test failed");
        static_assert(used_bits(int16_t{32767})==15, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-1})==0, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-2})==1, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-3})==2, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-4})==2, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-5})==3, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-8})==3, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-9})==4, "cnl::used_bits test failed");
        static_assert(used_bits(int8_t{-128})==7, "cnl::used_bits test failed");
        static_assert(used_bits(numeric_limits<int64_t>::lowest())==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(numeric_limits<int64_t>::lowest()+1)==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(numeric_limits<int64_t>::min()+1)==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(numeric_limits<int64_t>::max())==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(UINT64_C(0))==0,
                "cnl::used_bits test failed");
        static_assert(used_bits(numeric_limits<uint64_t>::min())==0,
                "cnl::used_bits test failed");
        static_assert(used_bits(numeric_limits<uint64_t>::max())==64,
                "cnl::used_bits test failed");
        static_assert(cnl::used_bits(INT64_C(0x7fffffff00000000)) == 63,
                "cnl::used_bits test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::used_bits(CNL_INTMAX_C(0x7fffffff000000000000000000000000)) == 127,
                "cnl::used_bits test failed");
#endif
    }

    namespace test_trailing_bits {
        static_assert(trailing_bits(1)==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(2)==1, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(uint8_t{255})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(uint16_t{256})==8, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int16_t{32767})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int32_t{32768})==15, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-1})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-2})==1, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-3})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-4})==2, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-5})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-8})==3, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-9})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-128})==7, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(numeric_limits<int64_t>::lowest()+1)==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(numeric_limits<int64_t>::min()+1)==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(numeric_limits<int64_t>::max())==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(numeric_limits<uint64_t>::min())==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(numeric_limits<uint64_t>::max())==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(numeric_limits<uint64_t>::max())==0,
                "cnl::trailing_bits test failed");
    }

    namespace test_leading_bits {
        static_assert(leading_bits(int8_t{127})==0, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{64})==0, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{63})==1, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{4})==4, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{3})==5, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{2})==5, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{1})==6, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{0})==7, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{-1})==7, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{-2})==6, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{-16})==3, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{-64})==1, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{-65})==0, "cnl::leading_bits");
        static_assert(leading_bits(int8_t{-128})==0, "cnl::leading_bits");

        static_assert(leading_bits(UINT32_C(0)) == 32, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(127)) == 25, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(64)) == 25, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(63)) == 26, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(4)) == 29, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(3)) == 30, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(2)) == 30, "cnl::leading_bits");
        static_assert(leading_bits(UINT32_C(1)) == 31, "cnl::leading_bits");

        static_assert(leading_bits(cnl::numeric_limits<int64>::max()) == 0, "cnl::leading_bits");

        static_assert(_impl::identical(cnl::leading_bits(INT64_C(1)), 62), "cnl::leading_bits");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::leading_bits(CNL_INTMAX_C(0x7fffffff000000000000000000000000)) == 0, "cnl::leading_bits");
        static_assert(_impl::identical(cnl::leading_bits(CNL_INTMAX_C(0x0)), 127), "cnl::leading_bits");
#endif

        TEST(numeric, leading_bits) {
            EXPECT_EQ(leading_bits(INT32_C(0)), 31);
            EXPECT_EQ(leading_bits(INT32_C(127)), 24);
            EXPECT_EQ(leading_bits(INT32_C(64)), 24);
            EXPECT_EQ(leading_bits(INT32_C(63)), 25);
            EXPECT_EQ(leading_bits(INT32_C(4)), 28);
            EXPECT_EQ(leading_bits(INT32_C(3)), 29);
            EXPECT_EQ(leading_bits(INT32_C(2)), 29);
            EXPECT_EQ(leading_bits(INT32_C(1)), 30);
            EXPECT_EQ(leading_bits(INT32_C(-1)), 31);
            EXPECT_EQ(leading_bits(INT32_C(-2)), 30);
            EXPECT_EQ(leading_bits(INT32_C(-3)), 29);
            EXPECT_EQ(leading_bits(INT32_C(-4)), 29);
            EXPECT_EQ(leading_bits(INT32_C(-5)), 28);
            EXPECT_EQ(leading_bits(INT32_C(-129)), 23);
            EXPECT_EQ(leading_bits(INT32_C(-128)), 24);
            EXPECT_EQ(leading_bits(INT32_C(-127)), 24);
            EXPECT_EQ(leading_bits(INT32_C(-65)), 24);
            EXPECT_EQ(leading_bits(INT32_C(-64)), 25);
        }
    }
}
