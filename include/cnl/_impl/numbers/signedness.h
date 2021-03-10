
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/numbers.h
/// \brief primary template and related specializations of \ref cnl::numbers::signedness

#if !defined(CNL_IMPL_NUMBERS_SIGNEDNESS_H)
#define CNL_IMPL_NUMBERS_SIGNEDNESS_H

#include "../../numeric_limits.h"
#include "../config.h"
#include "../cstdint/types.h"
#include "../type_traits/is_integral.h"

#include <type_traits>

/// compositional numeric library, numbers header/namespace
namespace cnl::numbers {
    /// \brief trait with predicate determining that `T` is a signed number type
    /// \tparam T a numeric type
    /// \note can be specialized for user types
    template<typename T>
    struct signedness;

    /// \cond
    template<typename T>
    requires(std::is_integral_v<T>) struct signedness<T> : std::is_signed<T> {
    };
    /// \endcond

#if defined(CNL_INT128_ENABLED)
    template<>
    struct signedness<int128> : std::true_type {
    };
    template<>
    struct signedness<uint128> : std::false_type {
    };
#endif

    /// \cond
    template<typename T>
    requires(std::is_floating_point_v<T>) struct signedness<T> : std::true_type {
    };
    /// \endcond

    /// \brief `true` if `T` is a signed numeric type; `false` if it is unsigned
    template<typename T>
    inline constexpr bool signedness_v = signedness<T>::value;
}

#endif  // CNL_IMPL_NUMBERS_SIGNEDNESS_H
