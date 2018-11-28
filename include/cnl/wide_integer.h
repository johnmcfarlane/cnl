
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_WIDE_INTEGER_H)
#define CNL_WIDE_INTEGER_H 1

/// \file
/// \brief file containing definitions related to \ref wide_integer

#include "_impl/wide_integer/common_type.h"
#include "_impl/wide_integer/digits.h"
#include "_impl/wide_integer/forward_declaration.h"
#include "_impl/wide_integer/from_rep.h"
#include "_impl/wide_integer/from_value.h"
#include "_impl/wide_integer/make_wide_integer.h"
#include "_impl/wide_integer/numeric_limits.h"
#include "_impl/wide_integer/operators.h"
#include "_impl/wide_integer/scale.h"
#include "_impl/wide_integer/set_digits.h"
#include "_impl/wide_integer/type.h"

/// compositional numeric library
namespace cnl {
    using _impl::wide_integer;
}

#endif  // CNL_WIDE_INTEGER_H
