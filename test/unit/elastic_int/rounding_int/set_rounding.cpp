
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>

#include <type_traits>

namespace {
    namespace test_set_rounding_t {
        static_assert(
                std::is_same_v<
                        cnl::elastic_integer<
                                0, cnl::rounding_integer<int, cnl::native_rounding_tag>>,
                        typename cnl::set_rounding<
                                cnl::elastic_integer<
                                        0, cnl::rounding_integer<int, cnl::nearest_rounding_tag>>,
                                cnl::native_rounding_tag>::type>);
    }
}
