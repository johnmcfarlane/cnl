
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(CNL_IMPL_ASSERT_H)
#define CNL_IMPL_ASSERT_H 1

#include "common.h"
#include "terminate.h"

////////////////////////////////////////////////////////////////////////////////
// CNL_ASSERT - with passing resemblance to code from github.com/Microsoft/GSL

// CNL_ASSERT - enforces that a condition *must* be true
#if defined(CNL_RELEASE)
// in release builds,
#define CNL_ASSERT(CONDITION) CNL_ASSUME(CONDITION)
#elif defined(CNL_DEBUG)
#include <exception>
#define CNL_ASSERT(CONDITION) \
    (CNL_LIKELY(CONDITION) \
        ? static_cast<void>(0) \
        : cnl::_impl::terminate<void>(#CONDITION))
#else
#error internal library error
#endif

#endif  // CNL_IMPL_ASSERT_H
