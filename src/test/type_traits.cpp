
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits.h>

using std::is_same;

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
static_assert(is_same<resize_t<long double, 3>, float>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 4>, float>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 5>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<long double, 6>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 7>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 8>, double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<long double, 9>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 10>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 11>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<long double, 12>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 13>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<double, 14>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<long double, 15>, long double>::value, "sg14::resize_t test failed");
static_assert(is_same<resize_t<float, 16>, long double>::value, "sg14::resize_t test failed");
