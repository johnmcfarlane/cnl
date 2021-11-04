
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CNL_ASSERT_H)
#define CNL_IMPL_CNL_ASSERT_H

#include "likely.h"
#include "unreachable.h"

////////////////////////////////////////////////////////////////////////////////
// CNL_ASSERT - expresses that a condition *must* be true

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_ASSERT(CONDITION) (CNL_LIKELY(CONDITION) ? static_cast<void>(0) : cnl::_impl::unreachable("Assertion `" #CONDITION "' failed."))

#endif  // CNL_IMPL_CNL_ASSERT_H
