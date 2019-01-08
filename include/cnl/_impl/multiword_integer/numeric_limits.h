
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_MULTIWORD_INTEGER_NUMERIC_LIMITS_H

#include "../../limits.h"
#include "../limits/lowest.h"
#include "../duplex_integer/numeric_limits.h"
#include "../duplex_integer/shift.h"
#include "forward_declaration.h"
#include "from_rep.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename Word, int NumWords>
    struct numeric_limits<_impl::multiword_integer<Word, NumWords>>
            : numeric_limits<typename _impl::multiword_integer<Word, NumWords>::rep> {
        using _value_type = _impl::multiword_integer<Word, NumWords>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::min());
        }

        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }
    };
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_NUMERIC_LIMITS_H
