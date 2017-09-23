
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl::elastic_integer` type

#include <cnl/elastic_integer.h>

namespace {
    using std::is_same;
    using cnl::elastic_integer;
    using cnl::_impl::identical;
    using namespace cnl::literals;

    // simple one-off tests
    namespace test_digits {
        using cnl::digits;

        static_assert(digits<elastic_integer<7, int>>::value == 7, "elastic_integer test failed");
    }

    namespace test_from_value {
        using cnl::from_value;

        static_assert(std::is_same<elastic_integer<7, int>::rep, int>::value, "");
        static_assert(identical(cnl::from_value<elastic_integer<3>, int>::type{1},
                                elastic_integer<cnl::numeric_limits<int>::digits>{1}), "elastic_integer test failed");
        static_assert(identical(cnl::_impl::from_value<elastic_integer<3>>(1),
                elastic_integer<cnl::numeric_limits<int>::digits>{1}), "elastic_integer test failed");
        static_assert(identical(cnl::_impl::from_value<elastic_integer<1>>(INT32_C(0)), elastic_integer<31>{0}), "cnl::elastic_integer test failed");
    }


    namespace test_operate {
        using cnl::_impl::is_derived_from_number_base;
        using cnl::_impl::equal_tag;
        using cnl::_impl::equal_op;
        using cnl::_impl::multiply_tag;
        using cnl::_impl::operate;
        using cnl::_impl::precedes;
        using cnl::_impl::to_rep;

        static_assert(is_derived_from_number_base<elastic_integer<1>>::value,
                "cnl::_impl::is_derived_from_number_base test failed");
        static_assert(precedes<elastic_integer<1>, int32_t>::value, "");
        static_assert(!precedes<int32_t, elastic_integer<1>>::value, "");
        static_assert(
                operate(elastic_integer<31>{0x7fffffff}, elastic_integer<31>{cnl::numeric_limits<std::int32_t>::min()}, cnl::_impl::greater_than_tag),
                "cnl::elastic_integer test failed");
        static_assert(operate(
                elastic_integer<31>{0x7fffffff},
                elastic_integer<31>{cnl::numeric_limits<std::int32_t>::min()},
                cnl::_impl::greater_than_tag),
                      "cnl::elastic_integer test failed");
        static_assert(identical(
                typename std::common_type<elastic_integer<31>, elastic_integer<32, unsigned>>::type{},
                elastic_integer<32>{}),
                "cnl::elastic_integer test failed");
        static_assert(cnl::_impl::less_than_tag(elastic_integer<31, unsigned>{-1},
                                                 elastic_integer<31>{1}),
                      "cnl::elastic_integer test failed");
        static_assert(operate(elastic_integer<1>{0}, INT32_C(0), cnl::_impl::equal_tag),
                "cnl::elastic_integer test failed");
        static_assert(identical(
                operate(elastic_integer<1>{0}, INT32_C(0), multiply_tag), elastic_integer<31, int>{0}),
                "cnl::elastic_integer test failed");

        static_assert(
                equal_tag(
                        to_rep(static_cast<int>(elastic_integer<8>{1L})),
                        to_rep(static_cast<int>(elastic_integer<8>{1L}))), "");

        static_assert(operate(elastic_integer<8>{1L}, elastic_integer<8>{1}, equal_tag),
                "elastic_integer comparison test failed");
        static_assert(operate(elastic_integer<12, std::uint16_t>{1324}, 1324, equal_tag),
                "cnl::elastic_integer test failed");
        static_assert(operate(~elastic_integer<12, std::uint16_t>{0}, 0xFFF, equal_tag),
                "cnl::elastic_integer test failed");
    }

    namespace test_ctor {
        static_assert(identical(elastic_integer<8>{1L}, elastic_integer<8>{1}), "elastic_integer test failed");

        static_assert(identical(elastic_integer<4>{10_c}, elastic_integer<4>{10}), "");
#if defined(__cpp_deduction_guides)
        static_assert(identical(elastic_integer{std::integral_constant<unsigned, 1000>{}}, elastic_integer<10>{1000}), "");
#endif
        static_assert(identical(elastic_integer<10>{std::integral_constant<unsigned, 1000>{}}, elastic_integer<10>{1000}), "");
    }

    namespace test_is_elastic_integer {
        using cnl::_elastic_integer_impl::is_elastic_integer;
        static_assert(!is_elastic_integer<int>::value, "cnl::_elastic_integer_impl::is_elastic_integer test failed");
        static_assert(is_elastic_integer<elastic_integer<10, int>>::value,
                      "cnl::_elastic_integer_impl::is_elastic_integer test failed");
    }

    namespace test_make_elastic_integer {
        using cnl::make_elastic_integer;
        static_assert(identical(make_elastic_integer(136_c), elastic_integer<8, int>{136}),
                      "cnl::_elastic_integer_impl::make_elastic_integer test failed");
        static_assert(identical(make_elastic_integer(1000000000000_c), elastic_integer<40, int>{1000000000000}),
                      "cnl::_elastic_integer_impl::make_elastic_integer test failed");
    }

    namespace test_compare {
        static_assert(elastic_integer<31>{-1} < elastic_integer<31, unsigned>{1}, "cnl::elastic_integer test failed");
        static_assert(elastic_integer<31>{-1} < elastic_integer<31>{1}, "cnl::elastic_integer test failed");
        static_assert(cnl::elastic_integer<10>{0} == 0, "cnl::elastic_integer test failed");
        static_assert(elastic_integer<31>{-1} < elastic_integer<31, unsigned>{1}, "cnl::elastic_integer test failed");
        static_assert(!cnl::_impl::encompasses<cnl::elastic_integer<10>, int>::value,
                      "cnl::elastic_integer test failed");
        static_assert(elastic_integer<4, signed>{10} > elastic_integer<4, signed>{-7}, "elastic_integer comparison test failed");
    }

    namespace test_unary_subtract {
        static_assert(identical(-elastic_integer<1, unsigned>{1}, elastic_integer<1, signed>{-1}), "elastic_integer test failed");
        static_assert(identical(-elastic_integer<8, unsigned>{432}, elastic_integer<8, signed>{-432}), "operator- elastic_integer test failed");
    }

    namespace test_unary_add {
        static_assert(identical(+elastic_integer<1, unsigned>{1}, elastic_integer<1, signed>{1}), "elastic_integer test failed");
    }

    namespace test_subtract {
        // 0-0
        static_assert(
                identical(
                        elastic_integer<5>{0}-elastic_integer<5>{0},
                        elastic_integer<6>{0}),
                "cnl::elastic_integer test failed");

        // unsigned{0}-unsigned{max}
        static_assert(
                identical(
                        elastic_integer<5, unsigned>{0}-cnl::numeric_limits<elastic_integer<5, unsigned>>::max(),
                        elastic_integer<5, signed>{-31}),
                "cnl::elastic_integer test failed");

        // -signed{max}-unsigned{max}
        static_assert(
                identical(
                        -cnl::numeric_limits<elastic_integer<7>>::max()-cnl::numeric_limits<elastic_integer<4, unsigned>>::max(),
                        elastic_integer<8>{-142}),
                "cnl::elastic_integer test failed");
    }

    namespace test_add {
        // unsigned{max}+signed{max}
        static_assert(
                identical(
                        cnl::numeric_limits<elastic_integer<15, unsigned>>::max()+cnl::numeric_limits<elastic_integer<19>>::max(),
                        elastic_integer<20>{((1<<15)-1)+((1<<19)-1)}),
                "cnl::elastic_integer test failed");

        // signed{max}+signed{max}
        static_assert(
                identical(
                        cnl::numeric_limits<elastic_integer<10>>::max()-cnl::numeric_limits<elastic_integer<9>>::max(),
                        elastic_integer<11>{512}),
                "cnl::elastic_integer test failed");
    }

    namespace test_divide {
        static_assert(identical(elastic_integer<10>{777}/elastic_integer<4>{10}, elastic_integer<10>{77}),
                "cnl::elastic_integer test failed");
        static_assert(identical(elastic_integer<5>{32}/3, elastic_integer<5>{10}),
                "cnl::elastic_integer test failed");
        static_assert(identical(elastic_integer<10>{777}/10_c, elastic_integer<10>{77}),
                "cnl::elastic_integer test failed");
    }

    namespace test_bitwise_not {
        static_assert(~elastic_integer<12, std::uint16_t>{0}==0xFFF, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<12, int>{0x050}==~0x50, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<12, unsigned>{0}==0xFFFU, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<7, std::int8_t>{0x5a}==~0x5a, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<50, std::int64_t>{INT64_C(0x987654321)}==~INT64_C(0x987654321), "cnl::elastic_integer test failed");
    }

    namespace test_multiply {
        using cnl::make_elastic_integer;
        using cnl::_impl::multiply_tag;

        static_assert(identical(
                elastic_integer<15, int>{0x7fff}*elastic_integer<16, unsigned>{0xffff},
                elastic_integer<31, int>{0x7FFE8001}), "cnl::elastic_integer test failed");
        static_assert(identical(
                elastic_integer<31, int>{0x7fffffff}*elastic_integer<32, int>{0xffffffffu},
                elastic_integer<63, int>{0x7FFFFFFE80000001LL}), "cnl::elastic_integer test failed");

        static_assert(cnl::_impl::precedes<elastic_integer<1>, int32_t>::value, "");
        static_assert(identical(
                cnl::_impl::operate(elastic_integer<1>{0}, INT32_C(0), multiply_tag),
                cnl::elastic_integer<31, int>{0}),
                "cnl::elastic_integer test failed");
        static_assert(identical(cnl::_impl::to_rep(elastic_integer<4>{13}), 13), "cnl::elastic_integer test failed");
        static_assert(identical(elastic_integer<1>{0} * INT32_C(0), cnl::elastic_integer<31, int>{0}),
                      "cnl::elastic_integer test failed");
        static_assert(identical(make_elastic_integer(177_c), cnl::elastic_integer<8, int>{177}),
                      "cnl::elastic_integer test failed");
        static_assert(!cnl::_impl::precedes<cnl::elastic_integer<1>, double>::value,
                "cnl::elastic_integer test failed");
        static_assert(identical(3.*make_elastic_integer(3210), 9630.),
                "cnl::elastic_integer test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(elastic_integer<1>{0} * INT64_C(0), cnl::elastic_integer<63, int>{0}),
                      "cnl::elastic_integer test failed");
        static_assert(identical(make_elastic_integer(177_c) * INT64_C(9218), cnl::elastic_integer<71, int>{1631586}),
                      "cnl::elastic_integer test failed");
#endif
    }

    namespace test_scale {
        using cnl::_impl::scale;

        static_assert(identical(scale(elastic_integer<6>{55}, 2, 2), elastic_integer<6>{220}), "scale<elastic_integer> test failed");
    }

    namespace test_numeric_limits {
        static_assert(cnl::numeric_limits<elastic_integer<8, int>>::max()==255, "");
        static_assert(cnl::numeric_limits<elastic_integer<8, int>>::lowest()==-255, "");
        static_assert(
                cnl::numeric_limits<const int>::is_signed,
                "elastic_integer numeric_limits test failed");
        static_assert(
                cnl::numeric_limits<const cnl::elastic_integer<7, int>>::is_signed,
                "elastic_integer numeric_limits test failed");
    }

    // parameterized tests
    template<typename ElasticInteger, long long Lowest, long long Min, long long Max>
    struct elastic_integer_test {
        using value_type = ElasticInteger;
        using narrowest = typename ElasticInteger::narrowest;
        using numeric_limits = cnl::numeric_limits<value_type>;

        static constexpr value_type lowest{Lowest};
        static constexpr value_type min{Min};
        static constexpr value_type max{Max};

        ////////////////////////////////////////////////////////////////////////////////
        // members

        static constexpr int digits = value_type::digits;
        static constexpr bool is_signed = numeric_limits::is_signed;
        static_assert(is_signed==cnl::numeric_limits<narrowest>::is_signed, "narrowest is different signedness");

        ////////////////////////////////////////////////////////////////////////////////
        // type traits

        static_assert(is_signed==cnl::numeric_limits<typename value_type::rep>::is_signed,
                "signage of narrowest and rep should be the same");
        static_assert(!is_signed || numeric_limits::max()==-numeric_limits::lowest(), "type has most negative number");
        static_assert(!is_signed || -numeric_limits::max()==numeric_limits::lowest(), "type has most negative number");

        static_assert(identical(cnl::_impl::make_signed_t<value_type, is_signed>{0}, value_type{0}), "");

        ////////////////////////////////////////////////////////////////////////////////
        // constructors

        static constexpr value_type zero{0.};

        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        static_assert(zero==0., "elastic_integer comparison test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // arithmetic operators

        static constexpr auto zero_squared = zero*zero;
        using zero_squared_type = decltype(zero_squared);
        using zero_squared_narrowest = typename zero_squared_type::narrowest;
        static_assert(
                cnl::numeric_limits<decltype(zero)>::is_signed==cnl::numeric_limits<decltype(zero*zero)>::is_signed,
                "elastic_integer arithmetic test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // numeric_limits

        static_assert(numeric_limits::is_integer, "numeric_limits<elastic_integer<>>::is_integer test failed");
        static_assert(numeric_limits::lowest()==lowest, "numeric_limits<elastic_integer<>>::lowest test failed");
        static_assert(numeric_limits::min()==min, "numeric_limits<elastic_integer<>>::min test failed");
        static_assert(numeric_limits::max()==max, "numeric_limits<elastic_integer<>>::max test failed");
    };

    template
    struct elastic_integer_test<elastic_integer<7, int>, -127, 1, 127>;
    template
    struct elastic_integer_test<elastic_integer<1, int>, -1, 1, 1>;
#if defined(CNL_INT128_ENABLED)
    template
    struct elastic_integer_test<elastic_integer<39, unsigned int>, 0, 1, (INT64_C(1) << 39)-1>;
#endif

    // user-defined literal initialization
#if defined(__cpp_deduction_guides)
    // with class template deduction
    static_assert(identical(elastic_integer(1_c), elastic_integer<1>{1}), "");
#else
    // without class template deduction
    static_assert(identical(elastic_integer<1>(1_c), elastic_integer<1>{1}), "");
#endif

    namespace test_is_composite {
        using cnl::is_composite;

        static_assert(cnl::is_composite<elastic_integer<1>>::value, "cnl::is_composite<cnl::elastic_integer<>> test failed");
    }

    namespace test_digits {
        using cnl::digits;
        using cnl::set_digits_t;

        static_assert(digits<elastic_integer<3>>::value>=3, "cnl::digits / cnl::set_digits test failed");
        static_assert(identical(set_digits_t<elastic_integer<1>, 3>{6}, elastic_integer<3>{6}), "cnl::digits / cnl::set_digits test failed");
    }

    namespace test_used_bits {
        using cnl::used_bits;

        static_assert(used_bits(elastic_integer<7>{3})==2, "used_bits test failed");
        static_assert(used_bits(elastic_integer<12, std::uint16_t>{10})==4, "used_bits test failed");
    }

    namespace test_leading_bits {
        using cnl::leading_bits;

        static_assert(leading_bits(elastic_integer<7>{3})==5, "leading_bits test failed");
        static_assert(leading_bits(elastic_integer<12, std::uint16_t>{10})==8, "leading_bits test failed");
    }
}
