
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/numbers/adopt_signedness.h>
#include <cnl/_impl/type_traits/assert_same.h>

namespace {
    using cnl::_impl::assert_same;

    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<std::int32_t, std::uint32_t>, std::uint32_t>::value);
    static_assert(
            assert_same<cnl::_impl::adopt_signedness_t<std::int32_t, std::int32_t>, std::int32_t>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<std::int64_t, std::uint64_t>, std::uint64_t>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<std::int64_t, std::uint64_t>, std::uint64_t>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<std::uint32_t, std::uint32_t>, std::uint32_t>::value);
    static_assert(
            assert_same<cnl::_impl::adopt_signedness_t<std::uint32_t, std::int32_t>, std::int32_t>::value);
    static_assert(
            assert_same<
                    cnl::_impl::adopt_signedness_t<std::uint64_t, std::uint64_t>, std::uint64_t>::value);
    static_assert(
            assert_same<cnl::_impl::adopt_signedness_t<std::uint64_t, std::int64_t>, std::int64_t>::value);
}
