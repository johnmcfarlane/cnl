
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/type_traits.h>
#include <sg14/bits/int128.h>

using sg14::_type_traits_impl::first_fit;
using std::is_same;

////////////////////////////////////////////////////////////////////////////////
// sg14::_type_traits_impl::first_fit_t

static_assert(
        std::is_same<typename first_fit<16, std::tuple<std::int8_t, std::int16_t, std::int32_t>>::type, std::int16_t>::value, "");
static_assert(
        std::is_same<typename first_fit<16, std::tuple<std::int32_t, std::int16_t, std::int8_t>>::type, std::int32_t>::value, "");

////////////////////////////////////////////////////////////////////////////////
// sg14::set_width_t

using sg14::set_width_t;

static_assert(is_same<set_width_t<std::uint8_t, 8>, std::uint8_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int8_t, 16>, std::int16_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint16_t, 24>, std::uint32_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int16_t, 32>, std::int32_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint32_t, 40>, std::uint64_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int32_t, 48>, std::int64_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint64_t, 56>, std::uint64_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int64_t, 64>, std::int64_t>::value, "sg14::set_width_t test failed");
#if defined(_GLIBCXX_USE_INT128)
static_assert(is_same<set_width_t<std::uint8_t, 72>, unsigned __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int8_t, 80>, __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint16_t, 88>, unsigned __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int16_t, 96>, __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint32_t, 104>, unsigned __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int32_t, 112>, __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint64_t, 120>, unsigned __int128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int64_t, 128>, __int128>::value, "sg14::set_width_t test failed");
#endif

static_assert(is_same<set_width_t<double, 8>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 16>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 24>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 32>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 40>, double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 48>, double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 56>, double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 64>, double>::value, "sg14::set_width_t test failed");
#if !defined(_MSC_VER)
static_assert(is_same<set_width_t<long double, 72>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 80>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 88>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<long double, 96>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 104>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 112>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<long double, 120>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 128>, long double>::value, "sg14::set_width_t test failed");
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
