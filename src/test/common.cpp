
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/bits/common.h` definitions

#include <cnl/_impl/common.h>

#include <cnl/_impl/type_traits.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    namespace test_min {
        using cnl::_impl::min;
        static_assert(identical(min(-4, 4), -4), "cnl::_impl::min test failed");
    }

    namespace test_max {
        using cnl::_impl::max;
        static_assert(identical(max(-4, 4), 4), "cnl::_impl::max test failed");
    }
}
