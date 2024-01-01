
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/digits.h>

#include <cnl/_impl/wrapper/digits.h>

#include <test.h>

namespace {
    static_assert(identical(cnl::digits_v<char>, cnl::digits_v<cnl::_impl::wrapper<char>>));
}
