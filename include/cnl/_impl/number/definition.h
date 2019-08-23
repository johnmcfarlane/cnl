
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_TYPE_H)
#define CNL_IMPL_NUMBER_TYPE_H

#include "can_convert_tag_family.h"
#include "is_number.h"
#include "to_rep.h"
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
            number() = default;

        protected:
            /// constructor taking the rep type
            constexpr number(Rep const& r, int)
                    : _rep(r)
            {
            }

        public:
            /// constructor taking a related _impl::number type
            template<typename RhsRep, class RhsTag,
                    enable_if_t<can_convert_tag_family<Tag, RhsTag>::value, int> = 0>
            constexpr number(number<RhsRep, RhsTag> const& i)
                    : _rep(convert<Tag, RhsTag, Rep>(to_rep(i)))
            {
            }

            /// constructor taking an unrelated _impl::number type
            template<typename RhsRep, class RhsTag,
                    enable_if_t<!can_convert_tag_family<Tag, RhsTag>::value, int> = 0>
            constexpr number(number<RhsRep, RhsTag> const& i)
                    : _rep(convert<Tag, _impl::native_tag, Rep>(i))
            {
            }

            /// constructor taking a number type that isn't _impl::number
            template<class S, enable_if_t<!is_number<S>::value, int> Dummy = 0>
            constexpr number(S const& s)
            : _rep(convert<Tag, _impl::native_tag, Rep>(s))
            {
            }

            template<class S, enable_if_t<!is_number<S>::value, int> Dummy = 0>
            CNL_NODISCARD constexpr explicit operator S() const
            {
                return convert<_impl::native_tag, Tag, S>(_rep);
            }

            CNL_NODISCARD explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

            template<typename T, class Enable>
            friend struct cnl::to_rep;
        private:
            Rep _rep;
        };
    }
}

#endif  // CNL_IMPL_NUMBER_TYPE_H
