
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_FROM_REP_H)
#define CNL_IMPL_SCALED_INTEGER_FROM_REP_H

#include "type.h"
#include "../num_traits/from_rep.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref scaled_integer specialization of \ref from_rep
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Exponent the \c Exponent parameter of the generated \ref scaled_integer type
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    template<typename ArchetypeRep, int Exponent, int Radix, typename Rep>
    struct from_rep<scaled_integer<ArchetypeRep, power<Exponent, Radix>>, Rep> {
        /// \brief generates a \ref scaled_integer equivalent to \c r in type and value
        CNL_NODISCARD constexpr auto operator()(Rep const& r) const
        -> scaled_integer<Rep, power<Exponent, Radix>>
        {
            return scaled_integer<Rep, power<Exponent, Radix>>(r, 0);
        }
    };
}

#endif // CNL_IMPL_SCALED_INTEGER_FROM_REP_H
