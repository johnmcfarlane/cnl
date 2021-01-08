
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/make_wrapper.h>

#include <cnl/_impl/wrapper/make_wrapper.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/wrapper/operators.h>

using cnl::_impl::identical;

namespace {
    static_assert(identical(cnl::_impl::wrapper<long>{1}, cnl::_impl::make_wrapper(1L)));
}
