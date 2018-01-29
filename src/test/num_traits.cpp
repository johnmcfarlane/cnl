
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
        static_assert(identical(cnl::to_rep(UINT64_C(0xFEDCBA9876543210)), UINT64_C(0xFEDCBA9876543210)),
                      "cnl::numeric_traits<> test failed");
    }

    namespace test_from_value {
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::from_value_t<cnl::uint128, int>{123}, 123), "cnl::from_value_t<cnl::uint128, int>");
        static_assert(identical(cnl::_impl::from_value<cnl::uint128>(123), 123), "cnl::_impl::from_value<cnl::uint128>");
#endif

        static_assert(identical(cnl::_impl::from_value<cnl::uint8>(123), 123), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(
                cnl::_impl::from_value<cnl::uint64>(cnl::int8{123}),
                cnl::int8{123}), "cnl::_impl::from_value<cnl::uint64>(cnl::int8)");

        static_assert(identical(cnl::_impl::from_value<long>(123LL), 123LL), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(cnl::_impl::from_value<long long>(123LL), 123LL), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(cnl::_impl::from_value<long>(123L), 123L), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(cnl::_impl::from_value<long long>(123L), 123L), "cnl::_impl::from_value<cnl::uint8>");
    }

    namespace test_set_digits {
        using cnl::set_digits;
        static_assert(identical(cnl::set_digits<cnl::int32, 32>::type{0}, cnl::int64{0}), "");
    }

    namespace test_scale {
        static_assert(identical(cnl::_impl::shift<15, 2, unsigned>(3), 98304U), "cnl::_impl::shift test failed");

        static_assert(identical(cnl::_impl::shift<8, 2, uint8_t>(0b11110101), 0b1111010100000000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<4, 2, uint8_t>(0b10110110), 0b101101100000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<2, 2, uint8_t>(0b00111010), 0b11101000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<0, 2, uint8_t>(0b11101011), 0b11101011), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<-2, 2, uint8_t>(0b01100100), 0b00011001), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<-4, 2, uint8_t>(0b00111001), 0b00000011), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<-8, 2, uint8_t>(0b10110011), 0), "cnl::_impl::shift test failed");

        static_assert(identical(cnl::_impl::shift<8, 2, int8_t>(-0b1110101), -0b111010100000000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<4, 2, int8_t>(-0b0110110), -0b01101100000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<2, 2, int8_t>(+0b0011010), +0b1101000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<0, 2, int8_t>(-0b1101011), -0b1101011), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<-2, 2, int8_t>(+0b1100100), +0b0011001), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<-4, 2, int8_t>(+0b0111001), +0b0000011), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<-8, 2, int8_t>(-0b0110011), -0b0000000), "cnl::_impl::shift test failed");

        static_assert(identical(cnl::_impl::shift<30, 2, int32_t>(1), 0x40000000), "cnl::_impl::shift test failed");
        static_assert(identical(cnl::_impl::shift<4, 2, uint64_t>(1), UINT64_C(16)), "cnl::_impl::shift test failed");
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
