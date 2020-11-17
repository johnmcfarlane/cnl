
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_LIMITS_H)
#define CNL_IMPL_ELASTIC_INTEGER_LIMITS_H

#include "../../limits.h"
#include "../limits/lowest.h"
#include "definition.h"
#include "from_rep.h"
#include "rep.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest>>
            : numeric_limits<Narrowest> {
    private:
        // elastic integer-specific helpers
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = elastic_integer<Digits, Narrowest>;
        using _rep = _impl::rep_t<_value_type>;
        using _rep_numeric_limits = numeric_limits<_rep>;

        CNL_NODISCARD static constexpr _rep rep_max() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
        }
    public:

        // standard members
        static constexpr int digits = Digits;

        CNL_NODISCARD static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }

        CNL_NODISCARD static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(rep_max());
        }

        CNL_NODISCARD static constexpr _value_type lowest() noexcept
        {
            return _impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(rep_max());
        }
    };

    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest> const>
            : numeric_limits<elastic_integer<Digits, Narrowest>> {
    };
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_LIMITS_H
