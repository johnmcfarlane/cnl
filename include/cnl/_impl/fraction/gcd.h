
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_GCD_H)
#define CNL_IMPL_FRACTION_GCD_H

#include "type.h"

#include <numeric>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Numerator, typename Denominator>
        CNL_NODISCARD constexpr auto gcd(fraction<Numerator, Denominator> const& f) {
            using std::gcd;
            return gcd(f.numerator, f.denominator);
        }
    }
}

#endif  // CNL_IMPL_FRACTION_GCD_H
