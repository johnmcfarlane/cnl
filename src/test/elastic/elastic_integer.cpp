
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl::elastic_integer` type

#include <cnl/elastic_integer.h>
#include <cnl/numeric.h>
#include <cnl/_impl/rounding.h>
#include <cnl/_impl/common.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::assert_same;
    using cnl::elastic_integer;
    using cnl::_impl::identical;
    using namespace cnl::literals;

    ////////////////////////////////////////////////////////////////////////////////
    // simple one-off tests
    namespace test_depth {
        static_assert(cnl::_impl::depth<int>::value == 0, "");
        static_assert(assert_same<int&&, decltype(to_rep(elastic_integer<2>{}))>::value, "");
        static_assert(cnl::_impl::depth<elastic_integer<0>>::value == 1, "");
        static_assert(cnl::_impl::depth<elastic_integer<1>>::value == 1, "");
    }

    namespace test_digits {
        using cnl::digits;

        static_assert(digits<elastic_integer<7, int>>::value==7, "elastic_integer test failed");
    }

    namespace test_operate {
        using cnl::_impl::is_derived_from_number_base;
        using cnl::_impl::equal_op;

        static_assert(cnl::_impl::enable_binary<double, cnl::elastic_integer<31, int>>::value,
                "failed to meet conditions for applying operator*(double, cnl::elastic_integer)");
        static_assert(is_derived_from_number_base<elastic_integer<1>>::value,
                "cnl::_impl::is_derived_from_number_base test failed");
        static_assert(
                cnl::_impl::binary_operator<cnl::_impl::greater_than_op, elastic_integer<31>, elastic_integer<31>>()(
                        elastic_integer<31>{0x7fffffff}, elastic_integer<31>{cnl::numeric_limits<cnl::int32>::min()}),
                "cnl::elastic_integer test failed");
        static_assert(
                cnl::_impl::binary_operator<cnl::_impl::greater_than_op, elastic_integer<31>, elastic_integer<31>>()(
                        elastic_integer<31>{0x7fffffff}, elastic_integer<31>{cnl::numeric_limits<cnl::int32>::min()}),
                "cnl::elastic_integer test failed");
        static_assert(identical(
                typename std::common_type<elastic_integer<31>, elastic_integer<32, unsigned>>::type{},
                elastic_integer<32>{}),
                "cnl::elastic_integer test failed");
        static_assert(cnl::_impl::less_than_op()(elastic_integer<31, unsigned>{-1},
                elastic_integer<31>{1}),
                "cnl::elastic_integer test failed");
        static_assert(cnl::_impl::binary_operator<cnl::_impl::equal_op, elastic_integer<1>, std::int32_t>()(
                elastic_integer<1>{0}, INT32_C(0)),
                "cnl::elastic_integer test failed");
        static_assert(identical(
                cnl::_impl::binary_operator<cnl::_impl::multiply_op, elastic_integer<1>, std::int32_t>()(
                        elastic_integer<1>{0}, INT32_C(0)), elastic_integer<31, int>{0}),
                "cnl::elastic_integer test failed");

        static_assert(cnl::_impl::binary_operator<equal_op, elastic_integer<8>, elastic_integer<8>>()(
                elastic_integer<8>{1L}, elastic_integer<8>{1}),
                "elastic_integer comparison test failed");
        static_assert(cnl::_impl::binary_operator<equal_op, elastic_integer<12, cnl::uint16>, int>()(
                elastic_integer<12, cnl::uint16>{1324}, 1324),
                "cnl::elastic_integer test failed");
        static_assert(cnl::_impl::binary_operator<equal_op, elastic_integer<12, cnl::uint16>, int>()(
                ~elastic_integer<12, cnl::uint16>{0}, 0xFFF),
                "cnl::elastic_integer test failed");
    }

#if defined(__cpp_deduction_guides)
    namespace test_deduction_guides {
        static_assert(identical(elastic_integer{128_c}, elastic_integer<8>{128}), "elastic_integer test failed");
        static_assert(identical(elastic_integer{127_c}, elastic_integer<7>{127}), "elastic_integer test failed");
        static_assert(identical(elastic_integer{-128_c}, elastic_integer<8>{-128}), "elastic_integer test failed");
        static_assert(identical(elastic_integer{-127_c}, elastic_integer<7>{-127}), "elastic_integer test failed");
    }
#endif

    namespace test_to_rep {
        static_assert(cnl::_impl::equal_op()(1, to_rep(elastic_integer<8>{1})), "");
    }

    namespace test_impl_make_number {
        static_assert(identical(
                elastic_integer<cnl::digits<int>::value>{14},
                cnl::_impl::make_number<elastic_integer<>>(14)), "cnl::_impl::make_number<elastic_integer> test failed");
        static_assert(
                identical(
                elastic_integer<cnl::digits<int>::value>{22},
                        cnl::_impl::make_number<elastic_integer<>>(elastic_integer<>{22})),
                "cnl::_impl::make_number<elastic_integer> test failed");

        static_assert(identical(cnl::_impl::make_number<elastic_integer<>>(0_c), elastic_integer<0>{0}),
                "cnl::_impl::make_number<elastic_integer> test failed");
        static_assert(identical(cnl::_impl::make_number<elastic_integer<>>(1_c), elastic_integer<1>{1}),
                "cnl::_impl::make_number<elastic_integer> test failed");
        static_assert(identical(cnl::_impl::make_number<elastic_integer<>>(14_c), elastic_integer<4>{14}),
                "cnl::_impl::make_number<elastic_integer> test failed");
        static_assert(identical(cnl::_impl::make_number<elastic_integer<>>(-31_c), elastic_integer<5>{-31}),
                "cnl::_impl::make_number<elastic_integer> test failed");
        static_assert(identical(cnl::_impl::make_number<elastic_integer<>>(-32_c), elastic_integer<6>{-32}),
                "cnl::_impl::make_number<elastic_integer> test failed");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(
                cnl::_impl::make_number<elastic_integer<0>>(-0x10000000000000000000000000000000_c),
                elastic_integer<125>{-CNL_INTMAX_C(0x10000000000000000000000000000000)}),
                "cnl::_impl::make_number<elastic_integer> test failed");
#endif
    }

    namespace test_from_value {
        using cnl::from_value;

        static_assert(std::is_same<elastic_integer<7, int>::rep, int>::value, "");
        static_assert(identical(
                elastic_integer<cnl::numeric_limits<int>::digits>{1},
                cnl::_impl::make_number<elastic_integer<3>>(1)), "elastic_integer test failed");
        static_assert(identical(cnl::_impl::make_number<elastic_integer<3>>(1),
                elastic_integer<cnl::numeric_limits<int>::digits>{1}), "elastic_integer test failed");
        static_assert(identical(cnl::_impl::make_number<elastic_integer<1>>(INT32_C(0)), elastic_integer<31>{0}), "cnl::elastic_integer test failed");

        static_assert(identical(
                cnl::elastic_integer<3, signed>{6},
                cnl::_impl::make_number<cnl::elastic_integer<2, unsigned>>(cnl::constant<6>{})),
                "from_value<elastic_integer, constant>");
        static_assert(identical(
                cnl::elastic_integer<3, signed>{6},
                cnl::_impl::make_number<cnl::elastic_integer<2, signed>>(cnl::constant<6>{})),
                "from_value<elastic_integer, constant>");
    }

    namespace test_make_unsigned {
        static_assert(
                assert_same<
                        cnl::elastic_integer<39, unsigned int>,
                        typename cnl::make_unsigned<
                                cnl::elastic_integer<39, unsigned int>>::type>::value, "");
    }

    namespace test_set_signedness {
        static_assert(
                assert_same<
                        cnl::elastic_integer<39, unsigned int>,
                        typename cnl::_impl::set_signedness<
                                cnl::elastic_integer<39, unsigned int>, false>::type>::value, "");
    }

    namespace test_set_signedness_t {
        static_assert(
                assert_same<
                        cnl::elastic_integer<39, unsigned int>,
                        cnl::_impl::set_signedness_t<
                                cnl::elastic_integer<39, unsigned int>, false>>::value, "");
    }

    namespace test_ctor {
        static_assert(identical(elastic_integer<8>{1L}, elastic_integer<8>{1}), "elastic_integer test failed");

        static_assert(identical(elastic_integer<4>{10_c}, elastic_integer<4>{10}), "");
#if defined(__cpp_deduction_guides)
        static_assert(identical(elastic_integer{cnl::constant<1000>{}}, elastic_integer<10>{1000}), "");
#endif
        static_assert(identical(elastic_integer<10>{cnl::constant<1000>{}}, elastic_integer<10>{1000}), "");
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
        static_assert(elastic_integer<4, signed>{10} > elastic_integer<4, signed>{-7}, "elastic_integer comparison test failed");
    }

    namespace test_unary_subtract {
        static_assert(identical(-elastic_integer<1, unsigned>{1}, elastic_integer<1, signed>{-1}), "elastic_integer test failed");
        static_assert(identical(-elastic_integer<8, unsigned>{432}, elastic_integer<8, signed>{-432}), "operator- elastic_integer test failed");
    }

    namespace test_unary_add {
        static_assert(identical(+elastic_integer<1, unsigned>{1}, elastic_integer<1, unsigned>{1}), "elastic_integer test failed");
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

    namespace test_modulo {
        static_assert(identical(elastic_integer<10>{7}, elastic_integer<10>{777}%elastic_integer<4>{10}),
                "cnl::elastic_integer test failed");
        static_assert(identical(elastic_integer<5>{2}, elastic_integer<5>{32}%3),
                "cnl::elastic_integer test failed");
        static_assert(identical(elastic_integer<10>{7}, elastic_integer<10>{777}%10_c),
                "cnl::elastic_integer test failed");
    }

    namespace test_bitwise_not {
        static_assert(~elastic_integer<12, cnl::uint16>{0}==0xFFF, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<12, int>{0x050}==~0x50, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<12, unsigned>{0}==0xFFFU, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<7, cnl::int8>{0x5a}==~0x5a, "cnl::elastic_integer test failed");
        static_assert(~elastic_integer<50, cnl::int64>{INT64_C(0x987654321)}==~INT64_C(0x987654321), "cnl::elastic_integer test failed");
    }

    namespace test_multiply {
        using cnl::make_elastic_integer;

        static_assert(identical(
                elastic_integer<15, int>{0x7fff}*elastic_integer<16, unsigned>{0xffff},
                elastic_integer<31, int>{0x7FFE8001}), "cnl::elastic_integer test failed");
        static_assert(identical(
                elastic_integer<31, int>{0x7fffffff}*elastic_integer<32, int>{0xffffffffu},
                elastic_integer<63, int>{0x7FFFFFFE80000001LL}), "cnl::elastic_integer test failed");

        static_assert(identical(
                cnl::_impl::binary_operator<cnl::_impl::multiply_op, elastic_integer<1>, std::int32_t>()(
                        elastic_integer<1>{0}, INT32_C(0)),
                cnl::elastic_integer<31, int>{0}),
                "cnl::elastic_integer test failed");
        static_assert(identical(to_rep(elastic_integer<4>{13}), 13), "cnl::elastic_integer test failed");
        static_assert(identical(elastic_integer<1>{0} * INT32_C(0), cnl::elastic_integer<31, int>{0}),
                      "cnl::elastic_integer test failed");
        static_assert(identical(make_elastic_integer(177_c), cnl::elastic_integer<8, int>{177}),
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
#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::elastic_integer<73>{1024}, cnl::_impl::scale<10>(cnl::elastic_integer<63>{1})), "scale<elastic_integer> test failed");
#endif
        static_assert(identical(elastic_integer<8>{220}, cnl::scale<2, 2, elastic_integer<6>>{}(55)), "scale<elastic_integer> test failed");
    }

    namespace test_shift_right {
        static_assert(identical(
                elastic_integer<1>{1},
                cnl::_impl::shift_right<cnl::_impl::nearest_rounding_tag, cnl::elastic_integer<2>, cnl::constant<1>>{}(
                        elastic_integer<2>{2}, {})), "shift_right<elastic_integer>");
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

    namespace test_avg_fn {
        constexpr int avg(int a, int b) {
#if defined(__cpp_deduction_guides)
            return int((cnl::elastic_integer{a} + cnl::elastic_integer{b}) / 2);
#else
            return int((cnl::make_elastic_integer(a) + cnl::make_elastic_integer(b)) / 2);
#endif
        }

        static_assert(avg(INT_MAX, INT_MAX) == INT_MAX, "avg using elastic_integer");
        static_assert(avg(INT_MAX, 0) == INT_MAX / 2, "avg using elastic_integer");
        static_assert(avg(INT_MAX, -INT_MAX) == 0, "avg using elastic_integer");
        static_assert(avg(INT_MAX, INT_MIN) == 0, "avg using elastic_integer");

        static_assert(avg(0, INT_MAX) == INT_MAX / 2, "avg using elastic_integer");
        static_assert(avg(0, 0) == 0, "avg using elastic_integer");
        static_assert(avg(0, -INT_MAX) == -INT_MAX / 2, "avg using elastic_integer");
        static_assert(avg(0, INT_MIN) == INT_MIN / 2, "avg using elastic_integer");

        static_assert(avg(-INT_MAX, INT_MAX) == 0, "avg using elastic_integer");
        static_assert(avg(-INT_MAX, 0) == -INT_MAX / 2, "avg using elastic_integer");
        static_assert(avg(-INT_MAX, -INT_MAX) == -INT_MAX, "avg using elastic_integer");
        static_assert(avg(-INT_MAX, INT_MIN) == -INT_MAX, "avg using elastic_integer");

        static_assert(avg(INT_MIN, INT_MAX) == 0, "avg using elastic_integer");
        static_assert(avg(INT_MIN, 0) == INT_MIN / 2, "avg using elastic_integer");
        static_assert(avg(INT_MIN, -INT_MAX) == -INT_MAX, "avg using elastic_integer");
        static_assert(avg(INT_MIN, INT_MIN) == INT_MIN, "avg using elastic_integer");
    }

    // parameterized tests
    template<typename ElasticInteger, long long Lowest, long long Min, long long Max>
    struct elastic_integer_test;

    template<int Digits, typename Narrowest, long long Lowest, long long Min, long long Max>
    struct elastic_integer_test<elastic_integer<Digits, Narrowest>, Lowest, Min, Max> {
        using value_type = elastic_integer<Digits, Narrowest>;
        using narrowest = Narrowest;
        using numeric_limits = cnl::numeric_limits<value_type>;

        static constexpr value_type lowest{Lowest};
        static constexpr value_type min{Min};
        static constexpr value_type max{Max};

        ////////////////////////////////////////////////////////////////////////////////
        // members

        static constexpr int digits = cnl::digits<value_type>::value;
        static constexpr bool is_signed = numeric_limits::is_signed;
        static_assert(is_signed==cnl::numeric_limits<narrowest>::is_signed, "narrowest is different signedness");

        ////////////////////////////////////////////////////////////////////////////////
        // type traits

        static_assert(is_signed==cnl::numeric_limits<typename value_type::rep>::is_signed,
                "signage of narrowest and rep should be the same");
        static_assert(!is_signed || numeric_limits::max()==-numeric_limits::lowest(), "type has most negative number");
        static_assert(!is_signed || -numeric_limits::max()==numeric_limits::lowest(), "type has most negative number");

        static_assert(identical(cnl::_impl::set_signedness_t<value_type, is_signed>{0}, value_type{0}), "");

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

    namespace test_used_digits {
        static_assert(cnl::used_digits(elastic_integer<7>{3})==2, "used_digits test failed");
        static_assert(cnl::used_digits(elastic_integer<12, cnl::uint16>{10})==4, "used_digits test failed");
    }

    namespace test_leading_bits {
        using cnl::leading_bits;

        static_assert(leading_bits(elastic_integer<7>{3})==5, "leading_bits test failed");
        static_assert(leading_bits(elastic_integer<12, cnl::uint16>{10})==8, "leading_bits test failed");
    }

    namespace test_power {
        static_assert(identical(cnl::elastic_integer<11>{1024}, cnl::power<cnl::elastic_integer<1>, 10, 2>()),
        "cnl::power test failed");
    }

    namespace test_shift {
        static_assert(identical(cnl::elastic_integer<11>{1024}, cnl::_impl::scale<10, 2>(cnl::elastic_integer<1>(1))), "");
        static_assert(identical(cnl::elastic_integer<3>{6}, cnl::_impl::scale<1, 2>(cnl::elastic_integer<2>{3})), "");
    }

    namespace test_shift_left {
        using namespace cnl::literals;

        // by int
        static_assert(identical(elastic_integer<9>{14} << 4, elastic_integer<9>{14 << 4}), "shift_left test failed");
#if defined(__cpp_binary_literals)
        static_assert(identical(
                elastic_integer<5+34, unsigned>{0b11001110101011101001LL << 34},
                elastic_integer<5+34, unsigned>{0b11001110101011101001} << 34), "shift_left test failed");
#endif

        // by cnl::constant
        static_assert(identical(elastic_integer<5>{14} << 4_c, elastic_integer<9>{14 << 4}), "shift_left test failed");
#if defined(__cpp_binary_literals)
        static_assert(identical(
                elastic_integer<20+34, unsigned>{0b11001110101011101001LL << 34},
                elastic_integer<20, unsigned>{0b11001110101011101001} << 34_c),
                "shift_left test failed");
#endif
        static_assert(identical(elastic_integer<33>{1LL<<32}, cnl::_impl::scale<32>(elastic_integer<1>{1})), "cnl::_impl::scale<32, elastic_integer<1>>");
    }

    namespace test_shift_right {
        using namespace cnl::literals;

        // by int
        static_assert(identical(elastic_integer<9>{14 << 4} >> 4, elastic_integer<9>{14}), "shift_left test failed");
#if defined(__cpp_binary_literals)
        static_assert(identical(
                elastic_integer<5+34, unsigned>{0b11001110101011101001},
                elastic_integer<5+34, unsigned>{0b11001110101011101001LL << 34} >> 34), "shift_left test failed");
#endif

        // by cnl::constant
        static_assert(identical(elastic_integer<9>{14 << 4} >> 4_c, elastic_integer<5>{14}), "shift_left test failed");
#if defined(__cpp_binary_literals)
        static_assert(identical(
                elastic_integer<20, unsigned>{0b11001110101011101001},
                elastic_integer<20+34, unsigned>{0b11001110101011101001LL << 34} >> 34_c),
                "shift_left test failed");
#endif
    }

    TEST(elastic_integer, to_rep_ref) {
        auto i = 123;
        auto e = cnl::elastic_integer<10>{i};
        int& expected = i;
        auto equal = identical(expected, cnl::_impl::to_rep(e));
        ASSERT_TRUE(equal);
    }

    TEST(elastic_integer, to_rep_const_ref) {
        auto i = 123;
        auto const e = cnl::elastic_integer<10>{i};
        int const& expected = i;
        auto equal = identical(expected, cnl::_impl::to_rep(e));
        ASSERT_TRUE(equal);
    }

    TEST(elastic_integer, to_rep_rvalue_ref) {
        auto i = 123;
        auto equal = identical(i, cnl::_impl::to_rep(cnl::elastic_integer<10>{i}));
        ASSERT_TRUE(equal);
    }

    TEST(elastic_integer, pre_increment) {
        auto i = 123;
        auto a = cnl::elastic_integer<10>{i};
        auto& b = ++a;
        ASSERT_EQ(a, i+1);
        ASSERT_EQ(b, i+1);
        ASSERT_EQ(&a, &b);
        static_assert(std::is_same<cnl::elastic_integer<10>&, decltype(b)>::value, "");
    }

    TEST(elastic_integer, pre_decrement) {
        auto i = 123;
        auto a = cnl::elastic_integer<10>{i};
        auto& b = --a;
        ASSERT_EQ(a, i-1);
        ASSERT_EQ(b, i-1);
        ASSERT_EQ(&a, &b);
        static_assert(std::is_same<cnl::elastic_integer<10>&, decltype(b)>::value, "");
    }

    TEST(elastic_integer, post_increment) {
        auto i = 123;
        auto a = cnl::elastic_integer<10>{i};
        auto b = a++;
        ASSERT_EQ(a, i+1);
        ASSERT_EQ(b, i);
        ASSERT_NE(&a, &b);
        static_assert(std::is_same<cnl::elastic_integer<10>, decltype(b)>::value, "");
    }

    TEST(elastic_integer, post_decrement) {
        auto i = 123;
        auto a = cnl::elastic_integer<10>{i};
        auto b = a--;
        ASSERT_EQ(a, i-1);
        ASSERT_EQ(b, i);
        ASSERT_NE(&a, &b);
        static_assert(std::is_same<cnl::elastic_integer<10>, decltype(b)>::value, "");
    }
}
