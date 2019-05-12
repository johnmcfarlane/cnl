
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>
#include <cnl/static_integer.h>

#include <cnl/_impl/type_traits/assert_same.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

template<
        int Digits,
        int Exponent>
using saturated_elastic_fixed_point = cnl::fixed_point<
        cnl::rounding_integer<
                cnl::overflow_integer<
                        cnl::elastic_integer<
                                Digits,
                                int>,
                        cnl::saturated_overflow_tag>,
                cnl::native_rounding_tag>,
        Exponent>;

TEST(a, b)
{
    using t1 = saturated_elastic_fixed_point<1, 0>;
    using t2 = saturated_elastic_fixed_point<2, -1>;

    constexpr t1 a = 1;
    constexpr t2 g = a;
    ASSERT_EQ(1 << 1, cnl::unwrap(g));
}
