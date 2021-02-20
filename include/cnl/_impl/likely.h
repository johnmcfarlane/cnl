
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_LIKELY_H)
#define CNL_IMPL_LIKELY_H

#include "config.h"

// CNL_LIKELY - hints that a condition is likely to be true
#if defined(__clang__) || defined(__GNUC__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_LIKELY(CONDITION) __builtin_expect(!!(CONDITION), 1)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_UNLIKELY(CONDITION) __builtin_expect(!!(CONDITION), 0)
#else
#define CNL_LIKELY(CONDITION) (!!(CONDITION))
#define CNL_UNLIKELY(CONDITION) (!!(CONDITION))
#endif

#endif  // CNL_IMPL_LIKELY_H
