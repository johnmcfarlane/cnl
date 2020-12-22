
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/num_traits/adopt_signedness.h>
#include <cnl/_impl/type_traits/assert_same.h>

namespace {
    using cnl::_impl::assert_same;

    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<cnl::int32, cnl::uint32>, cnl::uint32>::value);
    static_assert(
            assert_same<cnl::_impl::adopt_signedness_t<cnl::int32, cnl::int32>, cnl::int32>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<cnl::int64, cnl::uint64>, cnl::uint64>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<cnl::int64, cnl::uint64>, cnl::uint64>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<cnl::uint32, cnl::uint32>, cnl::uint32>::value);
    static_assert(
            assert_same<cnl::_impl::adopt_signedness_t<cnl::uint32, cnl::int32>, cnl::int32>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<cnl::uint64, cnl::uint64>, cnl::uint64>::value);
    static_assert(
            assert_same<cnl::_impl::adopt_signedness_t<cnl::uint64, cnl::int64>, cnl::int64>::value);
}
