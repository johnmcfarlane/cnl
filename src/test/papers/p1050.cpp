
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/scaled_integer.h>
#include <cnl/fraction.h>
#include <cnl/_impl/type_traits/identical.h>

#if (__cplusplus >= 201703L)

namespace {
    using namespace cnl;
    using _impl::identical;
    using namespace std;

    constexpr auto a = quotient(1, 3);
    static_assert(identical(scaled_integer<int64, -31>{0.333333333022892475128173828125L}, a));

    constexpr auto b = quotient(elastic_integer<1>{1}, elastic_integer<2>{3});
    static_assert(identical(scaled_integer<elastic_integer<3>, -2>{0.25}, b));

    constexpr auto c = quotient<scaled_integer<int, -16>>(1, 3);
    static_assert(identical(scaled_integer<int, -16>{0.3333282470703125}, c));

    constexpr auto d = quotient<scaled_integer<int, -16>>(elastic_integer<1>{1}, elastic_integer<2>{3});
    static_assert(identical(scaled_integer<int, -16>{0.3333282470703125}, d));

    constexpr auto a2 = scaled_integer{fraction{1, 3}};
    static_assert(identical(scaled_integer<int64, -31>{0.333333333022892475128173828125L}, a2));

    constexpr auto b2 = scaled_integer{fraction{elastic_integer<1>{1}, elastic_integer<2>{3}}};
    static_assert(identical(scaled_integer<elastic_integer<3>, -2>{0.25}, b2));

    constexpr auto c2 = scaled_integer<int, -16>{fraction{1, 3}};
    static_assert(identical(scaled_integer<int, -16>{0.3333282470703125}, c2));

    constexpr auto d2 = scaled_integer<int, -16>{fraction{elastic_integer<1>{1}, elastic_integer<2>{3}}};
    static_assert(identical(scaled_integer<int, -16>{0.3333282470703125}, d2));
}

#endif
