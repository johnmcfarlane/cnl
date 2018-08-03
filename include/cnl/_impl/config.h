
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
#if !defined(__SIZEOF_INT128__)
#error CNL_USE_INT128 is defined but __int128 is not supported
#endif

#define CNL_INT128_ENABLED

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

////////////////////////////////////////////////////////////////////////////////
// CNL_GCC_INTRINSICS_ENABLED macro definition

#if defined(CNL_GCC_INTRINSICS_ENABLED)
#error CNL_GCC_INTRINSICS_ENABLED already defined
#endif

#if defined(CNL_USE_GCC_INTRINSICS)
#if CNL_USE_GCC_INTRINSICS
#define CNL_GCC_INTRINSICS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#define CNL_GCC_INTRINSICS_ENABLED
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_UNREACHABLE_UB_ENABLED macro definition

// When enabled, cnl::_impl::unreachable exhibits undefined behavior,
// i.e. the compiler has carte blanche to optimize based on the assumption
// that no call to cnl::_impl::unreachable will ever be made at run time.

#if defined(CNL_UNREACHABLE_UB_ENABLED)
#error CNL_UNREACHABLE_UB_ENABLED already defined
#endif

#if defined(CNL_USE_UNREACHABLE_UB)
#if CNL_USE_UNREACHABLE_UB
#define CNL_UNREACHABLE_UB_ENABLED
#endif
#elif defined(NDEBUG)
#define CNL_UNREACHABLE_UB_ENABLED
#endif

#endif  // CNL_CONFIG_H
