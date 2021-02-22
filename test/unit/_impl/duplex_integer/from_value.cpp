
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/from_value.h>

#include <cnl/_impl/duplex_integer/from_value.h>

#include <cnl/_impl/duplex_integer.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cstdint.h>

using cnl::_impl::identical;

using duplex_63_int = cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>;
using duplex_127_int = cnl::_impl::duplex_integer<
        cnl::_impl::duplex_integer<cnl::int32, cnl::uint32>,
        cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>;
using duplex_127_long = cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>;

#if defined(CNL_INT128_ENABLED)
static_assert(identical(
        duplex_63_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_63_int>(0x3258739857398573LL)));
static_assert(identical(
        duplex_63_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_127_int>(0x3258739857398573LL)));
#else
static_assert(identical(
        duplex_63_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_63_int>(0x3258739857398573LL)));
static_assert(identical(
        duplex_63_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_127_int>(0x3258739857398573LL)));
#endif

static_assert(identical(
        duplex_63_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_63_int>(duplex_63_int{0x3258739857398573LL})));
static_assert(identical(
        duplex_63_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_127_int>(duplex_63_int{0x3258739857398573LL})));

static_assert(identical(
        duplex_127_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_63_int>(duplex_127_int{0x3258739857398573LL})));
static_assert(identical(
        duplex_127_int{0x3258739857398573LL},
        cnl::_impl::from_value<duplex_127_int>(duplex_127_int{0x3258739857398573LL})));
