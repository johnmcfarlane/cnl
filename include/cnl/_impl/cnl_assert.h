
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CNL_ASSERT_H)
#define CNL_IMPL_CNL_ASSERT_H

#include "config.h"
#include "likely.h"
#include "terminate.h"

////////////////////////////////////////////////////////////////////////////////
// CNL_ASSUME - hints that a condition *must* be true

#ifdef _MSC_VER
#define CNL_ASSUME(cond) __assume(cond)
#elif defined(__GNUC__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define CNL_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_ASSERT - with passing resemblance to code from github.com/Microsoft/GSL

// CNL_ASSERT - enforces that a condition *must* be true
#if defined(CNL_RELEASE)
// in release builds,
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_ASSERT(CONDITION) CNL_ASSUME(CONDITION)
#elif defined(CNL_DEBUG)
#include <exception>
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_ASSERT(CONDITION) \
    (CNL_LIKELY(CONDITION) ? static_cast<void>(0) : cnl::_impl::terminate<void>(#CONDITION))
#else
#error internal library error
#endif

#endif  // CNL_IMPL_CNL_ASSERT_H
