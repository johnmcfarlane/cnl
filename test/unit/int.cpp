
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief test of `cnl/integer.h`

#include <cnl/integer.h>

#include <cnl/_impl/cstdint/types.h>
#include <cnl/fraction.h>
#include <cnl/scaled_integer.h>
#include <cnl/wide_integer.h>

#include <utility>

static_assert(cnl::integer<int>);
static_assert(cnl::integer<unsigned char>);
static_assert(cnl::integer<bool>);
static_assert(!cnl::integer<cnl::scaled_integer<>>);
static_assert(cnl::integer<cnl::wide_integer<>>);
static_assert(!cnl::integer<cnl::fraction<>>);
static_assert(!cnl::integer<float>);
static_assert(!cnl::integer<void>);
static_assert(!cnl::integer<std::pair<int, int>>);

#if defined(CNL_INT128_ENABLED)
static_assert(cnl::integer<cnl::int128_t>);
static_assert(cnl::integer<cnl::uint128_t>);
#endif
