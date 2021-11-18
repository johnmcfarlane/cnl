
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief test of `cnl/fixed_point.h`

#include <cnl/fixed_point.h>

#include <cnl/_impl/cstdint/types.h>
#include <cnl/fraction.h>
#include <cnl/scaled_integer.h>
#include <cnl/wide_integer.h>

#include <utility>

static_assert(cnl::fixed_point<int>);
static_assert(cnl::fixed_point<unsigned char>);
static_assert(cnl::fixed_point<bool>);
static_assert(cnl::fixed_point<cnl::scaled_integer<>>);
static_assert(cnl::fixed_point<cnl::wide_integer<>>);
static_assert(!cnl::fixed_point<cnl::fraction<>>);
static_assert(!cnl::fixed_point<float>);
static_assert(!cnl::fixed_point<void>);
static_assert(!cnl::fixed_point<std::pair<int, int>>);

#if defined(CNL_INT128_ENABLED)
static_assert(cnl::fixed_point<cnl::int128_t>);
static_assert(cnl::fixed_point<cnl::uint128_t>);
#endif
