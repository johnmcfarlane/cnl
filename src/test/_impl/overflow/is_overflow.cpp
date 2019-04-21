
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/overflow/is_overflow.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::_impl::identical;

    namespace test_overflow_digits_positive {
        using cnl::_impl::overflow_digits;
        using cnl::_impl::polarity;

        static_assert(overflow_digits<int8_t, polarity::positive>::value==7,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint8_t, polarity::positive>::value==8,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<int16_t, polarity::positive>::value==15,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint16_t, polarity::positive>::value==16,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<int32_t, polarity::positive>::value==31,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint32_t, polarity::positive>::value==32,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<int64_t, polarity::positive>::value==63,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint64_t, polarity::positive>::value==64,
                "cnl::_impl::overflow_digits test failed");
    }

    namespace test_negative_digits {
        using cnl::_impl::overflow_digits;
        using cnl::_impl::polarity;

        static_assert(overflow_digits<int8_t, polarity::negative>::value==7,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint8_t, polarity::negative>::value==0,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<int16_t, polarity::negative>::value==15,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint16_t, polarity::negative>::value==0,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<int32_t, polarity::negative>::value==31,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint32_t, polarity::negative>::value==0,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<int64_t, polarity::negative>::value==63,
                "cnl::_impl::overflow_digits test failed");
        static_assert(overflow_digits<uint64_t, polarity::negative>::value==0,
                "cnl::_impl::overflow_digits test failed");
    }

    namespace test_is_convert_overflow_positive_integer {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow;
        using cnl::_impl::polarity;

        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(int8_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(int8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(uint16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(uint16_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint16_t{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(int16_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(int16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(int16_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(int16_t{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(int16_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");

        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint8_t{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint16_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(uint16_t{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(int16_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(int16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(int16_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(int16_t{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");

        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint16_t>(uint8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint16_t>(uint8_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint16_t>(int8_t{-128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint16_t>(int8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint16_t>(int16_t{-32768}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint16_t>(int16_t{32767}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
    }

    namespace test_is_convert_overflow_negative_integer {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow;
        using cnl::_impl::polarity;

        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int8_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(uint16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(uint16_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(uint16_t{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int16_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int16_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int16_t{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(int16_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");

        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(uint8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(uint8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(uint8_t{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(uint16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(uint16_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(uint16_t{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(int16_t{-129}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(int16_t{-128}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(int16_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");

        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(uint8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(uint8_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(int8_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(int8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(int8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(int16_t{-32768}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint16_t>(int16_t{32767}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
    }

    namespace test_is_convert_overflow_positive_float {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow;
        using cnl::_impl::polarity;

        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(float{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(float{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(float{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(double{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(double{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(double{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(double{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<uint8_t>(double{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");

        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(float{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(float{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(float{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(double{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(double{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(double{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(is_overflow<convert_op, polarity::positive>{}.template operator()<int8_t>(double{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");

        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<float>(uint8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<float>(uint8_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<float>(int8_t{-128}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<float>(int8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<float>(double{-32768}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
        static_assert(!is_overflow<convert_op, polarity::positive>{}.template operator()<float>(double{32767}),
                "cnl::_impl::is_overflow<convert_op, polarity::positive> test failed");
    }

    namespace test_is_convert_overflow_negative_float {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow;
        using cnl::_impl::polarity;

        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{256}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<uint8_t>(float{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");

        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(float{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(float{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(float{128}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(float{-129}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(float{-128}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<int8_t>(float{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");

        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(uint8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(uint8_t{255}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(int8_t{-1}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(int8_t{0}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(int8_t{127}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(float{-32768}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
        static_assert(!is_overflow<convert_op, polarity::negative>{}.template operator()<float>(float{32767}),
                "cnl::_impl::is_overflow<convert_op, polarity::negative>{}.template operator()<> test failed");
    }

    namespace test_minus_positive {
        using test_minus_positive = cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>;

        static_assert(!test_minus_positive{}(INT_MAX),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>{}");
        static_assert(!test_minus_positive{}(-INT_MAX),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>{}");
        static_assert(test_minus_positive{}(INT_MIN),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>{}");

        static_assert(!test_minus_positive{}(0U),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>{}");
        static_assert(!test_minus_positive{}(1U),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>{}");
        static_assert(!test_minus_positive{}(UINT_MAX),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::positive>{}");
    }

    namespace test_minus_negative {
        using test_minus_negative = cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>;

        static_assert(!test_minus_negative{}(INT_MAX),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>{}");
        static_assert(!test_minus_negative{}(-INT_MAX),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>{}");
        static_assert(!test_minus_negative{}(INT_MIN),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>{}");

        static_assert(!test_minus_negative{}(0U),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>{}");
        static_assert(test_minus_negative{}(1U),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>{}");
        static_assert(test_minus_negative{}(UINT_MAX),
                "cnl::_impl::is_overflow<cnl::_impl::minus_op, cnl::_impl::polarity::negative>{}");
    }

    namespace test_add_positive {
        using test_add_positive = cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::positive>;

        static_assert(test_add_positive{}(2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::positive>");
        static_assert(!test_add_positive{}(-2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::positive>");
        static_assert(!test_add_positive{}(2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::positive>");
        static_assert(!test_add_positive{}(-2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::positive>");
    }

    namespace test_add_negative {
        using test_add_negative = cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::negative>;

        static_assert(!test_add_negative{}(2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::negative>");
        static_assert(!test_add_negative{}(-2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::negative>");
        static_assert(!test_add_negative{}(2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::negative>");
        static_assert(test_add_negative{}(-2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::add_op, cnl::_impl::polarity::negative>");
    }

    namespace test_subtract_positive {
        using test_subtract_positive = cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>;

        static_assert(!test_subtract_positive{}(2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");
        static_assert(!test_subtract_positive{}(-2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");
        static_assert(test_subtract_positive{}(2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");
        static_assert(!test_subtract_positive{}(-2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");

        static_assert(test_subtract_positive{}(0, cnl::numeric_limits<int>::min()),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");
        static_assert(!test_subtract_positive{}(-1, cnl::numeric_limits<int>::min()),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");
    }

    namespace test_subtract_negative {
        using test_subtract_negative = cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::negative>;

        static_assert(!test_subtract_negative{}(2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::negative>");
        static_assert(test_subtract_negative{}(-2000000000, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::negative>");
        static_assert(!test_subtract_negative{}(2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::negative>");
        static_assert(!test_subtract_negative{}(-2000000000, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::negative>");

        static_assert(!test_subtract_negative{}(0, cnl::numeric_limits<int>::min()),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");
        static_assert(!test_subtract_negative{}(-1, cnl::numeric_limits<int>::min()),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::positive>");

        static_assert(test_subtract_negative{}(0u, 1u),
                "cnl::_impl::is_overflow<cnl::_impl::subtract_op, cnl::_impl::polarity::negative>");
    }

    namespace test_multiply_positive {
        using test_multiply_positive = cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>;

        static_assert(test_multiply_positive{}(2000000000, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(21, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-2000000000, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-21, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(2000000000, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(21, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(test_multiply_positive{}(-2000000000, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-21, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");

        static_assert(test_multiply_positive{}(2, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(2, 21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(2, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(2, -21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-2, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-2, 21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(test_multiply_positive{}(-2, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-2, -21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");

        // boundary
        static_assert(test_multiply_positive{}(-46341, -46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-46341, -46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-46341, 46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-46341, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-46340, -46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-46340, 46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(-46340, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(46340, 46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(!test_multiply_positive{}(46340, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
        static_assert(test_multiply_positive{}(46341, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::positive>");
    }

    namespace test_multiply_negative {
        using test_multiply_negative = cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::multiply_op>::negative

        static_assert(!test_multiply_negative{}(2000000000, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(21, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(test_multiply_negative{}(-2000000000, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-21, 2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(test_multiply_negative{}(2000000000, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(21, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-2000000000, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-21, -2),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");

        static_assert(!test_multiply_negative{}(2, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(2, 21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(test_multiply_negative{}(2, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(2, -21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(test_multiply_negative{}(-2, 2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-2, 21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-2, -2000000000),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-2, -21),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");

        // boundary
        static_assert(!test_multiply_negative{}(-46341, -46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-46341, -46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-46341, 46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(test_multiply_negative{}(-46341, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-46340, -46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-46340, 46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(-46340, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(46340, 46340),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(46340, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
        static_assert(!test_multiply_negative{}(46341, 46341),
                "cnl::_impl::is_overflow<cnl::_impl::multiply_op, cnl::_impl::polarity::negative>");
    }

    namespace test_multiply_mnn {
        template<cnl::_impl::polarity Polarity>
        using multiply_test = cnl::_impl::is_overflow<cnl::_impl::multiply_op, Polarity>;

        static_assert(multiply_test<cnl::_impl::polarity::positive>{}(INT_MIN, -1),
                "cnl::_impl::is_overflow<cnl::_impl::divide_op, polarity::positive>");
        static_assert(!multiply_test<cnl::_impl::polarity::negative>{}(INT_MIN, -1),
                "cnl::_impl::is_overflow<cnl::_impl::divide_op, polarity::negative>");
    }

    namespace test_divide {
        template<cnl::_impl::polarity Polarity>
        using divide_test = cnl::_impl::is_overflow<cnl::_impl::divide_op, Polarity>;

        ////////////////////////////////////////////////////////////////////////////////
        // most negative number

        static_assert(cnl::_impl::has_most_negative_number<int>::value, "");
        static_assert(
                divide_test<cnl::_impl::polarity::positive>{}(INT_MIN, -1),
                "cnl::_impl::is_overflow<cnl::_impl::divide_op, cnl::_impl::polarity::positive>");
        static_assert(
                !divide_test<cnl::_impl::polarity::negative>{}(INT_MIN, -1),
                "cnl::_impl::is_overflow<cnl::_impl::divide_op, cnl::_impl::polarity::negative>");
    }

    namespace test_shift_left_positive {
        using shift_left_test = cnl::_impl::is_overflow<cnl::_impl::shift_left_op, cnl::_impl::polarity::positive>;

        static_assert(
                shift_left_test{}(std::uint8_t{255}, 30u),
                "cnl::_impl::is_overflow<cnl::_impl::shift_left_op, cnl::_impl::polarity::positive>");
    }
}
