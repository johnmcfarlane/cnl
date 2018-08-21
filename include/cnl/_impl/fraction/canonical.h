
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_CANONICAL_H)
#define CNL_IMPL_FRACTION_CANONICAL_H 1

#include "reduce.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(__cpp_lib_gcd)
        template<typename Numerator, typename Denominator>
        constexpr auto canonical_from_reduce(fraction<Numerator, Denominator> const& f)
        -> decltype(-f)
        {
            return (f.denominator<Denominator(0.)) ? -f : f;
        }

        template<typename Numerator, typename Denominator>
        constexpr auto canonical(fraction<Numerator, Denominator> const& f)
        {
            return canonical_from_reduce(reduce(f));
        }
#endif
    }
}

#endif  // CNL_IMPL_FRACTION_CANONICAL_H
