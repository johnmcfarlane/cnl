
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/set_digits.h>

#include <cnl/_impl/wide_integer/set_digits.h>
#include <cnl/wide_integer.h>

#include <cnl/wide_integer.h>

#include <type_traits>

namespace {
    static_assert(
            std::is_same_v<
                    cnl::wide_integer<7>,
                    typename cnl::set_digits<cnl::wide_integer<>, 7>::type>);
    static_assert(
            std::is_same_v<
                    cnl::wide_integer<8>,
                    typename cnl::set_digits<cnl::wide_integer<>, 8>::type>);

    static_assert(
            std::is_same_v<cnl::wide_integer<32>, cnl::set_digits_t<cnl::wide_integer<31>, 32>>);
}
