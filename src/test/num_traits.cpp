
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/num_traits.h>

#include <cnl/bits/common.h>

#include <string>

namespace {
    using cnl::_impl::identical;

    namespace test_digits_type {
        static_assert(std::is_same<
                typename std::remove_cv<decltype(cnl::numeric_limits<void>::digits)>::type,
                cnl::_digits_type>::value, "cnl::_digits_type test failed");
    }

    namespace test_to_rep {
        using cnl::_impl::to_rep;

        static_assert(identical(to_rep(UINT64_C(0xFEDCBA9876543210)), UINT64_C(0xFEDCBA9876543210)),
                      "cnl::numeric_traits<> test failed");
    }

    namespace test_set_digits {
        using cnl::set_digits;
        static_assert(identical(cnl::set_digits<std::int32_t, 32>::type{0}, std::int64_t{0}), "");
    }

    namespace test_scale {
        using cnl::_impl::scale;

        static_assert(identical(scale<unsigned>(3, 2, 15), 98304U),
                      "cnl::numeric_traits<> test failed");

        static_assert(identical(scale<uint8_t>(0b11110101, 2, 8), 0b1111010100000000), "cnl::scale test failed");
        static_assert(scale<uint8_t>(0b10110110, 2, 4) == 0b101101100000, "cnl::scale test failed");
        static_assert(scale<uint8_t>(0b00111010, 2, 2) == 0b11101000, "cnl::scale test failed");
        static_assert(scale<uint8_t>(0b11101011, 2, 0) == 0b11101011, "cnl::scale test failed");
        static_assert(scale<uint8_t>(0b01100100, 2, -2) == 0b00011001, "cnl::scale test failed");
        static_assert(scale<uint8_t>(0b00111001, 2, -4) == 0b00000011, "cnl::scale test failed");
        static_assert(scale<uint8_t>(0b10110011, 2, -8) == 0, "cnl::scale test failed");

        static_assert(scale<int8_t>(-0b1110101, 2, 8) == -0b111010100000000, "cnl::scale test failed");
        static_assert(scale<int8_t>(-0b0110110, 2, 4) == -0b01101100000, "cnl::scale test failed");
        static_assert(scale<int8_t>(+0b0011010, 2, 2) == +0b1101000, "cnl::scale test failed");
        static_assert(scale<int8_t>(-0b1101011, 2, 0) == -0b1101011, "cnl::scale test failed");
        static_assert(scale<int8_t>(+0b1100100, 2, -2) == +0b0011001, "cnl::scale test failed");
        static_assert(scale<int8_t>(+0b0111001, 2, -4) == +0b0000011, "cnl::scale test failed");
        static_assert(scale<int8_t>(-0b0110011, 2, -8) == -0b0000000, "cnl::scale test failed");

        static_assert(scale<int32_t>(1, 2, 30) == 0x40000000, "cnl::scale test failed");
        static_assert(scale<uint64_t>(1, 2, 4) == 16, "cnl::scale test failed");
    }

    namespace test_is_integer_or_float {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_integer_or_float

        using cnl::_impl::is_integer_or_float;
        static_assert(is_integer_or_float<uint8_t>::value, "cnl::_integer_impl::is_integer_or_float test failed");
        static_assert(is_integer_or_float<float>::value, "cnl::_integer_impl::is_integer_or_float test failed");
        static_assert(!is_integer_or_float<void>::value, "cnl::_integer_impl::is_integer_or_float test failed");
        static_assert(!is_integer_or_float<int *>::value, "cnl::_integer_impl::is_integer_or_float test failed");
        static_assert(!is_integer_or_float<std::string>::value, "cnl::_integer_impl::is_integer_or_float test failed");
    }
}
