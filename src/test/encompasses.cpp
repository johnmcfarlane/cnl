
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of definitions in cnl/bits/encompasses.h

#include <cnl/bits/encompasses.h>

using std::is_same;

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::encompasses

static_assert(is_same<cnl::_impl::unsigned_or_float_t<cnl::int8>, cnl::uint8>::value, "");
static_assert(is_same<cnl::_impl::unsigned_or_float_t<cnl::uint8>, cnl::uint8>::value, "");
static_assert(is_same<cnl::_impl::unsigned_or_float_t<float>, float>::value, "");

static_assert(cnl::_impl::encompasses<short, short>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<short, int>::value, "cnl::_impl::encompasses test failed");
static_assert(cnl::_impl::encompasses<int, short>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::int8, cnl::uint8>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::uint8, cnl::int8>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::uint64, cnl::int64>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<cnl::int64, cnl::uint64>::value, "cnl::_impl::encompasses test failed");
static_assert(cnl::_impl::encompasses<long, unsigned short>::value, "cnl::_impl::encompasses test failed");
static_assert(!cnl::_impl::encompasses<unsigned short, long>::value, "cnl::_impl::encompasses test failed");
