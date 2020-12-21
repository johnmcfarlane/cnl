
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_OVERLOADS_H)
#define CNL_IMPL_ELASTIC_TAG_OVERLOADS_H

#include "../num_traits/set_width.h"
#include "../num_traits/width.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"
#include "definition.h"
#include "policy.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<
                class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits,
                typename RhsNarrowest>
        struct elastic_tag_overload_params {
            using policy = typename _impl::policy<
                    Operator, LhsDigits, numeric_limits<LhsNarrowest>::is_signed, RhsDigits,
                    numeric_limits<RhsNarrowest>::is_signed>;
            using narrowest = _impl::set_width_t<
                    _impl::set_signedness_t<
                            _impl::op_result<Operator, LhsNarrowest, RhsNarrowest>,
                            policy::is_signed>,
                    _impl::max(
                            _impl::width<LhsNarrowest>::value, _impl::width<RhsNarrowest>::value)>;

            using type = elastic_tag<policy::digits, narrowest>;
        };
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    CNL_NODISCARD constexpr auto operator+(
            elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::elastic_tag_overload_params<
                    _impl::add_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::elastic_tag_overload_params<
                _impl::add_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    CNL_NODISCARD constexpr auto operator-(
            elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::elastic_tag_overload_params<
                    _impl::subtract_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::elastic_tag_overload_params<
                _impl::subtract_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    CNL_NODISCARD constexpr auto operator*(
            elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::elastic_tag_overload_params<
                    _impl::multiply_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::elastic_tag_overload_params<
                _impl::multiply_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    CNL_NODISCARD constexpr auto operator/(
            elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::elastic_tag_overload_params<
                    _impl::divide_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::elastic_tag_overload_params<
                _impl::divide_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    CNL_NODISCARD constexpr auto operator%(
            elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::elastic_tag_overload_params<
                    _impl::modulo_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::elastic_tag_overload_params<
                _impl::modulo_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    CNL_NODISCARD constexpr auto operator&(
            elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::elastic_tag_overload_params<
                    _impl::bitwise_and_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::elastic_tag_overload_params<
                _impl::bitwise_and_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }
}

#endif  // CNL_IMPL_ELASTIC_TAG_OVERLOADS_H
