
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/duplex_integer/narrowest_integer.h>

#include <type_traits>

namespace {
    static_assert(
            std::is_same_v<
                    std::int8_t,
                    cnl::_impl::narrowest_integer_t<1, std::int8_t>>);
    static_assert(
            std::is_same_v<
                    cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<std::uint32_t, std::uint32_t>, std::uint32_t>,
                    cnl::_impl::narrowest_integer_t<77, std::uint32_t>>);
    static_assert(
            std::is_same_v<
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<std::int64_t, std::uint64_t>, std::uint64_t>,
                    cnl::_impl::narrowest_integer_t<191, std::int64_t>>);
    static_assert(
            std::is_same_v<
                    cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<short int, short unsigned int>, cnl::_impl::duplex_integer<short unsigned int, short unsigned int>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<short unsigned int, short unsigned int>, cnl::_impl::duplex_integer<short unsigned int, short unsigned int>>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<short unsigned int, short unsigned int>, cnl::_impl::duplex_integer<short unsigned int, short unsigned int>>, short unsigned int>>,
                    cnl::_impl::narrowest_integer_t<192, std::int16_t>>);
}
