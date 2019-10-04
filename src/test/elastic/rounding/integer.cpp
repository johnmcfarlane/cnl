
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>

namespace {
    using cnl::_impl::assert_same;

    namespace test_set_rounding_t {
        static_assert(
                assert_same<
                        cnl::elastic_integer<0, cnl::rounding_integer<int, cnl::native_rounding_tag>>,
                        typename cnl::set_rounding<
                                cnl::elastic_integer<0, cnl::rounding_integer<int, cnl::nearest_rounding_tag>>,
                                cnl::native_rounding_tag>::type>::value,
                "cnl::set_rounding_t<cnl::elastic_integer<cnl::rounding_integer<>>> test failed");
    }
}
