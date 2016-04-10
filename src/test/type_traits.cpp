
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/type_traits.h>
#include <sg14/bits/int128.h>

using sg14::_type_traits_impl::first_fit_t;
using std::is_same;

////////////////////////////////////////////////////////////////////////////////
// sg14::_type_traits_impl::first_fit_t

static_assert(
        std::is_same<first_fit_t<2, std::tuple<std::int8_t, std::int16_t, std::int32_t>>, std::int16_t>::value, "");
static_assert(
        std::is_same<first_fit_t<2, std::tuple<std::int32_t, std::int16_t, std::int8_t>>, std::int32_t>::value, "");

////////////////////////////////////////////////////////////////////////////////
// sg14::resize_t

using sg14::resize_t;

static_assert(is_same<resize_t<std::uint8_t, 1>, std::uint8_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int8_t, 2>, std::int16_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::uint16_t, 3>, std::uint32_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int16_t, 4>, std::int32_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::uint32_t, 5>, std::uint64_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int32_t, 6>, std::int64_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::uint64_t, 7>, std::uint64_t>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int64_t, 8>, std::int64_t>::value, "sg14::resize_t test failed");
#if defined(_GLIBCXX_USE_INT128)
static_assert(is_same<resize_t<std::uint8_t, 9>, unsigned __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int8_t, 10>, __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::uint16_t, 11>, unsigned __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int16_t, 12>, __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::uint32_t, 13>, unsigned __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int32_t, 14>, __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::uint64_t, 15>, unsigned __int128>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<std::int64_t, 16>, __int128>::value, "sg14::resize_t test failed");
#endif

static_assert(is_same<resize_t<double, 1>, float>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 2>, float>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 3>, float>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 4>, float>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 5>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 6>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 7>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 8>, double>::value, "sg14::resize_t test failed");
#if !defined(_MSC_VER)
static_assert(is_same<resize_t<long double, 9>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 10>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 11>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<long double, 12>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 13>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 14>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<long double, 15>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 16>, long double>::value, "sg14::resize_t test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::width

using sg14::width;

static_assert(width<std::int8_t>::value == 8, "sg14::width test failed");
static_assert(width<std::uint8_t>::value == 8, "sg14::width test failed");
static_assert(width<std::int16_t>::value == 16, "sg14::width test failed");
static_assert(width<std::uint16_t>::value == 16, "sg14::width test failed");
static_assert(width<std::int32_t>::value == 32, "sg14::width test failed");
static_assert(width<std::uint32_t>::value == 32, "sg14::width test failed");
static_assert(width<std::int64_t>::value == 64, "sg14::width test failed");
static_assert(width<std::uint64_t>::value == 64, "sg14::width test failed");

#if defined(_GLIBCXX_USE_INT128)
static_assert(width<__int128>::value == 128, "sg14::width test failed");
static_assert(width<unsigned __int128>::value == 128, "sg14::width test failed");
#endif
