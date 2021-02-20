
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/duplex_integer/instantiate_duplex_integer.h>

#include <cnl/_impl/type_traits/assert_same.h>

namespace {
    using cnl::_impl::assert_same;

    static_assert(
            assert_same<
                    cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>,
                    cnl::_impl::instantiate_duplex_integer_t<1, cnl::int8>>::value);
    static_assert(
            assert_same<
                    cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::uint32>,
                    cnl::_impl::instantiate_duplex_integer_t<77, cnl::uint32>>::value);
    static_assert(
            assert_same<
                    cnl::_impl::duplex_integer<
                            cnl::_impl::duplex_integer<cnl::int64, cnl::uint64>, cnl::uint64>,
                    cnl::_impl::instantiate_duplex_integer_t<191, cnl::int64>>::value,
            "cnl::_impl::instantiate_duplex_integer_t<>");
    static_assert(
            assert_same<
                    cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<short int, short unsigned int>, cnl::_impl::duplex_integer<short unsigned int, short unsigned int>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<short unsigned int, short unsigned int>, cnl::_impl::duplex_integer<short unsigned int, short unsigned int>>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<short unsigned int, short unsigned int>, cnl::_impl::duplex_integer<short unsigned int, short unsigned int>>, short unsigned int>>,
                    cnl::_impl::instantiate_duplex_integer_t<192, cnl::int16>>::value,
            "cnl::_impl::instantiate_duplex_integer_t<>");
}
