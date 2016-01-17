
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
