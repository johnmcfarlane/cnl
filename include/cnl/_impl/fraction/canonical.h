
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_CANONICAL_H)
#define CNL_IMPL_FRACTION_CANONICAL_H

#include "reduce.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Numerator, typename Denominator>
        CNL_NODISCARD constexpr auto negated(fraction<Numerator, Denominator> const& rhs)
        -> decltype(_impl::make_fraction(-rhs.numerator, -rhs.denominator))
        {
            return _impl::make_fraction(-rhs.numerator, -rhs.denominator);
        }

        template<typename Numerator, typename Denominator>
        CNL_NODISCARD constexpr auto canonical_from_reduce(fraction<Numerator, Denominator> const& f)
        -> decltype(negated(f))
        {
            return (f.denominator<Denominator(0.)) ? negated(f) : f;
        }

        template<typename Numerator, typename Denominator>
        CNL_NODISCARD constexpr auto canonical(fraction<Numerator, Denominator> const& f)
        {
            return canonical_from_reduce(reduce(f));
        }
    }
}

#endif  // CNL_IMPL_FRACTION_CANONICAL_H
