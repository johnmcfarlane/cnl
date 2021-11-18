
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/numeric_limits.h>

#include <cnl/_impl/wrapper/numeric_limits.h>

#include <cnl/_impl/wrapper/from_rep.h>

#include <limits>

namespace {
    static_assert(std::numeric_limits<cnl::_impl::wrapper<>>::is_specialized);
}
