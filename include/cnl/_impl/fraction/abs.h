
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_ABS_H)
#define CNL_IMPL_FRACTION_ABS_H

#include "../cmath/abs.h"
#include "make_fraction.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename N, typename D>
        CNL_NODISCARD constexpr auto abs(fraction<N, D> const& f)
        {
            return make_fraction(abs(f.numerator), abs(f.denominator));
        }
    }
}

#endif  // CNL_IMPL_FRACTION_ABS_H
