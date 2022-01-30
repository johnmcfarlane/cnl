
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/wide-integer.h>

#include <cnl/_impl/num_traits/rounding.h>
#include <cnl/_impl/rounding/native_rounding_tag.h>

#include <test.h>

static_assert(cnl::is_integer_v<cnl::_impl::math::wide_integer::uintwide_t<64>>);

static_assert(
        std::is_same_v<
                cnl::native_rounding_tag,
                cnl::rounding_t<cnl::_impl::math::wide_integer::uintwide_t<64>>>);

static_assert(
        std::is_same_v<
                std::uint64_t,
                cnl::_impl::math::wide_integer::detail::uint_type_helper<64>::exact_unsigned_type>);

static_assert(
        identical(
                cnl::_impl::from_value<cnl::_impl::math::wide_integer::uintwide_t<160, unsigned int, void, true>>(42),
                cnl::_impl::math::wide_integer::uintwide_t<32, unsigned int, void, true>(42)),
        "cnl::_impl::from_value<cnl::_impl::math::wide_integer::uintwide_t>");

TEST(wide_integer, float_ctor)  // NOLINT
{
    auto constexpr expected{cnl::_impl::math::wide_integer::uintwide_t<64>(42)};
    auto constexpr actual{cnl::_impl::math::wide_integer::uintwide_t<64>(42.F)};
    CNL_ASSERT_EQ(expected, actual);
}

namespace test_conversion_op {
    static_assert(identical(
            42,
            static_cast<int>(cnl::_impl::math::wide_integer::uintwide_t<64>{42})));
    static_assert(identical(
            42.L,
            static_cast<long double>(cnl::_impl::math::wide_integer::uintwide_t<64>{42})));
}

TEST(wide_integer, lowest_max)  // NOLINT
{
    using type = cnl::_impl::math::wide_integer::uintwide_t<64, unsigned, void, true>;
    using limits = std::numeric_limits<type>;
    constexpr auto expected{-1 - limits::max()};
    constexpr auto actual{limits::lowest()};
    CNL_ASSERT_EQ(expected, actual);
}
