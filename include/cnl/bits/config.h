
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_CONFIG_H)
#define CNL_CONFIG_H

////////////////////////////////////////////////////////////////////////////////
// CNL_INT128_ENABLED macro definition

#if defined(CNL_INT128_ENABLED)
#error CNL_INT128_ENABLED already defined
#endif

#if defined(CNL_USE_INT128)

// GCC/Clang 64-bit builds support 128-bit integer through __int128 type
#if defined(__SIZEOF_INT128__)
#define CNL_INT128_ENABLED
#endif

#endif  // defined(CNL_USE_INT128)

////////////////////////////////////////////////////////////////////////////////
// CNL_EXCEPTIONS_ENABLED macro definition

#if defined(CNL_EXCEPTIONS_ENABLED)
#error CNL_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define CNL_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define CNL_EXCEPTIONS_ENABLED
#endif
#else
#define CNL_EXCEPTIONS_ENABLED
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_COPY_CONSTEXPR macro definition

#if defined(CNL_COPY_CONSTEXPR)
#error CNL_COPY_CONSTEXPR already defined
#endif

#if (__cplusplus >= 201402L)
#define CNL_COPY_CONSTEXPR constexpr
#else
#define CNL_COPY_CONSTEXPR
#endif

#endif  // CNL_CONFIG_H
