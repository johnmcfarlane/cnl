
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/numeric_traits>

#include <sg14/bits/common.h>

namespace {
    using sg14::_impl::identical;

    namespace test_numeric_traits {
        using sg14::numeric_traits;

        static_assert(std::is_same<numeric_traits<int>::value_type, int>::value, "sg14::numeric_traits<> test failed");
        static_assert(!numeric_traits<float>::is_specialized, "sg14::numeric_traits<> test failed");
        static_assert(identical(numeric_traits<std::uint64_t>::to_rep(UINT64_C(0xFEDCBA9876543210)), UINT64_C(0xFEDCBA9876543210)), "sg14::numeric_traits<> test failed");
        static_assert(identical(numeric_traits<double>::from_rep(.987), .987), "sg14::numeric_traits<> test failed");
        static_assert(identical(numeric_traits<unsigned>::scale(3, 2, 15), 98304U), "sg14::numeric_traits<> test failed");
    }

    namespace test_scale {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::scale

        using sg14::scale;

        static_assert(identical(scale<uint8_t>(0b11110101, 2, 8), 0b1111010100000000), "sg14::scale test failed");
        static_assert(scale<uint8_t>(0b10110110, 2, 4) == 0b101101100000, "sg14::scale test failed");
        static_assert(scale<uint8_t>(0b00111010, 2, 2) == 0b11101000, "sg14::scale test failed");
        static_assert(scale<uint8_t>(0b11101011, 2, 0) == 0b11101011, "sg14::scale test failed");
        static_assert(scale<uint8_t>(0b01100100, 2, -2) == 0b00011001, "sg14::scale test failed");
        static_assert(scale<uint8_t>(0b00111001, 2, -4) == 0b00000011, "sg14::scale test failed");
        static_assert(scale<uint8_t>(0b10110011, 2, -8) == 0, "sg14::scale test failed");

        static_assert(scale<int8_t>(-0b1110101, 2, 8) == -0b111010100000000, "sg14::scale test failed");
        static_assert(scale<int8_t>(-0b0110110, 2, 4) == -0b01101100000, "sg14::scale test failed");
        static_assert(scale<int8_t>(+0b0011010, 2, 2) == +0b1101000, "sg14::scale test failed");
        static_assert(scale<int8_t>(-0b1101011, 2, 0) == -0b1101011, "sg14::scale test failed");
        static_assert(scale<int8_t>(+0b1100100, 2, -2) == +0b0011001, "sg14::scale test failed");
        static_assert(scale<int8_t>(+0b0111001, 2, -4) == +0b0000011, "sg14::scale test failed");
        static_assert(scale<int8_t>(-0b0110011, 2, -8) == -0b0000000, "sg14::scale test failed");

        static_assert(scale<int32_t>(1, 2, 30) == 0x40000000, "sg14::scale test failed");
        static_assert(scale<uint64_t>(1, 2, 4) == 16, "sg14::scale test failed");
    }
}
