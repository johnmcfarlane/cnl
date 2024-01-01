
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/config.h>
#include <cnl/_impl/num_traits/max_digits.h>
#include <test.h>

namespace {
#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(cnl::_impl::max_digits<std::uint8_t>, 128), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<std::int16_t>, 127), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<std::uint32_t>, 128), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<std::int64_t>, 127), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::uint128_t>, 128),
            "cnl::_impl::max_digits<>");
#else
    static_assert(
            identical(cnl::_impl::max_digits<std::int8_t>, 63), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<std::uint16_t>, 64), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<std::int32_t>, 63), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<std::uint64_t>, 64), "cnl::_impl::max_digits<>");
#endif
}
