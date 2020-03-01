
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER__H)
#define CNL_IMPL_ELASTIC_INTEGER__H

#include "../num_traits/from_rep.h"
#include "../num_traits/rep.h"
#include "../num_traits/scale.h"
#include "../num_traits/to_rep.h"
#include "../type_traits/enable_if.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \private right-shifting elastic_integer specialization
    template<int ShiftDigits, int ScaleRadix, int ScalarDigits, class ScalarNarrowest>
    struct scale<
            ShiftDigits,
            ScaleRadix,
            elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(0 <= ShiftDigits)>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>;
            using result_rep = _impl::rep_t<result_type>;
            return _impl::from_rep<result_type>(
                    scale<ShiftDigits, ScaleRadix, result_rep>()(_impl::to_rep(s)));
        }
    };

    /// \private left-shifting elastic_integer specialization
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct scale<
            ShiftDigits,
            2,
            elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(ShiftDigits < 0)>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using divisor_type = elastic_integer<1-ShiftDigits, ScalarNarrowest>;
            using divisor_rep = _impl::rep_t<divisor_type>;
            return _impl::to_rep(s) / (divisor_rep{1} << -ShiftDigits);
        }
    };
}

#endif  // CNL_IMPL_ELASTIC_INTEGER__H
