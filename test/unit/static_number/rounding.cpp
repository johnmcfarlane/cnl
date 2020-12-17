
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/static_number.h>

namespace {
    using cnl::_impl::assert_same;

    namespace test_rounding_t {
        static_assert(
                assert_same<
                        cnl::nearest_rounding_tag, cnl::rounding_t<cnl::static_number<1>>>::value,
                "cnl::rounding_t<cnl::static_number<>> test failed");
    }
}
