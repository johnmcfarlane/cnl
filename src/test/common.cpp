
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `sg14/bits/common.h` definitions

#include <sg14/bits/common.h>

#include <sg14/bits/type_traits.h>

#include <gtest/gtest.h>

namespace {
    using sg14::_impl::identical;

    namespace test_min {
        using sg14::_impl::min;
        static_assert(identical(min(-4, 4), -4), "sg14::_impl::min test failed");
    }

    namespace test_max {
        using sg14::_impl::max;
        static_assert(identical(max(-4, 4), 4), "sg14::_impl::max test failed");
    }
}
