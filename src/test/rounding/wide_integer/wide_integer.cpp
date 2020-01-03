
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding.h>
#include <cnl/wide_integer.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_convert_native_rounding {
        static_assert(
                identical(cnl::_impl::number<int, cnl::wide_tag<31, int, void>>{42},
                        cnl::convert_operator<
                                cnl::nearest_rounding_tag,
                                cnl::native_rounding_tag,
                                cnl::_impl::number<int, cnl::wide_tag<31, int, void>>,
                                cnl::_impl::number<int, cnl::wide_tag<31, int, void>>>{}(42)),
                "convert_operator<nearest_rounding_tag, native_rounding_tag, wide_integer, wide_integer>");
    }
}
