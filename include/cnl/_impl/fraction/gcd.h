
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_GCD_H)
#define CNL_IMPL_FRACTION_GCD_H 1

#include "type.h"

#if defined(__cpp_lib_experimental_gcd_lcm)
#include <experimental/numeric>
#endif

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(__cpp_lib_experimental_gcd_lcm)
        template<typename Numerator, typename Denominator>
        constexpr auto gcd(fraction<Numerator, Denominator> const& f) {
            return std::experimental::gcd(f.numerator, f.denominator);
        }
#endif
    }
}

#endif  // CNL_IMPL_FRACTION_GCD_H
