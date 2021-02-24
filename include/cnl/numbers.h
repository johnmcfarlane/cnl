
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/numbers.h
/// \brief definitions that might belong in the \verbatim<numbers>\endverbatim header

#if !defined(CNL_NUMBERS_H)
#define CNL_NUMBERS_H

#if __has_include(<numbers>)
#include <numbers>
#endif

#include "_impl/numbers/signedness.h"
#include "_impl/operators/tag.h"

#endif  // CNL_NUMBERS_H
