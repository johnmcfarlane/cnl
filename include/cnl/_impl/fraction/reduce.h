
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_REDUCE_H)
#define CNL_IMPL_FRACTION_REDUCE_H 1

#include "gcd.h"
#include "make_fraction.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(__cpp_lib_experimental_gcd_lcm)
        template<typename Numerator, typename Denominator>
        constexpr auto reduce(fraction<Numerator, Denominator> const& f) {
            return make_fraction(f.numerator / gcd(f), f.denominator / gcd(f));
        }
#endif
    }
}

#endif  // CNL_IMPL_FRACTION_REDUCE_H
