
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `sg14/bits/common.h` definitions

#include <sg14/bits/common.h>

#include <gtest/gtest.h>


////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::is_integer_or_float

using sg14::_impl::is_integer_or_float;
static_assert(is_integer_or_float<uint8_t>::value, "sg14::_integer_impl::is_integer_class test failed");
static_assert(is_integer_or_float<float>::value, "sg14::_integer_impl::is_integer_class test failed");
static_assert(!is_integer_or_float<void>::value, "sg14::_integer_impl::is_integer_class test failed");
static_assert(!is_integer_or_float<int*>::value, "sg14::_integer_impl::is_integer_class test failed");
static_assert(!is_integer_or_float<std::string>::value, "sg14::_integer_impl::is_integer_class test failed");
