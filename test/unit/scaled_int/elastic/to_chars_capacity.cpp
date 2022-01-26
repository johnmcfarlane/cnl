
//          Copyright John McFarlane 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/charconv/to_chars_capacity.h>

#include <cnl/elastic_integer.h>
#include <cnl/scaled_integer.h>

#include <cstdint>

namespace test_to_chars_capacity {
    static_assert(
            4 == cnl::_impl::to_chars_capacity<cnl::scaled_integer<std::uint8_t, cnl::power<2>>>{}());  // 1020
    static_assert(
            7 == cnl::_impl::to_chars_capacity<cnl::scaled_integer<std::int8_t, cnl::power<-3>>>{}());  // -15.875
    static_assert(
            5 == cnl::_impl::to_chars_capacity<cnl::scaled_integer<std::uint16_t, cnl::power<>>>{}());  // 65536
    static_assert(
            41 == cnl::_impl::to_chars_capacity<cnl::scaled_integer<cnl::elastic_integer<41>, cnl::power<-38>>>{}());
    static_assert(
            44 == cnl::_impl::to_chars_capacity<cnl::scaled_integer<std::int64_t, cnl::power<-32>>>{}());  // −2147483647.99999999976716935634613037109375
}
