
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/from_rep.h>

#include <cnl/wide_integer.h>

#include <cnl/_impl/type_traits/assert_same.h>

using cnl::_impl::assert_same;

namespace {
    static_assert(
            assert_same<
                    cnl::wide_integer<16, std::int8_t>,
                    typename cnl::set_rep<
                            cnl::_impl::wrapper<unsigned short, cnl::wide_tag<16, std::uint8_t>>,
                            int>::type>::value,
            "cnl::wide_integer<16, std::uint8_t> << cnl::constant");
    static_assert(
            assert_same<
                    cnl::wide_integer<31, int>,
                    typename cnl::set_rep<cnl::wide_integer<>, std::int64_t>::type>::value,
            "cnl::from_rep_t<cnl::wide_integer>");
#if defined(CNL_INT128_ENABLED)
    static_assert(
            assert_same<
                    cnl::_impl::wrapper<cnl::uint128_t, cnl::wide_tag<128, unsigned int>>,
                    typename cnl::set_rep<
                            cnl::_impl::wrapper<long unsigned int, cnl::wide_tag<128, unsigned int>>,
                            long unsigned int>::type>::value,
            "cnl::from_rep_t<cnl::wide_integer>");
#endif
    static_assert(
            assert_same<
                    cnl::wide_integer<31, unsigned int>,
                    cnl::_impl::set_rep_t<cnl::wide_integer<>, std::uint32_t>>::value,
            "cnl::from_rep_t<cnl::wide_integer>");

    static_assert(
            assert_same<
                    cnl::wide_integer<15, short>,
                    cnl::_impl::set_rep_t<cnl::wide_integer<15, short>, std::int16_t>>::value,
            "cnl::from_rep_t<cnl::wide_integer>");
    static_assert(
            assert_same<
                    cnl::wide_integer<15, unsigned short>,
                    cnl::_impl::set_rep_t<cnl::wide_integer<15, short>, std::uint16_t>>::value,
            "cnl::from_rep_t<cnl::wide_integer>");
}
