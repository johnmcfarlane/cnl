
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/overflow/is_overflow.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::_impl::identical;

    namespace test_positive_digits {
        using cnl::_impl::positive_digits;

        static_assert(positive_digits<int8_t>::value==7,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint8_t>::value==8,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<int16_t>::value==15,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint16_t>::value==16,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<int32_t>::value==31,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint32_t>::value==32,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<int64_t>::value==63,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint64_t>::value==64,
                      "cnl::_impl::positive_digits test failed");
    }
    
    namespace test_negative_digits {
        using cnl::_impl::negative_digits;

        static_assert(negative_digits<int8_t>::value==7,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint8_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<int16_t>::value==15,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint16_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<int32_t>::value==31,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint32_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<int64_t>::value==63,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint64_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
    }

    namespace test_is_convert_overflow_positive_integer {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow_positive;

        static_assert(!is_overflow_positive<convert_op, uint8_t, int8_t>(-1),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, int8_t>(127),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, uint16_t>(0),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, uint16_t>(255),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, uint16_t>(256),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, int16_t>(-1),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, int16_t>(0),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, int16_t>(255),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, int16_t>(256),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, int16_t>(-1),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");

        static_assert(!is_overflow_positive<convert_op, int8_t, uint8_t>(0),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, uint8_t>(127),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, uint8_t>(128),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, uint16_t>(0),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, uint16_t>(127),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, uint16_t>(128),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, int16_t>(-1),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, int16_t>(0),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, int16_t>(127),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, int16_t>(128),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");

        static_assert(!is_overflow_positive<convert_op, uint16_t, uint8_t>(0),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint16_t, uint8_t>(255),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint16_t, int8_t>(-128),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint16_t, int8_t>(127),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint16_t, int16_t>(-32768),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint16_t, int16_t>(32767),
                      "cnl::_impl::is_overflow_positive<convert_op, > test failed");
    }

    namespace test_is_convert_overflow_negative_integer {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow_negative;

        static_assert(is_overflow_negative<convert_op, uint8_t, int8_t>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, int8_t>(127),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, uint16_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, uint16_t>(255),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, uint16_t>(256),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, uint8_t, int16_t>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, int16_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, int16_t>(255),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, int16_t>(256),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, uint8_t, int16_t>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");

        static_assert(!is_overflow_negative<convert_op, int8_t, uint8_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, uint8_t>(127),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, uint8_t>(128),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, uint16_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, uint16_t>(127),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, uint16_t>(128),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, int8_t, int16_t>(-129),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, int16_t>(-128),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, int16_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");

        static_assert(!is_overflow_negative<convert_op, uint16_t, uint8_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint16_t, uint8_t>(255),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, uint16_t, int8_t>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint16_t, int8_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint16_t, int8_t>(127),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, uint16_t, int16_t>(-32768),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint16_t, int16_t>(32767),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
    }

    namespace test_is_convert_overflow_positive_float {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow_positive;

        static_assert(!is_overflow_positive<convert_op, uint8_t, float>(0),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, float>(255),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, float>(256),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, double>(-1),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, double>(0),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, double>(255),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, double>(256),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, uint8_t, double>(-1),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");

        static_assert(!is_overflow_positive<convert_op, int8_t, float>(0),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, float>(127),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, float>(128),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, double>(-1),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, double>(0),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, int8_t, double>(127),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(is_overflow_positive<convert_op, int8_t, double>(128),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");

        static_assert(!is_overflow_positive<convert_op, float, uint8_t>(0),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, float, uint8_t>(255),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, float, int8_t>(-128),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, float, int8_t>(127),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, float, double>(-32768),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
        static_assert(!is_overflow_positive<convert_op, float, double>(32767),
                "cnl::_impl::is_overflow_positive<convert_op, > test failed");
    }

    namespace test_is_convert_overflow_negative_float {
        using cnl::_impl::convert_op;
        using cnl::_impl::is_overflow_negative;

        static_assert(!is_overflow_negative<convert_op, uint8_t, float>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, float>(255),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, float>(256),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, uint8_t, float>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, float>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, float>(255),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, uint8_t, float>(256),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, uint8_t, float>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");

        static_assert(!is_overflow_negative<convert_op, int8_t, float>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, float>(127),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, float>(128),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(is_overflow_negative<convert_op, int8_t, float>(-129),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, float>(-128),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, int8_t, float>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");

        static_assert(!is_overflow_negative<convert_op, float, uint8_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, float, uint8_t>(255),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, float, int8_t>(-1),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, float, int8_t>(0),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, float, int8_t>(127),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, float, float>(-32768),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
        static_assert(!is_overflow_negative<convert_op, float, float>(32767),
                "cnl::_impl::is_overflow_negative<convert_op, > test failed");
    }

    namespace test_minus {
        template<typename Rhs>
        using minus_test = cnl::_impl::overflow_test<cnl::_impl::minus_op, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::minus_op>::positive

        static_assert(!minus_test<int>::positive(INT_MAX),
                      "minus_test<int>::positive");
        static_assert(!minus_test<int>::positive(-INT_MAX),
                      "minus_test<int>::positive");
        static_assert(minus_test<int>::positive(INT_MIN),
                      "minus_test<int>::positive");

        static_assert(!minus_test<unsigned>::positive(0),
                      "minus_test<unsigned>::positive");
        static_assert(!minus_test<unsigned>::positive(1),
                      "minus_test<unsigned>::positive");
        static_assert(!minus_test<unsigned>::positive(UINT_MAX),
                      "minus_test<unsigned>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::minus_op>::negative

        static_assert(!minus_test<int>::negative(INT_MAX),
                      "minus_test<int>::negative");
        static_assert(!minus_test<int>::negative(-INT_MAX),
                      "minus_test<int>::negative");
        static_assert(!minus_test<int>::negative(INT_MIN),
                      "minus_test<int>::negative");

        static_assert(!minus_test<unsigned>::negative(0),
                      "minus_test<unsigned>::negative");
        static_assert(minus_test<unsigned>::negative(1),
                      "minus_test<unsigned>::negative");
        static_assert(minus_test<unsigned>::negative(UINT_MAX),
                      "minus_test<unsigned>::negative");
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
