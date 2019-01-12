
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_TYPE_H)
#define CNL_IMPL_WIDE_INTEGER_TYPE_H

#include "../duplex_integer/instantiate_duplex_integer.h"
#include "../duplex_integer/numeric_limits.h"
#include "../duplex_integer/rounding.h"
#include "../integer/type.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "../number_base.h"
#include "../type_traits/enable_if.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // wide_integer_rep
        template<int Digits, typename Narrowest, typename Enable = void>
        struct wide_integer_rep;

        template<int Digits, typename Narrowest>
        struct wide_integer_rep<Digits, Narrowest, enable_if_t<(max_digits<Narrowest>::value>=Digits)>>
                : set_digits<Narrowest, max(Digits, digits<Narrowest>::value)> {
        };

        template<int Digits, typename Narrowest>
        struct wide_integer_rep<Digits, Narrowest, enable_if_t<(max_digits<Narrowest>::value<Digits)>>
                : instantiate_duplex_integer<Digits, Narrowest> {
        };

        template<int Digits, typename Narrowest>
        using wide_integer_rep_t = typename wide_integer_rep<Digits, Narrowest>::type;

        // wide_integer
        template<int Digits, typename Narrowest>
        class wide_integer
                : public number_base<
                        wide_integer<Digits, Narrowest>,
                        wide_integer_rep_t<Digits, Narrowest>> {
            using _base = number_base<
                    wide_integer<Digits, Narrowest>,
                    wide_integer_rep_t<Digits, Narrowest>>;
        public:
            using rep = typename _base::rep;

            wide_integer() = default;

            template<typename T>
            constexpr wide_integer(T const& value)
                    : _base(static_cast<rep>(value)) { }

            template<class S>
            explicit constexpr operator S() const
            {
                return static_cast<S>(_impl::to_rep(*this));
            }
        };
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_TYPE_H
