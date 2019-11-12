
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/number/numeric_limits.h>

#include <cnl/_impl/integer.h>

namespace {
    static_assert(
            cnl::numeric_limits<cnl::_impl::integer<>>::is_specialized,
            "cnl::numeric_limits<cnl::_impl::integer>::is_specialized");
}
