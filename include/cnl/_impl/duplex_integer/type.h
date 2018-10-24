
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_TYPE_H)
#define CNL_IMPL_DUPLEX_INTEGER_TYPE_H 1

#include "../cmath/remainder.h"
#include "../operators.h"
#include "../type_traits/set_signedness.h"
#include "digits.h"
#include "forward_declaration.h"

#include <cmath>
#include <iterator>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        class duplex_integer {
            using upper_type = Upper;
            using lower_type = Lower;
        public:
            duplex_integer() = default;

            template<typename Integer,
                    _impl::enable_if_t<(numeric_limits<Integer>::is_integer
                            && digits<Integer>::value<=digits<lower_type>::value), int> Dummy = 0>
            constexpr duplex_integer(Integer const& i)
                    : _upper((i<0)?-1:0)
                    , _lower(abs(i)) {
            }

            template<typename Integer,
                    _impl::enable_if_t<(numeric_limits<Integer>::is_integer
                            && digits<Integer>::value>digits<lower_type>::value), int> Dummy = 0>
            constexpr duplex_integer(Integer const& i)
                    : _upper{static_cast<upper_type>(remainder(i, static_cast<Integer>(numeric_limits<lower_type>::max())+1))}
                    , _lower{static_cast<lower_type>(i / (static_cast<Integer>(numeric_limits<lower_type>::max())+1))} {
            }

            constexpr auto upper() const -> upper_type const&
            {
                return _upper;
            }

            constexpr auto lower() const -> lower_type const&
            {
                return _lower;
            }

            explicit constexpr operator bool() const { return _lower || _upper; }
        private:
            // value == _upper * (numeric_limits<lower_type>::max()+1) + 1
            upper_type _upper;
            lower_type _lower;
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_TYPE_H
