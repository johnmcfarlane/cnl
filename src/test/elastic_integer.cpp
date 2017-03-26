
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
    static_assert(identical(-elastic_integer<1, unsigned>{1}, elastic_integer<1, signed>{-1}), "elastic_integer test failed");
    static_assert(sg14::width<elastic_integer<7, int>>::value == 8, "elastic_integer test failed");

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

    namespace {
        static_assert(identical(elastic_integer<10>{777} / elastic_integer<4>{10}, elastic_integer<10>{77}),
                      "sg14::elastic_integer test failed");
        static_assert(identical(elastic_integer<10>{777} / 10_c, elastic_integer<10>{77}),
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
        static_assert(identical(elastic_integer<1>{0} * INT32_C(0), sg14::elastic_integer<32, int>{0}),
                      "sg14::elastic_integer test failed");
        static_assert(identical(make_elastic_integer(177_c), sg14::elastic_integer<8, int>{177}),
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

        static constexpr value_type lowest{Lowest};
        static constexpr value_type min{Min};
        static constexpr value_type max{Max};

        ////////////////////////////////////////////////////////////////////////////////
        // members

        static constexpr int width = sg14::width<value_type>::value;
        static constexpr int digits = value_type::digits;
        static constexpr bool is_signed = std::numeric_limits<narrowest>::is_signed;
        static_assert(width==digits+is_signed, "some of our bits are missing");

        ////////////////////////////////////////////////////////////////////////////////
        // type traits

        static_assert(is_signed==std::numeric_limits<typename value_type::rep>::is_signed,
                "signage of narrowest and rep should be the same");

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

        using numeric_limits = std::numeric_limits<value_type>;
        static_assert(numeric_limits::is_integer, "numeric_limits<elastic_integer<>>::is_integer test failed");
        static_assert(numeric_limits::lowest()==lowest, "numeric_limits<elastic_integer<>>::lowest test failed");
        static_assert(numeric_limits::min()==min, "numeric_limits<elastic_integer<>>::min test failed");
        static_assert(numeric_limits::max()==max, "numeric_limits<elastic_integer<>>::max test failed");
    };

    static_assert(elastic_integer<7, int>{3}==3, "");
    static_assert(std::numeric_limits<int8_t>::lowest()==-128, "");
    static_assert(std::is_same<elastic_integer<7, int>::rep, int>::value, "");
    static_assert(std::numeric_limits<elastic_integer<8, int>>::lowest()==-256, "");

    template
    struct elastic_integer_test<elastic_integer<7, int>, -128, 1, 127>;
    template
    struct elastic_integer_test<elastic_integer<1, int>, -2, 1, 1>;
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
