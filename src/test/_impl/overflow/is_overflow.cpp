
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

    namespace test_add {
        template<typename Lhs, typename Rhs>
        using add_test = cnl::_impl::overflow_test<cnl::_impl::add_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::add_op>::positive

        static_assert(add_test<int, int>::positive(2000000000, 2000000000),
                "add_test<int, int>::positive");
        static_assert(!add_test<int, int>::positive(-2000000000, 2000000000),
                "add_test<int, int>::positive");
        static_assert(!add_test<int, int>::positive(2000000000, -2000000000),
                "add_test<int, int>::positive");
        static_assert(!add_test<int, int>::positive(-2000000000, -2000000000),
                "add_test<int, int>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::add_op>::negative

        static_assert(!add_test<int, int>::negative(2000000000, 2000000000),
                "add_test<int, int>::negative");
        static_assert(!add_test<int, int>::negative(-2000000000, 2000000000),
                "add_test<int, int>::negative");
        static_assert(!add_test<int, int>::negative(2000000000, -2000000000),
                "add_test<int, int>::negative");
        static_assert(add_test<int, int>::negative(-2000000000, -2000000000),
                "add_test<int, int>::negative");
    }

    namespace test_subtract {
        template<typename Lhs, typename Rhs>
        using subtract_test = cnl::_impl::overflow_test<cnl::_impl::subtract_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::subtract_op>::positive

        static_assert(!subtract_test<int, int>::positive(2000000000, 2000000000),
                "subtract_test<int, int>::positive");
        static_assert(!subtract_test<int, int>::positive(-2000000000, 2000000000),
                "subtract_test<int, int>::positive");
        static_assert(subtract_test<int, int>::positive(2000000000, -2000000000),
                "subtract_test<int, int>::positive");
        static_assert(!subtract_test<int, int>::positive(-2000000000, -2000000000),
                "subtract_test<int, int>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::subtract_op>::negative

        static_assert(!subtract_test<int, int>::negative(2000000000, 2000000000),
                "subtract_test<int, int>::negative");
        static_assert(subtract_test<int, int>::negative(-2000000000, 2000000000),
                "subtract_test<int, int>::negative");
        static_assert(!subtract_test<int, int>::negative(2000000000, -2000000000),
                "subtract_test<int, int>::negative");
        static_assert(!subtract_test<int, int>::negative(-2000000000, -2000000000),
                "subtract_test<int, int>::negative");

        static_assert(subtract_test<unsigned, unsigned>::negative(0, 1),
                "subtract_test<int, int>::negative");
    }

    namespace test_multiply {
        template<typename Lhs, typename Rhs>
        using multiply_test = cnl::_impl::overflow_test<cnl::_impl::multiply_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::multiply_op>::positive

        static_assert(multiply_test<int, int>::positive(2000000000, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(21, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2000000000, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-21, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2000000000, -2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(21, -2),
                "multiply_test<int, int>::positive");
        static_assert(multiply_test<int, int>::positive(-2000000000, -2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-21, -2),
                "multiply_test<int, int>::positive");

        static_assert(multiply_test<int, int>::positive(2, 2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2, 21),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2, -2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2, -21),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2, 2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2, 21),
                "multiply_test<int, int>::positive");
        static_assert(multiply_test<int, int>::positive(-2, -2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2, -21),
                "multiply_test<int, int>::positive");

        // boundary
        static_assert(multiply_test<int, int>::positive(-46341, -46341),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46341, -46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46341, 46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46341, 46341),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46340, -46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46340, 46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46340, 46341),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(46340, 46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(46340, 46341),
                      "multiply_test<int, int>::positive");
        static_assert(multiply_test<int, int>::positive(46341, 46341),
                      "multiply_test<int, int>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::multiply_op>::negative

        static_assert(!multiply_test<int, int>::negative(2000000000, 2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(21, 2),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(-2000000000, 2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-21, 2),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(2000000000, -2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(21, -2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2000000000, -2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-21, -2),
                "multiply_test<int, int>::negative");

        static_assert(!multiply_test<int, int>::negative(2, 2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(2, 21),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(2, -2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(2, -21),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(-2, 2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2, 21),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2, -2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2, -21),
                "multiply_test<int, int>::negative");

        // boundary
        static_assert(!multiply_test<int, int>::negative(-46341, -46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46341, -46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46341, 46340),
                      "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(-46341, 46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46340, -46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46340, 46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46340, 46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(46340, 46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(46340, 46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(46341, 46341),
                      "multiply_test<int, int>::negative");

        ////////////////////////////////////////////////////////////////////////////////
        // most negative number

        static_assert(multiply_test<int, int>::positive(INT_MIN, -1), "multiply most negative number");
        static_assert(!multiply_test<int, int>::negative(INT_MIN, -1), "multiply most negative number");
    }

    namespace test_divide {
        template<typename Lhs, typename Rhs>
        using divide_test = cnl::_impl::overflow_test<cnl::_impl::divide_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // most negative number

        static_assert(cnl::_impl::has_most_negative_number<int>::value, "");
        static_assert(divide_test<int, int>::positive(INT_MIN, -1), "divide most negative number");
        static_assert(!divide_test<int, int>::negative(INT_MIN, -1), "divide most negative number");
    }
}
