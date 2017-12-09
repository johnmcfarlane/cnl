
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/precise_integer.h>

#include "../number_test.h"

#include <gtest/gtest.h>

namespace {
    using cnl::precise_integer;
    using std::is_same;
    using cnl::_impl::identical;

    namespace number_base_traits {
        using cnl::_impl::number_base;
        using cnl::_impl::is_derived_from_number_base;

        static_assert(!is_derived_from_number_base<int>::value, "");
        static_assert(!is_derived_from_number_base<number_base<precise_integer<>, int>>::value, "");
        static_assert(is_derived_from_number_base<precise_integer<>>::value, "");
    }

    namespace default_parameters {
        using cnl::closest_rounding_tag;

        using default_rep = int;

        template<typename T>
        using default_tag = cnl::closest_rounding_tag;

        static_assert(is_same<precise_integer<>, precise_integer<default_rep, default_tag<default_rep>>>::value, "cnl::precise_integer parameter default test failed");

        static_assert(is_same<precise_integer<>::rep, default_rep>::value, "cnl::precise_integer parameter default test failed");
        static_assert(is_same<precise_integer<>::rounding, default_tag<default_rep>>::value, "cnl::precise_integer parameter default test failed");
    }

    namespace is_number {
        using cnl::_impl::is_derived_from_number_base;

        static_assert(is_derived_from_number_base<precise_integer<>>::value, "is_derived_from_number_base<precise_integer<>>");
    }

    namespace test_traits {

        namespace test_make_signed_t {
            using cnl::make_signed_t;

            static_assert(std::is_same<precise_integer<int>, make_signed_t<precise_integer<int>>>::value,
                          "cnl::make_signed_t<cnl::precise_integer<>>");
            static_assert(std::is_same<precise_integer<short>, make_signed_t<precise_integer<unsigned short>>>::value,
                          "cnl::make_signed_t<cnl::precise_integer<>>");
        }

        namespace test_make_unsigned_t {
            using cnl::make_unsigned_t;

            static_assert(
                    std::is_same<precise_integer<unsigned char>, make_unsigned_t<precise_integer<unsigned char>>>::value,
                    "cnl::make_unsigned_t<cnl::precise_integer<>>");
            static_assert(
                    std::is_same<precise_integer<unsigned long long>, make_unsigned_t<precise_integer<long long>>>::value,
                    "cnl::make_unsigned_t<cnl::precise_integer<>>");
        }

        namespace test_to_rep {
            using cnl::_impl::to_rep;

            static_assert(
                    identical(123, to_rep(precise_integer<>{123})),
                    "cnl::to_rep<precise_integer> test failed");
        }

        namespace test_from_rep {
            using cnl::_impl::from_rep;

            static_assert(
                    identical(precise_integer<>{2468}, from_rep<precise_integer<>>(2468)),
                    "cnl::from_rep<precise_integer> test failed");
        }

        namespace test_from_value {
            using cnl::_impl::from_value;

            static_assert(identical(precise_integer<long long>{9876543210LL}, from_value<precise_integer<long>>(9876543210LL)),
                          "cnl::from_value<precise_integer> test failed");
        }
    }
    
    namespace test_operate {
        static_assert(
                cnl::_impl::comparison_operator<cnl::_impl::equal_op, precise_integer<>, precise_integer<>>()(precise_integer<>{2468}, precise_integer<>{2468}),
                "cnl::numeric_traits<precise_integer> test failed");
        static_assert(
                cnl::_impl::comparison_operator<cnl::_impl::equal_op, int, precise_integer<>>()(2468, precise_integer<>{2468}),
                "cnl::numeric_traits<precise_integer> test failed");
        static_assert(
                cnl::_impl::comparison_operator<cnl::_impl::greater_than_op, precise_integer<>, int>()(precise_integer<>{234}, 233),
                "cnl::numeric_traits<precise_integer> test failed");
    }

    namespace test_comparison {
        static_assert(
                identical(precise_integer<>{2468}, precise_integer<>{2468}),
                "cnl::numeric_traits<precise_integer> test failed");
    }

    namespace test_conversion_operator {
        static_assert(identical(static_cast<int>(precise_integer<>{9876}), 9876), "precise_integer conversion test failed");
    }

    namespace test_closest_rounding_tag {
        using cnl::closest_rounding_tag;
        static_assert(identical(closest_rounding_tag::convert<int>(0.), 0), "cnl::closest_rounding_tag test failed");
        static_assert(identical(closest_rounding_tag::convert<int>(-1.), -1), "cnl::closest_rounding_tag test failed");
    }

    namespace closest {
        using precise_integer = cnl::precise_integer<>;

        static_assert(precise_integer{0.} == 0, "cnl::precise_integer test failed");
        static_assert(precise_integer{1.} == 1, "cnl::precise_integer test failed");
        static_assert(precise_integer{-1.} == -1, "cnl::precise_integer test failed");
        static_assert(precise_integer{.5} == 1, "cnl::precise_integer test failed");
        static_assert(precise_integer{-.5} == -1, "cnl::precise_integer test failed");
        static_assert(precise_integer{0.499} == 0, "cnl::precise_integer test failed");
        static_assert(precise_integer{-0.501} == -1, "cnl::precise_integer test failed");
    }

    namespace arithmetic {
        static_assert(identical(precise_integer<>{3}*precise_integer<>{7}, precise_integer<>{21}),
                "precise_integer operator*");
    }
    
    namespace numeric_limits {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::numeric_limits
        
        // cnl::numeric_limits<cnl::precise_integer<>>::is_integer
        static_assert(cnl::numeric_limits<cnl::precise_integer<int8_t, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<uint8_t, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<int16_t, cnl::closest_rounding_tag> const>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<uint16_t, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<int32_t, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<uint32_t, cnl::closest_rounding_tag> const>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<int64_t, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<uint64_t, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::numeric_limits<cnl::int128>::is_specialized,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<cnl::int128, cnl::closest_rounding_tag>>::is_specialized,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<cnl::int128, cnl::closest_rounding_tag>>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<cnl::uint128, cnl::closest_rounding_tag> const>::is_integer,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
#endif

        // cnl::numeric_limits<cnl::precise_integer<>>::is_signed
        static_assert(cnl::numeric_limits<cnl::precise_integer<int8_t, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!cnl::numeric_limits<cnl::precise_integer<uint8_t, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<int16_t, cnl::closest_rounding_tag> const>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!cnl::numeric_limits<cnl::precise_integer<uint16_t, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<int32_t, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!cnl::numeric_limits<cnl::precise_integer<uint32_t, cnl::closest_rounding_tag> const>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<cnl::precise_integer<int64_t, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!cnl::numeric_limits<cnl::precise_integer<uint64_t, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(cnl::numeric_limits<cnl::precise_integer<cnl::int128, cnl::closest_rounding_tag> const>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!cnl::numeric_limits<cnl::precise_integer<cnl::uint128, cnl::closest_rounding_tag>>::is_signed,
                "cnl::numeric_limits<cnl::precise_integer<>> test failed");
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // std::numeric_limits

        // std::numeric_limits<cnl::precise_integer<>>::is_integer
        static_assert(std::numeric_limits<cnl::precise_integer<int8_t, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint8_t, cnl::closest_rounding_tag> const>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int16_t, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint16_t, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int32_t, cnl::closest_rounding_tag> const>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint32_t, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int64_t, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint64_t, cnl::closest_rounding_tag> const>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(std::numeric_limits<cnl::precise_integer<cnl::int128, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<cnl::uint128, cnl::closest_rounding_tag>>::is_specialized,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
#endif

        // std::numeric_limits<cnl::precise_integer<>>::is_integer
        static_assert(std::numeric_limits<cnl::precise_integer<int8_t, cnl::closest_rounding_tag> const>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint8_t, cnl::closest_rounding_tag>>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int16_t, cnl::closest_rounding_tag>>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint16_t, cnl::closest_rounding_tag> const>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int32_t, cnl::closest_rounding_tag>>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint32_t, cnl::closest_rounding_tag>>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int64_t, cnl::closest_rounding_tag> const>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<uint64_t, cnl::closest_rounding_tag>>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(std::numeric_limits<cnl::precise_integer<cnl::int128, cnl::closest_rounding_tag>>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<cnl::uint128, cnl::closest_rounding_tag> const>::is_integer,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
#endif

        // std::numeric_limits<cnl::precise_integer<>>::is_signed
        static_assert(std::numeric_limits<cnl::precise_integer<int8_t, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!std::numeric_limits<cnl::precise_integer<uint8_t, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int16_t, cnl::closest_rounding_tag> const>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!std::numeric_limits<cnl::precise_integer<uint16_t, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int32_t, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!std::numeric_limits<cnl::precise_integer<uint32_t, cnl::closest_rounding_tag> const>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(std::numeric_limits<cnl::precise_integer<int64_t, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!std::numeric_limits<cnl::precise_integer<uint64_t, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
#if defined(CNL_INT128_ENABLED)
        static_assert(std::numeric_limits<cnl::precise_integer<cnl::int128, cnl::closest_rounding_tag> const>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
        static_assert(!std::numeric_limits<cnl::precise_integer<cnl::uint128, cnl::closest_rounding_tag>>::is_signed,
                "std::numeric_limits<cnl::precise_integer<>> test failed");
#endif
    }
}

template<class PreciseInteger>
struct precise_integer_tests {
    // from_rep
    static_assert(identical(precise_integer<>{123}, cnl::_impl::from_rep<precise_integer<>>(123)),
                  "cnl::_impl::from_rep<precise_integer<>> test failed");

    // to_rep
    static_assert(identical(123, cnl::_impl::to_rep(123)), "cnl::_impl::to_rep test failed");
    static_assert(identical(321, cnl::_impl::to_rep(precise_integer<>{321})), "cnl::_impl::to_rep test failed");
};

template struct number_test_by_rep_by_tag<precise_integer, cnl::closest_rounding_tag, precise_integer_tests>;
