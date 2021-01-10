
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OSTREAM_H)
#define CNL_IMPL_OSTREAM_H

#include "../cstdint.h"
#include "to_chars.h"

#include <array>
#include <ostream>

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(CNL_INT128_ENABLED)
        /// \brief output-streaming operator for native signed 128-bit integer
        /// \note must be used in same scope following a `using cnl::operator<<;` directive
        inline std::ostream& operator<<(std::ostream& out, int128 const n)
        {
            std::array<char, 41> line{};
            return out << cnl::_impl::to_chars_natural(std::begin(line), std::end(line), n);
        }

        /// \brief output-streaming operator for native unsigned 128-bit integer
        /// \note must be used in same scope following a `using cnl::operator<<;` directive
        inline std::ostream& operator<<(std::ostream& out, uint128 const n)
        {
            std::array<char, 40> line{};
            return out << cnl::_impl::to_chars_natural(std::begin(line), std::end(line), n);
        }
#endif
    }

    using _impl::operator<<;
}

#endif  // CNL_IMPL_OSTREAM_H
