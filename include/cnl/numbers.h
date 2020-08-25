
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/numbers.h
/// \brief functions that might belong in the \verbatim<numbers>\endverbatim header

#if !defined(CNL_NUMBERS_H)
#define CNL_NUMBERS_H

#if defined(__cpp_concepts)
#include "_impl/numbers_20.h"
#else
#include "_impl/numbers_17.h"
#endif

#endif  // CNL_NUMBERS_H
