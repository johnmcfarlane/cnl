
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_scaled_integer.h>
#include <cnl/rounding.h>

using cnl::_impl::identical;

namespace {
    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::elastic_scaled_integer<4, -1>{0.5};
        static constexpr auto actual = cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, -1>>(
                cnl::elastic_scaled_integer<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer, cnl::elastic_scaled_integer>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::elastic_scaled_integer<4, -2>{0.25};
        static constexpr auto actual = cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer<4, -2>>(
                cnl::elastic_scaled_integer<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_scaled_integer, cnl::elastic_scaled_integer>");
    }
}
