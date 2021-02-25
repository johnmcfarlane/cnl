
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_DEFINITION_H)
#define CNL_IMPL_WRAPPER_DEFINITION_H

#include <utility>

#include "../num_traits/from_value.h"
#include "../operators/custom_operator.h"
#include "../operators/tagged.h"
#include "can_convert_tag_family.h"
#include "declaration.h"
#include "is_wrapper.h"
#include "tag_of.h"
#include "to_rep.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // a numeric type parameterized on storage and behavior
        template<integer Rep, tag Tag>
        class wrapper {
        public:
            wrapper() = default;

        protected:
            /// constructor taking the rep type
            constexpr wrapper(Rep r, int)
                : _rep(std::move(std::move(r)))
            {
            }

        public:
            /// constructor taking a related _impl::wrapper type
            template<typename RhsRep, tag RhsTag>
            requires can_convert_tag_family<Tag, RhsTag>::value
                    // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
                    constexpr wrapper(wrapper<RhsRep, RhsTag> const& i)
                : _rep(convert<Tag, RhsTag, Rep>(to_rep(i)))
            {
            }

            /// constructor taking an unrelated _impl::wrapper type
            template<_impl::any_wrapper Number>
            requires(!can_convert_tag_family<Tag, tag_of_t<Number>>::value)
                    // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
                    constexpr wrapper(Number const& i)
                : _rep(convert<Tag, _impl::native_tag, Rep>(i))
            {
            }

            /// constructor taking a number type that isn't _impl::wrapper
            template<class S>
            requires(!is_wrapper<S>)
                    // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
                    constexpr wrapper(S const& s)
                : _rep(convert<Tag, _impl::native_tag, Rep>(s))

            {
            }

            template<class S>
            requires(!is_wrapper<S>)
                    [[nodiscard]] constexpr explicit
                    operator S() const
            {
                return convert<_impl::native_tag, Tag, S>(_rep);
            }

            [[nodiscard]] explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

            template<typename T>
            friend struct cnl::to_rep;

        private:
            Rep _rep;
        };
    }
}

#endif  // CNL_IMPL_WRAPPER_DEFINITION_H
