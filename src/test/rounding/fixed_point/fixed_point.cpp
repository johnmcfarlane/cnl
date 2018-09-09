
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/rounding.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::fixed_point<int, -1>{0.5};
        static constexpr auto actual = cnl::_impl::convert<cnl::nearest_rounding_tag, cnl::fixed_point<int, -1>>(
                cnl::fixed_point<int, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::fixed_point, cnl::fixed_point>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::fixed_point<int, -2>{-0.25};
        static constexpr auto actual = cnl::_impl::convert<cnl::nearest_rounding_tag, cnl::fixed_point<int, -2>>(
                cnl::fixed_point<int, -4>{-0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::fixed_point, cnl::fixed_point>");
    }

    namespace test_truncate_round_up {
        static constexpr auto expected = cnl::fixed_point<int, 0>{1};
        static constexpr auto actual = cnl::_impl::convert<cnl::nearest_rounding_tag, cnl::fixed_point<int, 0>>(
                cnl::fixed_point<int, -2>{0.75});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag>"); //Fails gives 0.75 as result.
    }
}
