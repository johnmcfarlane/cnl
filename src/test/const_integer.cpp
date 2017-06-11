
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `sg14/auxiliary/const_integer.h` definitions

#include <sg14/auxiliary/const_integer.h>
#include <sg14/bits/type_traits.h>

namespace {
    using sg14::_impl::identical;

    namespace test_const_integer_impl {

        namespace test_digits_to_integral {
            using sg14::_const_integer_impl::digits_to_integral;

            static_assert(identical(digits_to_integral<'0'>::value, INTMAX_C(0)),
                "sg14::_const_integer_impl::digits_to_integral test failed");
            static_assert(identical(digits_to_integral<'1'>::value, INTMAX_C(1)),
                "sg14::_const_integer_impl::digits_to_integral test failed");
        }

        namespace test_operate {
            using namespace sg14;
            using namespace literals;
            using namespace _impl;
            using _const_integer_impl::operate;
            static_assert(
                    identical(operate(
                            const_integer<std::uint8_t, 2>{},
                            const_integer<std::intmax_t, 3>{}, _impl::add_tag),
                            const_integer<std::intmax_t, 5>{}),
                    "sg14::_const_integer_impl::digits_to_integral test failed");
            static_assert(identical(_const_integer_impl::operate(777, 10_c, _impl::divide_tag), INTMAX_C(77)),
                    "sg14::elastic_integer test failed");
        }
    }

    namespace test_const_integer {
        using sg14::const_integer;

        static_assert(
                identical(const_integer<int, -1>{}, const_integer<int, -1, 0, 0>{}),
                "sg14::const_integer construction test failed");

        // unary minus
#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
        static_assert(
                identical(-const_integer<std::uint8_t, 1>{}, const_integer<int, -1>{}),
                "sg14::const_integer unary minus test failed");
        static_assert(
                identical(-const_integer<std::uint8_t, 2>{}, const_integer<int, -2>{}),
                "sg14::const_integer unary minus test failed");
#if ! defined(_MSC_VER)
        static_assert(
                identical(-const_integer<unsigned, 1>{}, const_integer<unsigned, std::numeric_limits<unsigned>::max()>{}),
                "sg14::const_integer unary minus test failed");
#endif
#endif

        // binary plus
        static_assert(identical(const_integer<std::uint8_t, 2>{} + const_integer<std::intmax_t, 3>{}, const_integer<std::intmax_t, 5>{}), "sg14::const_integer addition test failed");

        // divide
        using namespace sg14::literals;
        static_assert(identical(777/ 10_c, INTMAX_C(77)),
                "sg14::elastic_integer test failed");

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
