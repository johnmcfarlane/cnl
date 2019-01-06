
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/multiword_integer/numeric_limits.h>

#include <cnl/_impl/multiword_integer/numeric_limits.h>
#include <cnl/_impl/multiword_integer/operators.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_limits {
        static_assert(
                identical(
                        cnl::_impl::multiword_integer<cnl::uint8, 2>{0xffff},
                        cnl::numeric_limits<cnl::_impl::multiword_integer<cnl::uint8, 2>>::max()),
                "cnl::numeric_limits<cnl::_impl::multiword_integer<cnl::uint8, 2>>::max()");
    }

    namespace test_mnn {
        static_assert(
                cnl::numeric_limits<cnl::_impl::multiword_integer<int, 2>>::lowest()
                        <-cnl::numeric_limits<cnl::_impl::multiword_integer<int, 2>>::max(),
                "");
    }
}
