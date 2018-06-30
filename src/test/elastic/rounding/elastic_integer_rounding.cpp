
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding.h>

namespace {
    using cnl::_impl::identical;

    namespace test_shift_right_nearest {
        static_assert(
                identical(
                        cnl::elastic_integer<53>{14},
                        cnl::shift_right<cnl::nearest_rounding_tag, cnl::elastic_integer<62>, cnl::constant<9>>{}(
                                7000,
                                cnl::constant<9>{})),
                "shift_right(elastic_integer)");

#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::elastic_integer<117>{14},
                        cnl::shift_right<cnl::nearest_rounding_tag, cnl::elastic_integer<126>, cnl::constant<9>>{}(
                                7000,
                                cnl::constant<9>{})),
                "shift_right(elastic_integer)");
#endif
    }
}
