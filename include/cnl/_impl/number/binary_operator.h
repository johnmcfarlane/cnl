
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_BINARY_OPERATOR_H)
#define CNL_IMPL_NUMBER_BINARY_OPERATOR_H

#include "../num_traits/set_rep.h"
#include "../num_traits/set_tag.h"
#include "../operators/generic.h"
#include "../operators/is_same_tag_family.h"
#include "../operators/native_tag.h"
#include "../operators/operators.h"
#include "../operators/overloads.h"
#include "../type_traits/enable_if.h"
#include "is_number.h"
#include "make_number.h"
#include "operator_helpers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // higher OP number<>
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<std::is_floating_point<Lhs>::value && _impl::is_number<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // number<> OP higher
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_number<Lhs>::value && std::is_floating_point<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP number<>
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::number_can_wrap<Rhs, Lhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // number<> OP lower
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::number_can_wrap<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, _impl::from_value<Lhs>(rhs));
        }
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag,
            Lhs, Rhs,
            _impl::enable_if_t<_impl::is_number<Lhs>::value
                    && _impl::is_number<Rhs>::value
                    && _impl::is_same_tag_family<_impl::tag_t<Lhs>, _impl::tag_t<Rhs>>::value>> {
        using _rep_operator = binary_operator<
                Operator,
                _impl::tag_t<Lhs>, _impl::tag_t<Rhs>,
                _impl::rep_t<Lhs>, _impl::rep_t<Rhs>>;
        using _result_rep = decltype(_rep_operator{}(
                _impl::to_rep(std::declval<Lhs>()),
                _impl::to_rep(std::declval<Rhs>())));
        using _result_tag = _impl::op_result<Operator, _impl::tag_t<Lhs>, _impl::tag_t<Rhs>>;
        using _result_archetype = _impl::set_rep_t<_impl::set_tag_t<Lhs, _result_tag>, _result_rep>;

        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<_result_archetype>(_rep_operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_BINARY_OPERATOR_H
