
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/multiword_integer/optimal_multiword_integer.h>
#include <cnl/_impl/type_traits/assert_same.h>

namespace {
    using cnl::_impl::assert_same;

#if defined(CNL_INT128_ENABLED)
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::int64, 2>,
                    cnl::_impl::optimal_multiword_integer_t<1, cnl::int8>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::uint64, 2>,
                    cnl::_impl::optimal_multiword_integer_t<77, cnl::uint32>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::int64, 3>,
                    cnl::_impl::optimal_multiword_integer_t<191, cnl::int64>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::int64, 4>,
                    cnl::_impl::optimal_multiword_integer_t<192, cnl::int16>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
#else
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::int32, 2>,
                    cnl::_impl::optimal_multiword_integer_t<1, cnl::int8>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::uint32, 3>,
                    cnl::_impl::optimal_multiword_integer_t<77, cnl::uint32>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::int32, 6>,
                    cnl::_impl::optimal_multiword_integer_t<191, cnl::int64>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
    static_assert(
            assert_same<
                    cnl::_impl::multiword_integer<cnl::int32, 7>,
                    cnl::_impl::optimal_multiword_integer_t<192, cnl::int16>>::value,
            "cnl::_impl::optimal_multiword_integer<>");
#endif
}
