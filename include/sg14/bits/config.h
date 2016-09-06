
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SG14_CONFIG_H
#define SG14_CONFIG_H

////////////////////////////////////////////////////////////////////////////////
// SG14_INT128_ENABLED macro definition

#if defined(SG14_INT128_ENABLED)
#error SG14_INT128_ENABLED already defined
#endif

#if defined(SG14_USE_INT128)

// GCC/Clang 64-bit builds support 128-bit integer through __int128 type
#if defined(__SIZEOF_INT128__)
#define SG14_INT128_ENABLED
using SG14_INT128 = __int128;
using SG14_UINT128 = unsigned __int128;
#endif

#endif  // defined(SG14_USE_INT128)

////////////////////////////////////////////////////////////////////////////////
// SG14_EXCEPTIONS_ENABLED macro definition

#if defined(SG14_EXCEPTIONS_ENABLED)
#error SG14_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define SG14_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define SG14_EXCEPTIONS_ENABLED
#endif
#else
#define SG14_EXCEPTIONS_ENABLED
#endif

#endif // SG14_CONFIG_H
