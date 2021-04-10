
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OSTREAM_H)
#define CNL_IMPL_OSTREAM_H

#include "../cstdint.h"
#include "charconv/to_chars.h"
#include "config.h"

#include <array>
#if defined(CNL_IOSTREAMS_ENABLED)
#include <ostream>
#endif

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(CNL_INT128_ENABLED) && defined(CNL_IOSTREAMS_ENABLED)
        /// \brief output-streaming operator for native signed 128-bit integer
        /// \note must be used in same scope following a `using cnl::operator<<;` directive
        inline auto& operator<<(std::ostream& out, int128 const n)
        {
            return out << cnl::to_chars_static(n).chars.data();
        }

        /// \brief output-streaming operator for native unsigned 128-bit integer
        /// \note must be used in same scope following a `using cnl::operator<<;` directive
        inline auto& operator<<(std::ostream& out, uint128 const n)
        {
            return out << cnl::to_chars_static(n).chars.data();
        }
#endif
    }

    using _impl::operator<<;
}

#endif  // CNL_IMPL_OSTREAM_H
