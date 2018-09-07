
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_TYPE_H)
#define CNL_IMPL_WIDE_INTEGER_TYPE_H

#include "../integer/type.h"
#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "../number_base.h"
#include "../type_traits/enable_if.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<int Digits = digits<int>::value, typename Narrowest = int, class Enable = void>
        class wide_integer;

        template<int Digits, typename Narrowest>
        class wide_integer<Digits, Narrowest, enable_if_t<max_digits<Narrowest>::value>=Digits>>
                : public number_base<
                        wide_integer<Digits, Narrowest>,
                        set_digits_t<Narrowest, max(Digits, digits<Narrowest>::value)>> {
            using _base = number_base<
                    wide_integer<Digits, Narrowest>,
                    set_digits_t<Narrowest, max(Digits, digits<Narrowest>::value)>>;
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
