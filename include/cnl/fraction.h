
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fraction` type

#if !defined(CNL_FRACTION_H)
#define CNL_FRACTION_H 1

#include "_impl/fraction/gcd.h"
#include "_impl/fraction/make_fraction.h"
#include "_impl/fraction/operators.h"
#include "_impl/fraction/type.h"

/// compositional numeric library
namespace cnl {
    using _impl::make_fraction;

#if defined(__cpp_lib_experimental_gcd_lcm)
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::reduce

    template<typename Numerator, typename Denominator>
    constexpr auto reduce(fraction<Numerator, Denominator> const& f) {
        return make_fraction(f.numerator / _impl::gcd(f), f.denominator / _impl::gcd(f));
    }
#endif
}

#endif  // CNL_FRACTION_H
