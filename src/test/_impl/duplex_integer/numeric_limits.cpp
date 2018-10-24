
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/numeric_limits.h>

#include <cnl/_impl/duplex_integer/numeric_limits.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_is_specialize {
        static_assert(
                cnl::numeric_limits<cnl::_impl::duplex_integer<int, unsigned int>>::is_specialized,
                "");
    }
}
