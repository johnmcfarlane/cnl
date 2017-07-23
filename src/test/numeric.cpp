
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/auxiliary/numeric.h>
#include <cnl/bits/limits.h>

#include <gtest/gtest.h>

#include <cstdint>
#include <cnl/bits/type_traits.h>

namespace {
    using namespace cnl;
    using std::int8_t;
    using std::uint8_t;
    using std::int64_t;

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
            static_assert(used_bits_positive(std::numeric_limits<int64_t>::max())==63,
                    "cnl::_numeric_impl::used_bits_positive test failed");
            static_assert(used_bits_positive(std::numeric_limits<uint64_t>::max())==64,
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
        static_assert(_impl::used_bits_symmetric(std::numeric_limits<int64_t>::lowest()+1)==63,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(std::numeric_limits<int64_t>::min()+1)==63,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(std::numeric_limits<int64_t>::max())==63,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(std::numeric_limits<uint64_t>::min())==0,
                "cnl::_impl::used_bits_symmetric test failed");
        static_assert(_impl::used_bits_symmetric(std::numeric_limits<uint64_t>::max())==64,
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
        static_assert(used_bits(std::numeric_limits<int64_t>::lowest())==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(std::numeric_limits<int64_t>::lowest()+1)==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(std::numeric_limits<int64_t>::min()+1)==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(std::numeric_limits<int64_t>::max())==63,
                "cnl::used_bits test failed");
        static_assert(used_bits(UINT64_C(0))==0,
                "cnl::used_bits test failed");
        static_assert(used_bits(std::numeric_limits<uint64_t>::min())==0,
                "cnl::used_bits test failed");
        static_assert(used_bits(std::numeric_limits<uint64_t>::max())==64,
                "cnl::used_bits test failed");
    }

    namespace test_trailing_bits {
        static_assert(trailing_bits(1)==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(2)==1, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(uint8_t{255})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int16_t{32767})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-1})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-2})==1, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-3})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-4})==2, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-5})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-8})==3, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-9})==0, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(int8_t{-128})==7, "cnl::trailing_bits test failed");
        static_assert(trailing_bits(std::numeric_limits<int64_t>::lowest()+1)==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(std::numeric_limits<int64_t>::min()+1)==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(std::numeric_limits<int64_t>::max())==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(std::numeric_limits<uint64_t>::min())==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(std::numeric_limits<uint64_t>::max())==0,
                "cnl::trailing_bits test failed");
        static_assert(trailing_bits(std::numeric_limits<uint64_t>::max())==0,
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

        TEST(numeric, leading_bits) {
            ASSERT_EQ(leading_bits(INT32_C(0)), 31);
            ASSERT_EQ(leading_bits(INT32_C(127)), 24);
            ASSERT_EQ(leading_bits(INT32_C(64)), 24);
            ASSERT_EQ(leading_bits(INT32_C(63)), 25);
            ASSERT_EQ(leading_bits(INT32_C(4)), 28);
            ASSERT_EQ(leading_bits(INT32_C(3)), 29);
            ASSERT_EQ(leading_bits(INT32_C(2)), 29);
            ASSERT_EQ(leading_bits(INT32_C(1)), 30);
            ASSERT_EQ(leading_bits(INT32_C(-1)), 31);
            ASSERT_EQ(leading_bits(INT32_C(-2)), 30);
            ASSERT_EQ(leading_bits(INT32_C(-3)), 29);
            ASSERT_EQ(leading_bits(INT32_C(-4)), 29);
            ASSERT_EQ(leading_bits(INT32_C(-5)), 28);
            ASSERT_EQ(leading_bits(INT32_C(-129)), 23);
            ASSERT_EQ(leading_bits(INT32_C(-128)), 24);
            ASSERT_EQ(leading_bits(INT32_C(-127)), 24);
            ASSERT_EQ(leading_bits(INT32_C(-65)), 24);
            ASSERT_EQ(leading_bits(INT32_C(-64)), 25);
        }
    }
}
