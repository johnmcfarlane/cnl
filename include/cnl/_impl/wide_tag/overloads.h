
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TAG_TAG_OVERLOADS_H)
#define CNL_IMPL_TAG_TAG_OVERLOADS_H

#include "../num_traits/set_width.h"
#include "../num_traits/width.h"
#include "../numbers/set_signedness.h"
#include "../numbers/signedness.h"
#include "definition.h"

#include <algorithm>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<
                binary_arithmetic_op Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits,
                typename RhsNarrowest>
        struct wide_tag_overload_params {
            static constexpr bool is_signed{
                    numbers::signedness_v<LhsNarrowest> | numbers::signedness_v<RhsNarrowest>};
            static constexpr int digits{std::max(LhsDigits, RhsDigits)};

            using narrowest = _impl::set_width_t<
                    numbers::set_signedness_t<
                            _impl::op_result<Operator, LhsNarrowest, RhsNarrowest>, is_signed>,
                    std::max(
                            _impl::width<LhsNarrowest>, _impl::width<RhsNarrowest>)>;

            using type = cnl::wide_tag<digits, narrowest>;
        };
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    [[nodiscard]] constexpr auto operator+(
            cnl::wide_tag<LhsDigits, LhsNarrowest>, cnl::wide_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::wide_tag_overload_params<
                    _impl::add_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::wide_tag_overload_params<
                _impl::add_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    [[nodiscard]] constexpr auto operator-(
            cnl::wide_tag<LhsDigits, LhsNarrowest>, cnl::wide_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::wide_tag_overload_params<
                    _impl::subtract_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::wide_tag_overload_params<
                _impl::subtract_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    [[nodiscard]] constexpr auto operator*(
            cnl::wide_tag<LhsDigits, LhsNarrowest>, cnl::wide_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::wide_tag_overload_params<
                    _impl::multiply_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::wide_tag_overload_params<
                _impl::multiply_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    [[nodiscard]] constexpr auto operator/(
            cnl::wide_tag<LhsDigits, LhsNarrowest>, cnl::wide_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::wide_tag_overload_params<
                    _impl::divide_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::wide_tag_overload_params<
                _impl::divide_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    [[nodiscard]] constexpr auto operator%(
            cnl::wide_tag<LhsDigits, LhsNarrowest>, cnl::wide_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::wide_tag_overload_params<
                    _impl::modulo_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::wide_tag_overload_params<
                _impl::modulo_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }

    template<int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    [[nodiscard]] constexpr auto operator&(
            cnl::wide_tag<LhsDigits, LhsNarrowest>, cnl::wide_tag<RhsDigits, RhsNarrowest>) ->
            typename _impl::wide_tag_overload_params<
                    _impl::bitwise_and_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type
    {
        return typename _impl::wide_tag_overload_params<
                _impl::bitwise_and_op, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::type{};
    }
}

#endif  // CNL_IMPL_TAG_TAG_OVERLOADS_H
