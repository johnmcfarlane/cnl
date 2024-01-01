
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/fraction.h>
#include <cnl/scaled_integer.h>

#include <test.h>

namespace {
    using namespace cnl;
    using namespace std;

    constexpr auto a = make_scaled_integer(cnl::fraction{1, 3});
    static_assert(
            identical(scaled_integer<std::int64_t, power<-31>>{0.333333333022892475128173828125L}, a));

    constexpr auto b =
            make_scaled_integer(cnl::fraction{elastic_integer<1>{1}, elastic_integer<2>{3}});
    static_assert(identical(scaled_integer<elastic_integer<3>, power<-2>>{0.25}, b));

    constexpr auto c2 = scaled_integer<int, power<-16>>{fraction{1, 3}};
    static_assert(identical(scaled_integer<int, power<-16>>{0.3333282470703125}, c2));

    constexpr auto d2 =
            scaled_integer<int, power<-16>>{fraction{elastic_integer<1>{1}, elastic_integer<2>{3}}};
    static_assert(identical(scaled_integer<int, power<-16>>{0.3333282470703125}, d2));
}
