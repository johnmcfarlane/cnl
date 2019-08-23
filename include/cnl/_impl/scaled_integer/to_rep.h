
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TO_REP_H)
#define CNL_IMPL_SCALED_INTEGER_TO_REP_H 1

#include "type.h"
#include "../number.h"
#include "../num_traits/to_rep.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref scaled_integer specialization of \ref to_rep
    /// \headerfile cnl/scaled_integer.h
    template<typename Rep, class Scale>
    struct to_rep<scaled_integer<Rep, Scale>>
            : to_rep<_impl::number<Rep, Scale>> {
    };
}

#endif // CNL_IMPL_SCALED_INTEGER_TO_REP_H
