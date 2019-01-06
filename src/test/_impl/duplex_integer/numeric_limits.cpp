
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/numeric_limits.h>

#include <cnl/_impl/duplex_integer/numeric_limits.h>
#include <cnl/_impl/duplex_integer/operators.h>
#include <cnl/_impl/duplex_integer/type.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_is_specialize {
        static_assert(
                cnl::numeric_limits<cnl::_impl::duplex_integer<int, unsigned int>>::is_specialized,
                "");
    }

    namespace test_max {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0xffff},
                        cnl::numeric_limits<cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>>::max()),
                "cnl::numeric_limits<cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>>::max()");
    }

    namespace test_max {
        static_assert(
                identical(
                        16,
                        cnl::numeric_limits<cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>>::digits),
                "cnl::numeric_limits<cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>>::digits");
        static_assert(
                identical(
                        cnl::numeric_limits<long long>::digits+cnl::numeric_limits<unsigned>::digits,
                        cnl::numeric_limits<cnl::_impl::duplex_integer<long long, unsigned>>::digits),
                "cnl::numeric_limits<cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>>::digits");
    }

    namespace test_mnn {
        static_assert(
                cnl::numeric_limits<cnl::_impl::duplex_integer<int, unsigned>>::lowest()
                        <-cnl::numeric_limits<cnl::_impl::duplex_integer<int, unsigned>>::max(),
                "");
    }
}
