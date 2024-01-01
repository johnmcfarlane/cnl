
//          Copyright John McFarlane 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions of use in tests

#if !defined(CNL_TEST_FRAMEWORK_TEST_H)
#define CNL_TEST_FRAMEWORK_TEST_H

#include <gtest/gtest.h>

#include <type_traits>

// cnl::_impl::identical - compiles iff same type; returns true iff equal
template<class A, class B>
[[nodiscard]] constexpr auto identical(A const& a, B const& b)
{
    static_assert(std::is_same<A, B>::value, "different types");
    return a == b;
}

#endif  // CNL_TEST_FRAMEWORK_TEST_H
