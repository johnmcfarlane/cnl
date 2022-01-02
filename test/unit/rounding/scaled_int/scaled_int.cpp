
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding.h>
#include <cnl/scaled_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <type_traits>

using cnl::_impl::identical;

namespace {
    namespace test_set_rounding {
        using expected = cnl::scaled_integer<int, cnl::power<-1>>;
        using number = cnl::scaled_integer<int, cnl::power<-1>>;
        using rounding_tag = cnl::native_rounding_tag;
        using actual = typename cnl::set_rounding<number, rounding_tag>::type;
        static_assert(std::is_same_v<expected, actual>);
    }

    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<-1>>{0.5};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::scaled_integer<int, cnl::power<-1>>>{}(
                cnl::scaled_integer<int, cnl::power<-4>>{0.3125});

        static_assert(identical(expected, actual));
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<-2>>{-0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::scaled_integer<int, cnl::power<-2>>>{}(
                cnl::scaled_integer<int, cnl::power<-4>>{-0.3125});

        static_assert(identical(expected, actual));
    }

    namespace test_nearest_round_up_float {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<-2>>{-0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::scaled_integer<int, cnl::power<-2>>>{}(-0.3125F);

        static_assert(identical(expected, actual));
    }

    namespace test_truncate_round_up {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<>>{1};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::scaled_integer<int, cnl::power<>>>{}(
                cnl::scaled_integer<int, cnl::power<-2>>{0.75});

        static_assert(identical(expected, actual));  // Fails gives 0.75 as result.
    }

    namespace test_nearest_int {
        static_assert(
                identical(
                        3, cnl::convert<cnl::nearest_rounding_tag, int, cnl::power<>>{}(
                                   cnl::scaled_integer<int, cnl::power<-2>>{2.5})));
        static_assert(identical(
                2, cnl::convert<cnl::nearest_rounding_tag, int, cnl::power<>>{}(
                           cnl::scaled_integer<int, cnl::power<-2>>{2.4375})));
    }

    namespace test_nearest_from_int {
        static_assert(identical(
                cnl::scaled_integer<int, cnl::power<2>>{4},
                cnl::convert<cnl::nearest_rounding_tag, cnl::scaled_integer<int, cnl::power<2>>, cnl::power<>>{}(
                        5)));
        static_assert(identical(
                cnl::scaled_integer<int, cnl::power<2>>{8},
                cnl::convert<cnl::nearest_rounding_tag, cnl::scaled_integer<int, cnl::power<2>>, cnl::power<>>{}(
                        6)));
    }

    namespace test_native_int {
        static_assert(
                identical(
                        2, cnl::custom_operator<
                                   cnl::_impl::convert_op,
                                   cnl::op_value<cnl::scaled_integer<int, cnl::power<-2>>, cnl::_impl::native_tag>,
                                   cnl::op_value<int, cnl::native_rounding_tag>>{}(2.5)));
        static_assert(identical(
                2, cnl::convert<cnl::native_rounding_tag, int>{}(
                           cnl::scaled_integer<int, cnl::power<-2>>{2.4375})));
    }

    namespace test_native_from_int {
        static_assert(identical(
                cnl::scaled_integer<int, cnl::power<2>>{4},
                cnl::convert<
                        cnl::native_rounding_tag,
                        cnl::scaled_integer<int, cnl::power<2>>>{}(5)));
        static_assert(identical(
                cnl::scaled_integer<int, cnl::power<2>>{4},
                cnl::convert<
                        cnl::native_rounding_tag,
                        cnl::scaled_integer<int, cnl::power<2>>>{}(6)));
    }

    namespace test_convert_nearest_rounding_elastic_number {
        static constexpr auto a = cnl::scaled_integer<int, cnl::power<-4>>{0.3125};
        static constexpr auto b = cnl::custom_operator<
                cnl::_impl::convert_op,
                cnl::op_value<cnl::scaled_integer<int, cnl::power<-4>>, cnl::native_rounding_tag>,
                cnl::op_value<cnl::scaled_integer<int, cnl::power<-1>>, cnl::nearest_rounding_tag>>{}(a);
        static_assert(
                identical(cnl::scaled_integer<int, cnl::power<-1>>{0.5}, b));
    }
}
