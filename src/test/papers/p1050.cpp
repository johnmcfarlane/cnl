
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/fixed_point.h>
#include <cnl/fractional.h>

#if (__cplusplus >= 201700)

namespace {
    using namespace cnl;
    using _impl::identical;
    using namespace std;

    constexpr auto a = divide(1, 3);
    static_assert(identical(fixed_point<int64, -31>{0.333333333022892475128173828125L}, a));

    constexpr auto b = divide(elastic_integer<1>{1}, elastic_integer<2>{3});
    static_assert(identical(fixed_point<elastic_integer<3>, -2>{0.25}, b));

    constexpr auto c = divide<fixed_point<int, -16>>(1, 3);
    static_assert(identical(fixed_point<int, -16>{0.3333282470703125}, c));

    constexpr auto d = divide<fixed_point<int, -16>>(elastic_integer<1>{1}, elastic_integer<2>{3});
    static_assert(identical(fixed_point<int, -16>{0.3333282470703125}, d));

    constexpr auto a2 = fixed_point{fractional{1, 3}};
    static_assert(identical(fixed_point<int64, -31>{0.333333333022892475128173828125L}, a2));

    constexpr auto b2 = fixed_point{fractional{elastic_integer<1>{1}, elastic_integer<2>{3}}};
    static_assert(identical(fixed_point<elastic_integer<3>, -2>{0.25}, b2));

    constexpr auto c2 = fixed_point<int, -16>{fractional{1, 3}};
    static_assert(identical(fixed_point<int, -16>{0.3333282470703125}, c2));

    constexpr auto d2 = fixed_point<int, -16>{fractional{elastic_integer<1>{1}, elastic_integer<2>{3}}};
    static_assert(identical(fixed_point<int, -16>{0.3333282470703125}, d2));
}

#endif
