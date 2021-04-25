
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_FROM_REP_H)
#define CNL_IMPL_SCALED_INTEGER_FROM_REP_H

#include "../num_traits/from_rep.h"
#include "../wrapper/declaration.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref scaled_integer specialization of \ref from_rep
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Exponent the \c Exponent parameter of the generated \ref scaled_integer type
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    template<integer ArchetypeRep, tag Scale, typename Rep>
    requires is_scaled_tag_v<Scale>
    struct from_rep<scaled_integer<ArchetypeRep, Scale>, Rep> {
        using result_type =
                _impl::set_rep_t<scaled_integer<ArchetypeRep, Scale>, Rep>;
        /// \brief generates a \ref scaled_integer equivalent to \c r in type and value
        [[nodiscard]] constexpr auto operator()(Rep const& r) const -> result_type
        {
            return result_type(r, 0);
        }
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_FROM_REP_H
