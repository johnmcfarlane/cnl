
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/config.h>
#include <cnl/_impl/num_traits/max_digits.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::_impl::identical;

#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(cnl::_impl::max_digits<cnl::uint8>, 128), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::int16>, 127), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::uint32>, 128), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::int64>, 127), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::uint128>, 128),
            "cnl::_impl::max_digits<>");
#else
    static_assert(
            identical(cnl::_impl::max_digits<cnl::int8>, 63), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::uint16>, 64), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::int32>, 63), "cnl::_impl::max_digits<>");
    static_assert(
            identical(cnl::_impl::max_digits<cnl::uint64>, 64), "cnl::_impl::max_digits<>");
#endif
}
