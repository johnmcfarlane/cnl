
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE)
#define CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE

#include "../type_traits/enable_if.h"
#include "digits.h"
#include "from_rep.h"
#include "is_composite.h"
#include "scale.h"
#include "to_rep.h"

namespace cnl {
    /// \brief returns a scaled value of the same type
    /// \headerfile cnl/scaled_integer.h
    template<int Digits, int Radix, class Scalar, class Enable=void>
    struct fixed_width_scale;

    /// \private specialization for non-composite types
    template<int Digits, int Radix, typename S>
    struct fixed_width_scale<Digits, Radix, S, _impl::enable_if_t<!is_composite<S>::value>> {
        CNL_NODISCARD constexpr S operator()(S const& s) const
        {
            static_assert(
                    Radix!=2||digits<S>::value>-Digits,
                    "this operation will flush the given value");

            return static_cast<S>(scale<Digits, Radix, S>()(s));
        }
    };

    namespace _impl {
        template<int Digits, int Radix=2, class S=void>
        CNL_NODISCARD constexpr S fixed_width_scale(S const& s)
        {
            return cnl::fixed_width_scale<Digits, Radix, S>()(s);
        }
    }

    /// \private specialization for composite types
    template<int Digits, int Radix, typename Composite>
    struct fixed_width_scale<
            Digits, Radix, Composite,
            _impl::enable_if_t<is_composite<Composite>::value>> {
        CNL_NODISCARD constexpr Composite operator()(Composite const& s) const
        {
            return _impl::from_rep<Composite>(_impl::fixed_width_scale<Digits, Radix>(_impl::to_rep(s)));
        }
    };
}

#endif  // CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE
