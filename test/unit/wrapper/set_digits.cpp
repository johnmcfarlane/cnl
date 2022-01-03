
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/set_digits.h>

#include <cnl/_impl/wrapper/set_digits.h>

#include <type_traits>

namespace {
    static_assert(
            std::is_same_v<
                    cnl::_impl::wrapper<int8_t>,
                    typename cnl::set_digits<cnl::_impl::wrapper<>, 7>::type>);
    static_assert(
            std::is_same_v<
                    cnl::_impl::wrapper<int16_t>,
                    typename cnl::set_digits<cnl::_impl::wrapper<>, 8>::type>);
    static_assert(
            std::is_same_v<
                    cnl::_impl::wrapper<int16_t>,
                    typename cnl::set_digits<cnl::_impl::wrapper<>, 8>::type>);
}
