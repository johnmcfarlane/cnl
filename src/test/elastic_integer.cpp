
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `sg14::elastic_integer` type

#include <sg14/auxiliary/elastic_integer.h>

namespace {
    using std::is_same;
    using sg14::_width_type;
    using sg14::elastic_integer;
    using sg14::_impl::identical;
    using namespace sg14::literals;

    // simple one-off tests
    namespace test_operate {
        using sg14::_impl::is_derived_from_number_base;
        using sg14::_impl::equal_tag;
        using sg14::_impl::equal_tag_t;
        using sg14::_impl::multiply_tag;
        using sg14::_impl::op_fn;
        using sg14::_impl::operate;
        using sg14::_impl::precedes;
        using sg14::to_rep;

        static_assert(is_derived_from_number_base<elastic_integer<1>>::value,
                "sg14::_impl::is_derived_from_number_base test failed");
        static_assert(precedes<elastic_integer<1>, int32_t>::value, "");
        static_assert(!precedes<int32_t, elastic_integer<1>>::value, "");
        static_assert(
                operate(elastic_integer<31>{0x7fffffff}, elastic_integer<31>{std::numeric_limits<std::int32_t>::min()}, sg14::_impl::greater_than_tag),
                "sg14::elastic_integer test failed");
        static_assert(identical(
                typename std::common_type<elastic_integer<31>, elastic_integer<32, unsigned>>::type{},
                elastic_integer<32>{}),
                "sg14::elastic_integer test failed");
        static_assert(elastic_integer<31>{-1} < elastic_integer<31, unsigned>{1},
                "sg14::elastic_integer test failed");
        static_assert(sg14::_impl::op_fn<sg14::_impl::less_than_tag_t>(elastic_integer<31, unsigned>{-1},
                elastic_integer<31>{1}),
                "sg14::elastic_integer test failed");
        static_assert(operate(elastic_integer<1>{0}, INT32_C(0), sg14::_impl::equal_tag),
                "sg14::elastic_integer test failed");
        static_assert(identical(
                operate(elastic_integer<1>{0}, INT32_C(0), multiply_tag), elastic_integer<32, int>{0}),
                "sg14::elastic_integer test failed");


        static_assert(
                op_fn<equal_tag_t>(
                        to_rep(static_cast<int>(elastic_integer<8>{1L})),
                        to_rep(static_cast<int>(elastic_integer<8>{1L}))), "");

        static_assert(operate(elastic_integer<8>{1L}, elastic_integer<8>{1}, equal_tag),
                "elastic_integer comparison test failed");
        static_assert(operate(elastic_integer<12, std::uint16_t>{1324}, 1324, equal_tag),
                "sg14::elastic_integer test failed");
        static_assert(operate(~elastic_integer<12, std::uint16_t>{0}, 0xFFF, equal_tag),
                "sg14::elastic_integer test failed");
    }

    namespace test_width {
        static_assert(sg14::width<elastic_integer<7, int>>::value == 8, "elastic_integer test failed");
    }

    namespace test_ctor {
        static_assert(identical(elastic_integer<8>{1L}, elastic_integer<8>{1}), "elastic_integer test failed");
        static_assert(identical(-elastic_integer<1, unsigned>{1}, elastic_integer<1, signed>{-1}), "elastic_integer test failed");

        static_assert(identical(elastic_integer<4>{10_c}, elastic_integer<4>{10}), "");
        static_assert(identical(elastic_integer<10>{sg14::const_integer<unsigned, 1000>{}}, elastic_integer<10>{1000}), "");
    }

    namespace test_is_elastic_integer {
        using sg14::_elastic_integer_impl::is_elastic_integer;
        static_assert(!is_elastic_integer<int>::value, "sg14::_elastic_integer_impl::is_elastic_integer test failed");
        static_assert(is_elastic_integer<elastic_integer<10, int>>::value,
                      "sg14::_elastic_integer_impl::is_elastic_integer test failed");
    }

    namespace test_make_elastic_integer {
        using sg14::make_elastic_integer;
        static_assert(identical(make_elastic_integer(136_c), elastic_integer<8, int>{136}),
                      "sg14::_elastic_integer_impl::make_elastic_integer test failed");
        static_assert(identical(make_elastic_integer(1000000000000_c), elastic_integer<40, int>{1000000000000}),
                      "sg14::_elastic_integer_impl::make_elastic_integer test failed");
    }

    namespace test_subtract {
        // 0-0
        static_assert(
                identical(
                        elastic_integer<5>{0}-elastic_integer<5>{0},
                        elastic_integer<6>{0}),
                "sg14::elastic_integer test failed");

        // unsigned{0}-unsigned{max}
        static_assert(
                identical(
                        elastic_integer<5, unsigned>{0}-std::numeric_limits<elastic_integer<5, unsigned>>::max(),
                        elastic_integer<5, signed>{-31}),
                "sg14::elastic_integer test failed");

        // -signed{max}-unsigned{max}
        static_assert(
                identical(
                        -std::numeric_limits<elastic_integer<7>>::max()-std::numeric_limits<elastic_integer<4, unsigned>>::max(),
                        elastic_integer<8>{-142}),
                "sg14::elastic_integer test failed");

        // unsigned{max}+signed{max}
        static_assert(
                identical(
                        std::numeric_limits<elastic_integer<15, unsigned>>::max()+std::numeric_limits<elastic_integer<19>>::max(),
                        elastic_integer<20>{((1<<15)-1)+((1<<19)-1)}),
                "sg14::elastic_integer test failed");

        // signed{max}+signed{max}
        static_assert(
                identical(
                        std::numeric_limits<elastic_integer<10>>::max()-std::numeric_limits<elastic_integer<9>>::max(),
                        elastic_integer<11>{512}),
                "sg14::elastic_integer test failed");
    }

    namespace test_divide {
        static_assert(identical(elastic_integer<10>{777}/elastic_integer<4>{10}, elastic_integer<10>{77}),
                "sg14::elastic_integer test failed");
        static_assert(identical(elastic_integer<5>{32}/3, elastic_integer<5>{10}),
                "sg14::elastic_integer test failed");
        static_assert(identical(elastic_integer<10>{777}/10_c, elastic_integer<10>{77}),
                "sg14::elastic_integer test failed");
    }

    namespace test_bitwise_not {
        static_assert(~elastic_integer<12, std::uint16_t>{0}==0xFFF, "sg14::elastic_integer test failed");
        static_assert(~elastic_integer<12, int>{0x050}==~0x50, "sg14::elastic_integer test failed");
        static_assert(~elastic_integer<12, unsigned>{0}==0xFFFU, "sg14::elastic_integer test failed");
        static_assert(~elastic_integer<7, std::int8_t>{0x5a}==~0x5a, "sg14::elastic_integer test failed");
        static_assert(~elastic_integer<50, std::int64_t>{INT64_C(0x987654321)}==~INT64_C(0x987654321), "sg14::elastic_integer test failed");
    }

    namespace test_multiply {
        using sg14::make_elastic_integer;
        using sg14::_impl::multiply_tag;

        static_assert(sg14::_impl::precedes<elastic_integer<1>, int32_t>::value, "");
        static_assert(identical(
                sg14::_impl::operate(elastic_integer<1>{0}, INT32_C(0), multiply_tag),
                sg14::elastic_integer<32, int>{0}),
                "sg14::elastic_integer test failed");
        static_assert(identical(elastic_integer<1>{0} * INT32_C(0), sg14::elastic_integer<32, int>{0}),
                      "sg14::elastic_integer test failed");
        static_assert(identical(make_elastic_integer(177_c), sg14::elastic_integer<8, int>{177}),
                      "sg14::elastic_integer test failed");
        static_assert(!sg14::_impl::precedes<sg14::elastic_integer<1>, double>::value,
                "sg14::elastic_integer test failed");
        static_assert(identical(3.*make_elastic_integer(3210), 9630.),
                "sg14::elastic_integer test failed");
#if defined(SG14_INT128_ENABLED)
        static_assert(identical(elastic_integer<1>{0} * INT64_C(0), sg14::elastic_integer<64, int>{0}),
                      "sg14::elastic_integer test failed");
        static_assert(identical(make_elastic_integer(177_c) * INT64_C(9218), sg14::elastic_integer<71, int>{1631586}),
                      "sg14::elastic_integer test failed");
#endif
    }

    // parameterized tests
    template<typename ElasticInteger, long long Lowest, long long Min, long long Max>
    struct elastic_integer_test {
        using value_type = ElasticInteger;
        using narrowest = typename ElasticInteger::narrowest;
        using numeric_limits = std::numeric_limits<value_type>;

        static constexpr value_type lowest{Lowest};
        static constexpr value_type min{Min};
        static constexpr value_type max{Max};

        ////////////////////////////////////////////////////////////////////////////////
        // members

        static constexpr int width = sg14::width<value_type>::value;
        static constexpr int digits = value_type::digits;
        static constexpr bool is_signed = numeric_limits::is_signed;
        static_assert(is_signed==std::numeric_limits<narrowest>::is_signed, "narrowest is different signedness");
        static_assert(width==digits+is_signed, "some of our bits are missing");

        ////////////////////////////////////////////////////////////////////////////////
        // type traits

        static_assert(is_signed==std::numeric_limits<typename value_type::rep>::is_signed,
                "signage of narrowest and rep should be the same");
        static_assert(!is_signed || numeric_limits::max()==-numeric_limits::lowest(), "type has most negative number");
        static_assert(!is_signed || -numeric_limits::max()==numeric_limits::lowest(), "type has most negative number");

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
                std::numeric_limits<decltype(zero)>::is_signed==std::numeric_limits<decltype(zero*zero)>::is_signed,
                "elastic_integer arithmetic test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // numeric_limits

        static_assert(numeric_limits::is_integer, "numeric_limits<elastic_integer<>>::is_integer test failed");
        static_assert(numeric_limits::lowest()==lowest, "numeric_limits<elastic_integer<>>::lowest test failed");
        static_assert(numeric_limits::min()==min, "numeric_limits<elastic_integer<>>::min test failed");
        static_assert(numeric_limits::max()==max, "numeric_limits<elastic_integer<>>::max test failed");
    };

    static_assert(elastic_integer<7, int>{3}==3, "");
    static_assert(std::numeric_limits<int8_t>::lowest()==-128, "");
    static_assert(std::is_same<elastic_integer<7, int>::rep, int>::value, "");
    static_assert(std::numeric_limits<elastic_integer<8, int>>::max()==255, "");
    static_assert(std::numeric_limits<elastic_integer<8, int>>::lowest()==-255, "");

    template
    struct elastic_integer_test<elastic_integer<7, int>, -127, 1, 127>;
    template
    struct elastic_integer_test<elastic_integer<1, int>, -1, 1, 1>;
#if defined(SG14_INT128_ENABLED)
    template
    struct elastic_integer_test<elastic_integer<39, unsigned int>, 0, 1, (INT64_C(1) << 39)-1>;
#endif

    // user-defined literal initialization
#if (__cplusplus > 201402L)
    // with class template deduction
    static_assert(identical(elastic_integer(1_c), elastic_integer<1>{1}), "");
#else
    // without class template deduction
    static_assert(identical(elastic_integer<1>(1_c), elastic_integer<1>{1}), "");
#endif
}
