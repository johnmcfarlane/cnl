
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_MAKE_FRACTION_H)
#define CNL_IMPL_FRACTION_MAKE_FRACTION_H 1

#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        /// creates a fraction with types deduced from the numerator and denominator
        template<typename Numerator, typename Denominator>
        constexpr fraction<Numerator, Denominator> make_fraction(Numerator const& n, Denominator const& d)
        {
            return fraction<Numerator, Denominator>{n, d};
        }

        /// creates a fraction with types deduced from the numerator
        template<typename Numerator>
        constexpr fraction<Numerator, Numerator> make_fraction(Numerator const& n)
        {
            return fraction<Numerator, Numerator>{n, 1};
        }
    }
}

#endif  // CNL_IMPL_FRACTION_MAKE_FRACTION_H
