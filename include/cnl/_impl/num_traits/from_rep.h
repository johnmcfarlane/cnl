
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_FROM_REP)
#define CNL_IMPL_NUM_TRAITS_FROM_REP

#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"

namespace cnl {
    /// \brief generic function object that returns the number encapsulating a given value
    ///
    /// \tparam Number archetype for the encapsulating type
    ///
    /// \note Rather than returning Number, invocation may return an alternative
    /// template instantiation based on input parameter.
    /// \sa to_rep, from_value
    template<typename Number, typename Rep, class Enable = void>
    struct from_rep;

    /// \brief Specialization of \ref from_rep for integer types
    ///
    /// \tparam Number fundamental integer type to return
    ///
    /// \note This specialization *does* return integers of type, \c Number
    /// \sa to_rep, from_value
    template<typename Number, typename Rep>
    struct from_rep<Number, Rep, _impl::enable_if_t<cnl::_impl::is_integral<Number>::value>> {
        CNL_NODISCARD constexpr Number operator()(Rep const& rep) const {
            // by default, a number type's rep type is the number type itself
            return static_cast<Number>(rep);
        }
    };

    namespace _impl {
        template<class Number, class Rep>
        CNL_NODISCARD constexpr auto from_rep(Rep const& rep)
        {
            return cnl::from_rep<Number, Rep>{}(rep);
        }
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_FROM_REP
