
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_number.h>
#include <cnl/rounding.h>
#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_nearest_round_down {
        static constexpr auto expected = cnl::elastic_number<4, -1>{0.5};
        static constexpr auto actual = cnl::_impl::convert<cnl::nearest_rounding_tag, cnl::elastic_number<4, -1>>(
                cnl::elastic_number<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_number, cnl::elastic_number>");
    }

    namespace test_nearest_round_up {
        static constexpr auto expected = cnl::elastic_number<4, -2>{0.25};
        static constexpr auto actual = cnl::_impl::convert<cnl::nearest_rounding_tag, cnl::elastic_number<4, -2>>(
                cnl::elastic_number<8, -4>{0.3125});

        static_assert(
                identical(expected, actual),
                "cnl::convert<cnl::nearest_rounding_tag, cnl::elastic_number, cnl::elastic_number>");
    }
}
