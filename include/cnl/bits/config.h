
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
// CNL_RELAXED_CONSTEXPR macro definition

#if defined(CNL_RELAXED_CONSTEXPR)
#error CNL_RELAXED_CONSTEXPR already defined
#endif

#if (__cpp_constexpr >= 201304L)
#define CNL_RELAXED_CONSTEXPR constexpr
#else
#define CNL_RELAXED_CONSTEXPR
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_TEMPLATE_AUTO feature test

#if defined(CNL_TEMPLATE_AUTO)
#error CNL_TEMPLATE_AUTO already defined
#endif

// many uses of ""_c fail due to:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=82226
#if defined(__GNUG__) && !defined(__clang__) && __GNUG__ >= 7
#define CNL_TEMPLATE_AUTO_DISABLED
#endif

// not because MSVC defines __cpp_template_auto
// but because if it ever did,
// the workaround for this would probably have to change:
// https://godbolt.org/g/wbLMuN
#if defined(_MSC_VER)
#define CNL_TEMPLATE_AUTO_DISABLED
#endif

#if defined(__cpp_template_auto) && !defined(CNL_TEMPLATE_AUTO_DISABLED)
#define CNL_TEMPLATE_AUTO
#endif

#endif  // CNL_CONFIG_H
