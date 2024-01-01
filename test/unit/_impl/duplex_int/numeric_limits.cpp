
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/numeric_limits.h>

#include <cnl/_impl/duplex_integer/definition.h>
#include <cnl/_impl/duplex_integer/numeric_limits.h>
#include <cnl/_impl/duplex_integer/operators.h>

#include <test.h>

#include <limits>

namespace {
    namespace test_is_specialize {
        static_assert(
                std::numeric_limits<cnl::_impl::duplex_integer<int, unsigned int>>::is_specialized);
    }

    namespace test_max {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>{0xffff},
                        std::numeric_limits<
                                cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>>::max()),
                "std::numeric_limits<cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>>::max()");
    }

    namespace test_max {
        static_assert(
                identical(
                        16, std::numeric_limits<
                                    cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>>::digits),
                "std::numeric_limits<cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>>::digits");
        static_assert(
                identical(
                        std::numeric_limits<long long>::digits
                                + std::numeric_limits<unsigned>::digits,
                        std::numeric_limits<
                                cnl::_impl::duplex_integer<long long, unsigned>>::digits),
                "std::numeric_limits<cnl::_impl::duplex_integer<std::uint8_t, std::uint8_t>>::digits");
    }

    namespace test_mnn {
        static_assert(
                std::numeric_limits<cnl::_impl::duplex_integer<int, unsigned>>::lowest()
                < -std::numeric_limits<cnl::_impl::duplex_integer<int, unsigned>>::max());
    }
}
