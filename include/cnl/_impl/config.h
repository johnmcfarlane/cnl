
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions which can be used to configure CNL library

#if !defined(CNL_CONFIG_H)
#define CNL_CONFIG_H

////////////////////////////////////////////////////////////////////////////////
// CNL_DEBUG and CNL_RELEASE macro definitions

// Customization point: define either CNL_DEBUG or CNL_RELEASE
// to affect how contract violations are handled
// in builds that don't support contract [[attributes]].
#if defined(CNL_DEBUG) && defined(CNL_RELEASE)
#error Mutually exclusive macros, CNL_DEBUG are both defined CNL_RELEASE.
#elif !defined(CNL_DEBUG) && !defined(CNL_RELEASE)
#if defined(NDEBUG)
#define CNL_RELEASE
#else
#define CNL_DEBUG
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_INT128_ENABLED macro definition

#if defined(CNL_INT128_ENABLED)
#error CNL_INT128_ENABLED already defined
#endif

#if defined(CNL_CAN_USE_INT128)
#error CNL_CAN_USE_INT128 already defined
#endif

#if defined(__SIZEOF_INT128__)
#define CNL_CAN_USE_INT128 1  // NOLINT(cppcoreguidelines-macro-usage)
#else
#define CNL_CAN_USE_INT128 0  // NOLINT(cppcoreguidelines-macro-usage)
#endif

#if !defined(CNL_USE_INT128)
/// \def CNL_USE_INT128
/// \brief user flag enables or disables support for `__int128` on 64-bit GCC/Clang platforms;
///        defaults to `1` on supported platforms.
/// \note GCC pedantic warnings must be disabled in order to use `__int128`.
/// \sa CNL_INT128_ENABLED, cnl::int128, cnl::uint128
#define CNL_USE_INT128 CNL_CAN_USE_INT128  // NOLINT(cppcoreguidelines-macro-usage)
#endif

#if CNL_USE_INT128
#if !CNL_CAN_USE_INT128
#error CNL_USE_INT128 is defined but 128-bit integers is not enabled for this compiler
#endif
/// \def CNL_INT128_ENABLED
/// \brief non-zero iff CNL is configured to support 128-bit integers
/// \sa CNL_USE_INT128, cnl::int128, cnl::uint128
#define CNL_INT128_ENABLED
#endif

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
// CNL_TEMPLATE_AUTO feature test

#if defined(CNL_TEMPLATE_AUTO)
#error CNL_TEMPLATE_AUTO already defined
#endif

// not because MSVC defines __cpp_template_auto
// but because if it ever did,
// the workaround for this would probably have to change:
// https://godbolt.org/g/wbLMuN
#if defined(_MSC_VER)
#define CNL_TEMPLATE_AUTO_DISABLED
#endif

#if defined(__cpp_nontype_template_parameter_auto) && !defined(CNL_TEMPLATE_AUTO_DISABLED)
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
#elif defined(CNL_RELEASE)
#define CNL_UNREACHABLE_UB_ENABLED
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_IOSTREAM_ENABLED macro definition

#if defined(CNL_IOSTREAM_ENABLED)
#error CNL_IOSTREAM_ENABLED already defined
#endif

#if defined(CNL_USE_IOSTREAM)
#if CNL_USE_IOSTREAM
#define CNL_IOSTREAM_ENABLED
#endif
#elif defined(__STDC_HOSTED__)
#if __STDC_HOSTED__
#define CNL_IOSTREAM_ENABLED
#else
// Either CNL_USE_IOSTREAM or __STDC_HOSTED__
// must be defined and set to either 0 or 1.
#error __STDC_HOSTED__ not defined
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_BUILTIN_OVERFLOW_ENABLED

#if defined(CNL_BUILTIN_OVERFLOW_ENABLED)
#error CNL_BUILTIN_OVERFLOW_ENABLED already defined
#endif

#if defined(CNL_BUILTIN_OVERFLOW_SUPPORTED_BY_TOOLCHAIN)
#error CNL_BUILTIN_OVERFLOW_SUPPORTED_BY_TOOLCHAIN already defined
#elif defined(__GNUG__) && !defined(__clang__)
#define CNL_BUILTIN_OVERFLOW_SUPPORTED_BY_TOOLCHAIN
#endif

#if defined(CNL_BUILTIN_OVERFLOW_SUPPORTED_BY_TOOLCHAIN)
#define CNL_BUILTIN_OVERFLOW_ENABLED
#endif

////////////////////////////////////////////////////////////////////////////////
// CNL_NODISCARD

#if defined(CNL_NODISCARD)
#error CNL_NODISCARD already defined
#endif

#if (__cplusplus >= 201703L) && !defined(_MSC_VER)
#define CNL_NODISCARD [[nodiscard]]  // NOLINT(cppcoreguidelines-macro-usage)
#endif

#if ! defined(CNL_NODISCARD)
#define CNL_NODISCARD
#endif

////////////////////////////////////////////////////////////////////////////////

#endif  // CNL_CONFIG_H
