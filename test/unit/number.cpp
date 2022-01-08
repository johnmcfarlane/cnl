
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief test of `cnl/number.h`

#include <cnl/number.h>

#include <cnl/_impl/cstdint/types.h>
#include <cnl/fraction.h>
#include <cnl/scaled_integer.h>
#include <cnl/wide_integer.h>

static_assert(cnl::number<int>);
static_assert(cnl::number<int const>);
static_assert(cnl::number<char>);
static_assert(cnl::number<unsigned char>);
static_assert(cnl::number<bool>);
static_assert(cnl::number<cnl::scaled_integer<>>);
static_assert(cnl::number<cnl::wide_integer<>>);
static_assert(cnl::number<cnl::fraction<>>);
static_assert(cnl::number<float>);
static_assert(cnl::number<long double>);

static_assert(!cnl::number<void>);
static_assert(!cnl::number<int*>);
static_assert(!cnl::number<std::pair<int, int>>);

#if defined(CNL_INT128_ENABLED)
static_assert(cnl::number<__int128>);
static_assert(cnl::number<unsigned __int128>);
static_assert(cnl::number<cnl::int128_t>);
static_assert(cnl::number<cnl::uint128_t>);
#endif
