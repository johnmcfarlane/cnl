
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fraction` type

#if !defined(CNL_FRACTION_H)
#define CNL_FRACTION_H

#include "_impl/fraction/abs.h"
#include "_impl/fraction/canonical.h"
#include "_impl/fraction/ctors.h"
#include "_impl/fraction/gcd.h"
#include "_impl/fraction/hash.h"
#include "_impl/fraction/make_fraction.h"
#include "_impl/fraction/operators.h"
#include "_impl/fraction/reduce.h"
#include "_impl/fraction/to_string.h"
#include "_impl/fraction/type.h"

/// compositional numeric library
namespace cnl {
    using _impl::abs;
    using _impl::make_fraction;
    using _impl::to_string;
    using _impl::reduce;
    using _impl::canonical;
}

#endif  // CNL_FRACTION_H
