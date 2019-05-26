
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/numeric.h>
#include <cnl/constant.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

#include <cmath>

namespace {
    using namespace cnl;
    using cnl::int8;
    using cnl::uint8;
    using cnl::int64;

    namespace test_used_digits_signed {
        static_assert(_impl::used_digits_signed<false>{}(1, 2)==1,
                "cnl::_impl::used_digits_signed<false> test failed");
        static_assert(_impl::used_digits_signed<false>{}(2, 2)==2,
                "cnl::_impl::used_digits_signed<false> test failed");
        static_assert(_impl::used_digits_signed<false>{}(uint8_t{255}, 2)==8,
                "cnl::_impl::used_digits_signed<false> test failed");
        static_assert(_impl::used_digits_signed<false>{}(int16_t{32767}, 2)==15,
                "cnl::_impl::used_digits_signed<false> test failed");
        static_assert(_impl::used_digits_signed<false>{}(numeric_limits<int64_t>::max(), 2)==63,
                "cnl::_impl::used_digits_signed<false> test failed");
        static_assert(_impl::used_digits_signed<false>{}(numeric_limits<uint64_t>::max(), 2)==64,
                "cnl::_impl::used_digits_signed<false> test failed");
    }

    namespace test_used_digits_2 {
        static_assert(cnl::_impl::used_digits(0)==0, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(1)==1, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(2)==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(uint8_t{255})==8, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int16_t{32767})==15, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-1})==0, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-2})==1, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-3})==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-4})==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-5})==3, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-8})==3, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-9})==4, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-128})==7, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::lowest())==63,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::lowest()+1)==63,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::min()+1)==63,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::max())==63,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(UINT64_C(0))==0,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<uint64_t>::min())==0,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<uint64_t>::max())==64,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(INT64_C(0x7fffffff00000000))==63,
                "cnl::_impl::used_digits test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::_impl::used_digits(CNL_INTMAX_C(0x7fffffff000000000000000000000000))==127,
                "cnl::_impl::used_digits test failed");
#endif
    }

    namespace test_used_digits_10 {
        static_assert(cnl::_impl::used_digits(0, 10)==0, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(1, 10)==1, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(9, 10)==1, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(10, 10)==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(99, 10)==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(100, 10)==3, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-1}, 10)==0, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-2}, 10)==1, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-10}, 10)==1, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-11}, 10)==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-100}, 10)==2, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(int8_t{-101}, 10)==3, "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::lowest(), 10)==19,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::lowest()+1, 10)==19,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::min()+1, 10)==19,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<int64_t>::max(), 10)==19,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(UINT64_C(0), 10)==0,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<uint64_t>::min(), 10)==0,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(numeric_limits<uint64_t>::max(), 10)==20,
                "cnl::_impl::used_digits test failed");
        static_assert(cnl::_impl::used_digits(INT64_C(0x7fffffff00000000), 10)==19,
                "cnl::_impl::used_digits test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::_impl::used_digits(CNL_INTMAX_C(0x7fffffff000000000000000000000000), 10)==39,
                "cnl::_impl::used_digits test failed");
#endif
    }

    namespace test_used_digits_n {
        template<int Radix>
        struct test_used_digits {
            static_assert(cnl::_impl::used_digits(-Radix*Radix-1, Radix)==3, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(-Radix*Radix, Radix)==2, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(-Radix-1, Radix)==2, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(-Radix, Radix)==1, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(-1, Radix)==0, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(0, Radix)==0, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(1, Radix)==1, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(Radix-1, Radix)==1, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(Radix, Radix)==2, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(Radix*Radix-1, Radix)==2, "cnl::_impl::used_digits<10> test failed");
            static_assert(cnl::_impl::used_digits(Radix*Radix, Radix)==3, "cnl::_impl::used_digits<10> test failed");
        };

        template
        struct test_used_digits<2>;
        template
        struct test_used_digits<3>;
        template
        struct test_used_digits<4>;
        template
        struct test_used_digits<5>;
        template
        struct test_used_digits<6>;
        template
        struct test_used_digits<7>;
        template
        struct test_used_digits<8>;
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
