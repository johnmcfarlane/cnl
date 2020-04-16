
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding.h>
#include <cnl/scaled_integer.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_set_rounding {
        using expected = cnl::scaled_integer<int, cnl::power<-1> >;
        using number = cnl::scaled_integer<int, cnl::power<-1> >;
        using rounding_tag = cnl::native_rounding_tag;
        using actual = typename cnl::set_rounding<number, rounding_tag>::type;
        static_assert(
                assert_same<expected, actual>::value,
                "set_rounding<scaled_integer, native_rounding_tag>");
    }

    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<-1>>{0.5};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::_impl::native_tag,
                cnl::scaled_integer<int, cnl::power<-1>>>(cnl::scaled_integer<int, cnl::power<-4>>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::scaled_integer, cnl::scaled_integer>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<-2>>{-0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::_impl::native_tag,
                cnl::scaled_integer<int, cnl::power<-2>>>(cnl::scaled_integer<int, cnl::power<-4>>{-0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::scaled_integer, cnl::scaled_integer>");
    }

    namespace test_nearest_round_up_float {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<-2>>{-0.25};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::_impl::native_tag,
                cnl::scaled_integer<int, cnl::power<-2>>>(-0.3125F);

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::scaled_integer, cnl::scaled_integer>");
    }

    namespace test_truncate_round_up {
        static constexpr auto expected = cnl::scaled_integer<int, cnl::power<>>{1};
        static constexpr auto actual = cnl::convert<
                cnl::nearest_rounding_tag,
                cnl::_impl::native_tag,
                cnl::scaled_integer<int, cnl::power<>>>(cnl::scaled_integer<int, cnl::power<-2>>{0.75});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag>"); //Fails gives 0.75 as result.
    }

    namespace test_nearest_int {
        static_assert(
                identical(
                        3,
                        cnl::convert<
                                cnl::nearest_rounding_tag,
                                cnl::power<>,
                                int>(cnl::scaled_integer<int, cnl::power<-2>>{2.5})),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int, cnl::scaled_integer>");
        static_assert(
                identical(
                        2,
                        cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int>(
                                cnl::scaled_integer<int, cnl::power<-2>>{2.4375})),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, int, cnl::scaled_integer>");
    }

    namespace test_nearest_from_int {
        static_assert(
                identical(
                        cnl::scaled_integer<int, cnl::power<2>>{4},
                        cnl::convert<
                                cnl::nearest_rounding_tag,
                                cnl::power<>,
                                cnl::scaled_integer<int, cnl::power<2>>>(5)),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::scaled_integer, int>");
        static_assert(
                identical(
                        cnl::scaled_integer<int, cnl::power<2>>{8},
                        cnl::convert<
                                cnl::nearest_rounding_tag,
                                cnl::power<>,
                                cnl::scaled_integer<int, cnl::power<2>>>(6)),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::power<>, cnl::scaled_integer, int>");
    }

    namespace test_native_int {
        static_assert(
                identical(
                        2,
                        cnl::convert_operator<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                int,
                                cnl::scaled_integer<int, cnl::power<-2>>>{}(2.5)),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int, cnl::scaled_integer>");
        static_assert(
                identical(
                        2,
                        cnl::convert<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                int>(cnl::scaled_integer<int, cnl::power<-2>>{2.4375})),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int, cnl::scaled_integer>");
    }

    namespace test_native_from_int {
        static_assert(
                identical(
                        cnl::scaled_integer<int, cnl::power<2>>{4},
                        cnl::convert<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                cnl::scaled_integer<int, cnl::power<2>>>(5)),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::scaled_integer, int>");
        static_assert(
                identical(
                        cnl::scaled_integer<int, cnl::power<2>>{4},
                        cnl::convert<
                                cnl::native_rounding_tag,
                                cnl::_impl::native_tag,
                                cnl::scaled_integer<int, cnl::power<2>>>(6)),
                "cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::scaled_integer, int>");
    }

    namespace test_convert_nearest_rounding_elastic_number
    {
        static constexpr auto a = cnl::scaled_integer<int, cnl::power<-4>>{0.3125};
        static constexpr auto b = cnl::convert_operator<
                cnl::nearest_rounding_tag,
                cnl::native_rounding_tag,
                cnl::scaled_integer<int, cnl::power<-1>>,
                cnl::scaled_integer<int, cnl::power<-4>>>{}(a);
        static_assert(identical(cnl::scaled_integer<int, cnl::power<-1>>{0.5}, b),
                "convert_operator<native_rounding_tag, nearest_rounding_tag, scaled_integer, scaled_integer>");
    }
}
