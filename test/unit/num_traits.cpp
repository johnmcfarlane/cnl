
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/num_traits.h>

#include <cnl/_impl/common.h>
#include <cnl/_impl/type_traits/identical.h>

#include <string>

namespace {
    using cnl::_impl::identical;

    namespace testint {
        static_assert(std::is_same<
                typename std::remove_cv<decltype(cnl::numeric_limits<void>::digits)>::type,
                int>::value, "int test failed");
    }

    namespace test_to_rep {
        static_assert(identical(cnl::_impl::to_rep(UINT64_C(0xFEDCBA9876543210)), UINT64_C(0xFEDCBA9876543210)),
                      "cnl::numeric_traits<> test failed");
    }

    namespace test_from_value {
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::from_value_t<cnl::uint128, int>{123}, 123), "cnl::from_value_t<cnl::uint128, int>");
        static_assert(
                identical(
                        cnl::_impl::from_value<cnl::uint128>(123),
                        123),
                "cnl::_impl::from_value<cnl::uint128>");
#endif

        static_assert(identical(cnl::_impl::from_value<cnl::uint8>(123), 123), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(
                cnl::_impl::from_value<cnl::uint64>(cnl::int8{123}),
                cnl::int8{123}), "cnl::_impl::from_value<cnl::uint64>(cnl::int8)");

        static_assert(identical(cnl::_impl::from_value<long>(123LL), 123LL), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(
                identical(
                        cnl::_impl::from_value<long long>(123LL),
                        123LL),
                "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(cnl::_impl::from_value<long>(123L), 123L), "cnl::_impl::from_value<cnl::uint8>");
        static_assert(identical(cnl::_impl::from_value<long long>(123L), 123L), "cnl::_impl::from_value<cnl::uint8>");

        static_assert(identical(std::int16_t{123}, cnl::_impl::from_value<std::int16_t>(std::int16_t{123})),
                "cnl::_impl::from_value<std::int16_t, std::int16_t>");
        static_assert(identical(std::int32_t{123}, cnl::_impl::from_value<std::int16_t>(std::int32_t{123})),
                "cnl::_impl::from_value<std::int16_t, std::int32_t>");
        static_assert(identical(std::int64_t{123}, cnl::_impl::from_value<std::int16_t>(std::int64_t{123})),
                "cnl::_impl::from_value<std::int16_t, std::int64_t>");
        static_assert(identical(std::int16_t{123}, cnl::_impl::from_value<std::int32_t>(std::int16_t{123})),
                "cnl::_impl::from_value<std::int32_t, std::int16_t>");
        static_assert(identical(std::int32_t{123}, cnl::_impl::from_value<std::int32_t>(std::int32_t{123})),
                "cnl::_impl::from_value<std::int32_t, std::int32_t>");
        static_assert(identical(std::int64_t{123}, cnl::_impl::from_value<std::int32_t>(std::int64_t{123})),
                "cnl::_impl::from_value<std::int32_t, std::int64_t>");
        static_assert(identical(std::int16_t{123}, cnl::_impl::from_value<std::int64_t>(std::int16_t{123})),
                "cnl::_impl::from_value<std::int64_t, std::int16_t>");
        static_assert(identical(std::int32_t{123}, cnl::_impl::from_value<std::int64_t>(std::int32_t{123})),
                "cnl::_impl::from_value<std::int64_t, std::int32_t>");
        static_assert(identical(std::int64_t{123}, cnl::_impl::from_value<std::int64_t>(std::int64_t{123})),
                "cnl::_impl::from_value<std::int64_t, std::int64_t>");

        // assumes that int === int32_t
        static_assert(identical(123, cnl::_impl::from_value<std::int16_t>(cnl::constant<123>{})),
                "cnl::_impl::from_value<std::int16_t, cnl::constant<std::int64_t>>");
        static_assert(identical(123, cnl::_impl::from_value<std::int64_t>(cnl::constant<123>{})),
                "cnl::_impl::from_value<std::int64_t, cnl::constant<std::int64_t>>");
        static_assert(
                identical(
                        INT64_C(0x123456789abcdef),
                        cnl::_impl::from_value<std::int16_t>(cnl::constant<0x123456789abcdef>{})),
                "cnl::_impl::from_value<std::int16_t, cnl::constant<std::int64_t>>");
    }

    namespace test_set_digits {
        using cnl::set_digits;
        static_assert(identical(cnl::set_digits<cnl::int32, 32>::type{0}, cnl::int64{0}), "");

        static_assert(identical(cnl::set_digits<cnl::uint8, 1>::type{234}, cnl::uint8{234}), "");
        static_assert(identical(cnl::set_digits<cnl::uintmax, 1>::type{234}, cnl::uint8{234}), "");
        static_assert(identical(cnl::set_digits<cnl::int8, 1>::type{123}, cnl::int8{123}), "");
        static_assert(identical(cnl::set_digits<cnl::intmax, 1>::type{123}, cnl::int8{123}), "");

        static_assert(identical(cnl::set_digits<cnl::uint8, 10>::type{234}, cnl::uint16{234}), "");
        static_assert(identical(cnl::set_digits<cnl::uintmax, 10>::type{234}, cnl::uint16{234}), "");
        static_assert(identical(cnl::set_digits<cnl::int8, 10>::type{234}, cnl::int16{234}), "");
        static_assert(identical(cnl::set_digits<cnl::intmax, 10>::type{234}, cnl::int16{234}), "");

        static_assert(identical(cnl::set_digits<cnl::uint8, 64>::type{234}, cnl::uint64{234}), "");
        static_assert(identical(cnl::set_digits<cnl::uintmax, 64>::type{234}, cnl::uint64{234}), "");
        static_assert(identical(cnl::set_digits<cnl::int8, 63>::type{234}, cnl::int64{234}), "");
        static_assert(identical(cnl::set_digits<cnl::intmax, 63>::type{234}, cnl::int64{234}), "");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::set_digits<cnl::uint8, 128>::type{234}, cnl::uint128{234}), "");
        static_assert(identical(cnl::set_digits<cnl::uintmax, 128>::type{234}, cnl::uint128{234}), "");
        static_assert(identical(cnl::set_digits<cnl::int8, 127>::type{234}, cnl::int128{234}), "");
        static_assert(identical(cnl::set_digits<cnl::intmax, 127>::type{234}, cnl::int128{234}), "");
#endif
    }

    namespace test_digits_2 {
        static_assert(cnl::digits<cnl::int8>::value==7, "");
        static_assert(cnl::digits<cnl::uint8>::value==8, "");
        static_assert(cnl::digits<cnl::int16>::value==15, "");
        static_assert(cnl::digits<cnl::uint16>::value==16, "");
        static_assert(cnl::digits<cnl::int32>::value==31, "");
        static_assert(cnl::digits<cnl::uint32>::value==32, "");
        static_assert(cnl::digits<cnl::int64>::value==63, "");
        static_assert(cnl::digits<cnl::uint64>::value==64, "");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::digits<cnl::int128>::value==127, "");
        static_assert(cnl::digits<cnl::uint128>::value==128, "");
#endif
    }

    namespace test_shift {
        static_assert(identical(cnl::_impl::scale<15, 2, unsigned>(3), 98304U), "cnl::_impl::scale test failed");

        static_assert(identical(cnl::_impl::scale<8, 2, uint8_t>(0b11110101), 0b1111010100000000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<4, 2, uint8_t>(0b10110110), 0b101101100000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<2, 2, uint8_t>(0b00111010), 0b11101000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<0, 2, uint8_t>(0b11101011), 0b11101011), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<-2, 2, uint8_t>(0b01100100), 0b00011001), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<-4, 2, uint8_t>(0b00111001), 0b00000011), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<-8, 2, uint8_t>(0b10110011), 0), "cnl::_impl::scale test failed");

        static_assert(identical(cnl::_impl::scale<8, 2, int8_t>(-0b1110101), -0b111010100000000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<4, 2, int8_t>(-0b0110110), -0b01101100000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<2, 2, int8_t>(+0b0011010), +0b1101000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<0, 2, int8_t>(-0b1101011), -0b1101011), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<-2, 2, int8_t>(+0b1100100), +0b0011001), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<-4, 2, int8_t>(+0b0111001), +0b0000011), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<-8, 2, int8_t>(-0b0110011), -0b0000000), "cnl::_impl::scale test failed");

        static_assert(identical(cnl::_impl::scale<30, 2, int32_t>(1), 0x40000000), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<4, 2, uint64_t>(1), UINT64_C(16)), "cnl::_impl::scale test failed");

        static_assert(identical(cnl::_impl::scale<5, 3, int32_t>(1), 243), "cnl::_impl::scale test failed");
        static_assert(identical(cnl::_impl::scale<9, 4, uint64_t>(7), UINT64_C(1835008)),
                "cnl::_impl::scale test failed");
    }
}
