
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_TYPE_H)
#define CNL_IMPL_INTEGER_TYPE_H

#include "can_convert_tag_family.h"
#include "is_integer.h"
#include "../num_traits/from_value.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/tagged.h"
#include "../type_traits/enable_if.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // just an integer; useful when you just want to use a fundamental integer but for some reason, a class is
        // required, e.g.: `class X : public int`
        template<typename Rep=int, class Tag=native_tag>
        class integer {
        public:
            using rep = Rep;
            using tag = Tag;

            integer() = default;

            /// constructor taking a related _impl::integer type
            template<typename RhsRep, class RhsTag,
                    enable_if_t<can_convert_tag_family<Tag, RhsTag>::value, int> = 0>
            constexpr integer(integer<RhsRep, RhsTag> const& i)
                    : _rep(convert<Tag, RhsTag, Rep>(to_rep(i)))
            {
            }

            /// constructor taking an unrelated _impl::integer type
            template<typename RhsRep, class RhsTag,
                    enable_if_t<!can_convert_tag_family<Tag, RhsTag>::value, int> = 0>
            constexpr integer(integer<RhsRep, RhsTag> const& i)
                    : _rep(convert<Tag, _impl::native_tag, Rep>(i))
            {
            }

            /// constructor taking an integer type
            template<class S, enable_if_t<
                    (numeric_limits<S>::is_integer&&!is_integer<S>::value)||is_constant<S>::value,
                    int> Dummy = 0>
            constexpr integer(S const& s)
            : _rep(convert<tag, _impl::native_tag, Rep>(_impl::from_value<Rep>(s)))
            {
            }

            /// constructor taking a floating-point type
            template<class S, enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
            constexpr integer(S s)
            : _rep(convert<tag, _impl::native_tag, rep>(s))
            {
            }

            template<class S, enable_if_t<!is_integer<S>::value, int> Dummy = 0>
            CNL_NODISCARD constexpr explicit operator S() const
            {
                return convert<Tag, _impl::native_tag, S>(_impl::to_rep(*this));
            }

            CNL_NODISCARD explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

            friend struct cnl::to_rep<integer>;
        private:
            rep _rep;
        };
    }
}

#endif  // CNL_IMPL_INTEGER_TYPE_H
