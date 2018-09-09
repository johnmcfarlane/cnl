
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FIXED_POINT_FROM_REP_H)
#define CNL_IMPL_FIXED_POINT_FROM_REP_H 1

#include "type.h"
#include "../num_traits/from_rep.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref fixed_point specialization of \ref from_rep
    /// \headerfile cnl/fixed_point.h
    ///
    /// \tparam Exponent the \c Exponent parameter of the generated \ref fixed_point type
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    template<typename ArchetypeRep, int Exponent, int Radix, typename Rep>
    struct from_rep<fixed_point<ArchetypeRep, Exponent, Radix>, Rep> {
        /// \brief generates a \ref fixed_point equivalent to \c r in type and value
        constexpr auto operator()(Rep const& r) const
        -> fixed_point<Rep, Exponent, Radix>
        {
            return fixed_point<Rep, Exponent, Radix>(r, 0);
        }
    };
}

#endif // CNL_IMPL_FIXED_POINT_FROM_REP_H
