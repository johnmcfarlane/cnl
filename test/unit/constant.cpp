
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/constant.h` definitions

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/constant.h>

namespace {
    using cnl::_impl::identical;

    namespace test_literals {
        using namespace cnl::literals;

        static_assert(identical(
                0b011010000110100000011111101000000010110110101_c,
                cnl::constant<CNL_INTMAX_C(0b011010000110100000011111101000000010110110101)>()),
                "cnl::literals test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                0b1101000011010000001111110100000001011011010101101000011010000001111110100000001011011010101101000011010000001111110100000001011_c,
                cnl::constant<CNL_INTMAX_C(0b1101000011010000001111110100000001011011010101101000011010000001111110100000001011011010101101000011010000001111110100000001011)>()),
                "cnl::literals test failed");
#endif

        static_assert(identical(07777041726354_c, cnl::constant<CNL_INTMAX_C(07777041726354)>()),
                "cnl::literals test failed");

        static_assert(identical(0_c, cnl::constant<CNL_INTMAX_C(0)>()),
                "cnl::literals test failed");
        static_assert(identical(1_c, cnl::constant<CNL_INTMAX_C(1)>()),
                "cnl::literals test failed");
        static_assert(identical(2_c, cnl::constant<CNL_INTMAX_C(2)>()),
                "cnl::literals test failed");
        static_assert(3_c==cnl::constant<CNL_INTMAX_C(3)>(),
                "cnl::literals test failed");
        static_assert(identical(13971581_c, cnl::constant<CNL_INTMAX_C(13971581)>()),
                "cnl::literals test failed");
        static_assert(identical(9081726354_c, cnl::constant<CNL_INTMAX_C(9081726354)>()),
                "cnl::literals test failed");
        static_assert(identical(9223372036854775807_c, cnl::constant<CNL_INTMAX_C(0x7FFFFFFFFFFFFFFF)>()),
                "cnl::literals test failed");

        static_assert(identical(0x10000_c, cnl::constant<CNL_INTMAX_C(65536)>()),
                "cnl::literals test failed");
        static_assert(identical(0x9081726354_c, cnl::constant<CNL_INTMAX_C(0x9081726354)>()),
                "cnl::literals test failed");
        static_assert(identical(0x91827364564738_c, cnl::constant<CNL_INTMAX_C(0x91827364564738)>()),
                "cnl::literals test failed");
    }

    namespace test_constant {
        using namespace cnl::literals;

        static_assert(identical(128_c, cnl::constant<CNL_INTMAX_C(128)>{}), "cnl::constant test failed");
        static_assert(identical(127_c, cnl::constant<CNL_INTMAX_C(127)>{}), "cnl::constant test failed");
        static_assert(identical(-128_c, cnl::constant<-CNL_INTMAX_C(128)>{}), "cnl::constant test failed");
        static_assert(identical(-127_c, cnl::constant<-CNL_INTMAX_C(127)>{}), "cnl::constant test failed");

#if defined(CNL_TEMPLATE_AUTO)
        static_assert(std::is_same<cnl::uint16, cnl::constant<cnl::uint16{1234}>::value_type>::value);
#endif
    }

    namespace test_operators {
#if defined(CNL_TEMPLATE_AUTO)
        static_assert(identical(cnl::constant<int{11}>{}, cnl::constant<cnl::uint8{6}>{}+cnl::constant<cnl::int8{5}>{}));
        static_assert(identical(cnl::constant<int{-115}>{}, cnl::constant<short{-23}>{}*cnl::constant<cnl::uint8{5}>{}));
        static_assert(identical(
                cnl::constant<cnl::uint64{0x5F5FFAAF5A0FFFFF}>{},
                cnl::constant<cnl::uint64{0x5A0F5A0F5A0F5A0F}>{} | cnl::constant<cnl::uint64{0x5555AAAA0000FFFF}>{}));
        static_assert(identical(
                cnl::constant<cnl::uint64{0x50050A0A00005a0F}>{},
                cnl::constant<cnl::uint64{0x5A0F5A0F5A0F5A0F}>{} & cnl::constant<cnl::uint64{0x5555AAAA0000FFFF}>{}));
#else
        static_assert(identical(
                cnl::constant<cnl::intmax{11}>{},
                cnl::constant<cnl::uint8{6}>{}+cnl::constant<cnl::int8{5}>{}), "cnl::constant test failed");
        static_assert(identical(
                cnl::constant<cnl::intmax{-115}>{},
                cnl::constant<short{-23}>{}*cnl::constant<cnl::uint8{5}>{}), "cnl::constant test failed");
        static_assert(identical(
                cnl::constant<cnl::intmax{0x5F5FFAAF5A0FFFFF}>{},
                cnl::constant<cnl::uint64{0x5A0F5A0F5A0F5A0F}>{} | cnl::constant<cnl::uint64{0x5555AAAA0000FFFF}>{}),
                "cnl::constant test failed");
        static_assert(identical(
                cnl::constant<cnl::intmax{0x50050A0A00005a0F}>{},
                cnl::constant<cnl::uint64{0x5A0F5A0F5A0F5A0F}>{} & cnl::constant<cnl::uint64{0x5555AAAA0000FFFF}>{}),
                "cnl::constant test failed");
#endif
    }
}
