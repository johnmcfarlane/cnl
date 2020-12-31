
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief fixed-width integer types equivalent to those in \verbatim<cstdint>\endverbatim

#if !defined(CNL_IMPL_CSTDINT_TYPES_H)
#define CNL_IMPL_CSTDINT_TYPES_H

#include "../config.h"

#include <cstdint>

/// compositional numeric library
namespace cnl {

    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using uint16 = std::uint16_t;
    using int32 = std::int32_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

#if defined(CNL_INT128_ENABLED)
    // to disable 128-bit integer support, #define CNL_USE_INT128=0
    using int128 = __int128;
    using uint128 = unsigned __int128;

    using intmax = int128;
    using uintmax = uint128;
#else
    using intmax = std::intmax_t;
    using uintmax = std::uintmax_t;
#endif  // defined(CNL_INT128_ENABLED)
}

#endif  // CNL_IMPL_CSTDINT_TYPES_H
