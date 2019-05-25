
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/math.h
/// \brief functions that might belong in the \verbatim<math>\endverbatim header

#if !defined(CNL_MATH_H)
#define CNL_MATH_H

#if defined(__cpp_concepts)
#include "_impl/math_20.h"
#else
#include "_impl/math_17.h"
#endif

#endif  // CNL_MATH_H
