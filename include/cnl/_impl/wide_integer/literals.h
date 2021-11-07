
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_LITERALS_H)
#define CNL_IMPL_WIDE_INTEGER_LITERALS_H

#include "../num_traits/rep_of.h"
#include "../parse.h"
#include "custom_operator.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    namespace literals {
        // cnl::_impl::operator "" _wide()
        template<char... Chars>
        [[nodiscard]] constexpr auto operator"" _wide()
        {
            return _impl::parse<wide_integer<0>, Chars...>();
        }
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_LITERALS_H
