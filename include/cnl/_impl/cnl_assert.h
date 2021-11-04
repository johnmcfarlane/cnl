
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CNL_ASSERT_H)
#define CNL_IMPL_CNL_ASSERT_H

#include "abort.h"
#include "config.h"
#include "likely.h"
#include "unreachable.h"

////////////////////////////////////////////////////////////////////////////////
// CNL_ASSERT - with passing resemblance to code from github.com/Microsoft/GSL

// CNL_ASSERT - enforces that a condition *must* be true
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_ASSERT(CONDITION) \
    ((CONDITION) \
             ? static_cast<void>(0) \
             : cnl::_impl::unreachable(__FILE__ ":" CNL_STR(__LINE__) " assert: " #CONDITION))

#endif  // CNL_IMPL_CNL_ASSERT_H
