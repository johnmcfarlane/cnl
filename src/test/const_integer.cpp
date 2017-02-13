
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `sg14/auxiliary/const_integer.h` definitions

#include <sg14/auxiliary/const_integer.h>
#include <sg14/bits/common.h>

#include <limits>

namespace {
    using sg14::_impl::identical;

    namespace test_const_integer_impl {
        using sg14::_const_integer_impl::num_integer_bits_positive;

        namespace test_num_integer_bits_positive {
            static_assert(num_integer_bits_positive(1)==1,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
            static_assert(num_integer_bits_positive(1)==1,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
            static_assert(num_integer_bits_positive(2)==2,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
            static_assert(num_integer_bits_positive(uint8_t{255})==8,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
            static_assert(num_integer_bits_positive(int16_t{32767})==15,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
            static_assert(num_integer_bits_positive(std::numeric_limits<int64_t>::max())==63,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
            static_assert(num_integer_bits_positive(std::numeric_limits<uint64_t>::max())==64,
                "sg14::_const_integer_impl::num_integer_bits_positive test failed");
        }

        namespace test_num_integer_bits {
            using sg14::_const_integer_impl::num_integer_bits;

            static_assert(num_integer_bits(1)==1, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(2)==2, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(uint8_t{255})==8, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int16_t{32767})==15, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-1})==1, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-2})==2, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-3})==2, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-4})==3, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-5})==3, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-8})==4, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-9})==4, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(int8_t{-128})==8, "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(std::numeric_limits<int64_t>::lowest()+1)==63,
                "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(std::numeric_limits<int64_t>::min()+1)==63,
                "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(std::numeric_limits<int64_t>::max())==63,
                "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(std::numeric_limits<uint64_t>::min())==0,
                "sg14::_const_integer_impl::num_integer_bits test failed");
            static_assert(num_integer_bits(std::numeric_limits<uint64_t>::max())==64,
                "sg14::_const_integer_impl::num_integer_bits test failed");
        }

        namespace test_num_integer_zeros_positive {
            using sg14::_const_integer_impl::num_integer_zeros_positive;

            static_assert(num_integer_zeros_positive(1)==0,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(2)==1,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(3)==0,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(4)==2,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(5)==0,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(7)==0,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(8)==3,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(9)==0,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(96)==5,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(1023)==0,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
            static_assert(num_integer_zeros_positive(1024)==10,
                "sg14::_const_integer_impl::num_integer_zeros_positive test failed");
        }

        namespace test_num_integer_zeros {
            using sg14::_const_integer_impl::num_integer_zeros;

            static_assert(num_integer_zeros(1)==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(2)==1, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(uint8_t{255})==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int16_t{32767})==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-1})==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-2})==1, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-3})==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-4})==2, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-5})==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-8})==3, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-9})==0, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(int8_t{-128})==7, "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(std::numeric_limits<int64_t>::lowest()+1)==0,
                "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(std::numeric_limits<int64_t>::min()+1)==0,
                "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(std::numeric_limits<int64_t>::max())==0,
                "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(std::numeric_limits<uint64_t>::min())==0,
                "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(std::numeric_limits<uint64_t>::max())==0,
                "sg14::_const_integer_impl::num_integer_zeros test failed");
            static_assert(num_integer_zeros(std::numeric_limits<uint64_t>::max())==0,
                "sg14::_const_integer_impl::num_integer_zeros test failed");
        }

        namespace test_digits_to_integral {
            using sg14::_const_integer_impl::digits_to_integral;

            static_assert(identical(digits_to_integral<'0'>::value, INTMAX_C(0)),
                "sg14::_const_integer_impl::digits_to_integral test failed");
            static_assert(identical(digits_to_integral<'1'>::value, INTMAX_C(1)),
                "sg14::_const_integer_impl::digits_to_integral test failed");
        }
    }

    namespace test_const_integer {
        using sg14::const_integer;

        static_assert(
                identical(const_integer<int, -1>{}, const_integer<int, -1, 1, 0>{}),
                "sg14::const_integer construction test failed");

        // unary minus
#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
        static_assert(
                identical(-const_integer<std::uint8_t, 1>{}, const_integer<int, -1>{}),
                "sg14::const_integer unary minus test failed");
        static_assert(
                identical(-const_integer<std::uint8_t, 2>{}, const_integer<int, -2>{}),
                "sg14::const_integer unary minus test failed");
        static_assert(
                identical(-const_integer<unsigned, 1>{}, const_integer<unsigned, std::numeric_limits<unsigned>::max()>{}),
                "sg14::const_integer unary minus test failed");
#endif

        // binary plus
        static_assert(identical(const_integer<std::uint8_t, 2>{} + const_integer<std::intmax_t, 3>{}, const_integer<std::intmax_t, 5>{}), "sg14::const_integer addition test failed");

        // conversion to int
        static_assert(identical(static_cast<int>(const_integer<long, 77213>{}), 77213), "sg14::const_integer test failed");
    }

    namespace test_literals {
        using namespace sg14::literals;
        using sg14::const_integer;

        static_assert(identical(0_c, const_integer<intmax_t, INTMAX_C(0)>()),
            "sg14::literals test failed");
        static_assert(identical(1_c, const_integer<intmax_t, INTMAX_C(1)>()),
            "sg14::literals test failed");
        static_assert(identical(2_c, const_integer<intmax_t, INTMAX_C(2)>()),
            "sg14::literals test failed");
        static_assert(3_c == const_integer<std::int8_t, INTMAX_C(3)>(),
            "sg14::literals test failed");
        static_assert(identical(13971581_c, const_integer<intmax_t, INTMAX_C(13971581)>()),
            "sg14::literals test failed");
        static_assert(identical(9223372036854775807_c, const_integer<intmax_t, INT64_MAX>()),
            "sg14::literals test failed");

        static_assert(identical(0x10000_c, const_integer<intmax_t, 65536>()),
                      "sg14::literals test failed");
        static_assert(identical(0x91827364564738_c, const_integer<intmax_t, 0x91827364564738>()),
                      "sg14::literals test failed");

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
        static_assert(
                identical(-1_c, const_integer<intmax_t, -1>{}),
                "sg14::const_integer addition test failed");
#endif
    }
}
