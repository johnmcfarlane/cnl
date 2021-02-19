
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief integer creation macros equivalent to those in \verbatim<cstdint>\endverbatim

#if !defined(CNL_IMPL_CSTDINT_MACROS_H)
#define CNL_IMPL_CSTDINT_MACROS_H

#include "../common.h"
#include "../config.h"
#include "../parse.h"
#include "types.h"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// CNL_INTMAX_C and CNL_UINTMAX_C

#if defined(CNL_INT128_ENABLED)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_INTMAX_C(N) (::cnl::_impl::parse<::cnl::intmax>(CNL_STR(N)))

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_UINTMAX_C(N) (::cnl::_impl::parse<::cnl::uintmax>(CNL_STR(N)))

#else

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_INTMAX_C INTMAX_C

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_UINTMAX_C UINTMAX_C

#endif

#endif  // CNL_IMPL_CSTDINT_MACROS_H
