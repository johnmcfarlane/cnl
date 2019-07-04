
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_TYPE_H)
#define CNL_IMPL_NUMBER_TYPE_H

#include "is_number.h"
#include "../num_traits/from_value.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/tagged.h"
#include "../type_traits/enable_if.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // a numeric type parameterized on storage and behavior
        template<typename Rep=int, class Tag=native_tag>
        class number {
        public:
            using rep = Rep;
            using tag = Tag;

            number() = default;

            /// constructor taking an _impl::number type
            template<typename RhsRep>
            constexpr number(number<RhsRep, Tag> const& i)
                    : _rep(convert<tag, Rep>(to_rep(i)))
            {
            }

            /// constructor taking a number type that isn't _impl::number
            template<class S, enable_if_t<!is_number<S>::value, int> Dummy = 0>
            constexpr number(S const& s)
            : _rep(convert<tag, Rep>(s))
            {
            }

            template<class S>
            CNL_NODISCARD explicit constexpr operator S() const
            {
                return static_cast<S>(_impl::to_rep(*this));
            }

            friend struct cnl::to_rep<number>;
        private:
            rep _rep;
        };
    }
}

#endif  // CNL_IMPL_NUMBER_TYPE_H
