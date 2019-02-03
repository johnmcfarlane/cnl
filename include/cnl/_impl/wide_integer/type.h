
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_TYPE_H)
#define CNL_IMPL_WIDE_INTEGER_TYPE_H

#include "../duplex_integer.h"
#include "../integer/type.h"
#include "../number_base.h"
#include "forward_declaration.h"
#include "rep.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
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
