
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the integer rounding

#if !defined(CNL_BITS_ROUNDING_H)
#define CNL_BITS_ROUNDING_H

#include "operators.h"
#include <cnl/num_traits.h>

/// compositional numeric library
namespace cnl {
    namespace _rounding_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // operators

        template<class RoundingTag, class Operator, class Enable = void>
        struct binary_operator;

        template<class RoundingTag, class Operator, class Enable = void>
        struct comparison_operator;
    }

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // mode tags and objects

        // a reasonable default
        struct nearest_rounding_tag {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::convert

        template<class RoundingTag, class Result, class Input>
        struct convert;

        template<class Result, class Input>
        struct convert<nearest_rounding_tag, Result, Input> {
            constexpr Result operator()(Input const& from) const
            {
                return static_cast<Result>(from+((from>=0) ? .5 : -.5));
            }
        };
    }

    namespace _rounding_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_impl::binary_operator<..., add_op>

        template<class RoundingTag>
        struct binary_operator<RoundingTag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_impl::binary_operator<..., subtract_op>

        template<class RoundingTag>
        struct binary_operator<RoundingTag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_impl::binary_operator<..., multiply_op>

        template<class RoundingTag>
        struct binary_operator<RoundingTag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_impl::binary_operator<nearest_rounding_tag, divide_op>

        template<>
        struct binary_operator<_impl::nearest_rounding_tag, _impl::divide_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return (((lhs<0) ^ (rhs<0))
                        ? lhs-(rhs/2)
                        : lhs+(rhs/2))/rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_impl::binary_operator<nearest_rounding_tag, shift_right_op>

        template<>
        struct binary_operator<_impl::nearest_rounding_tag, _impl::shift_right_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs >> rhs)
            {
                return (lhs+(Lhs{1} << (rhs-1))) >> rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_impl::binary_operator<nearest_rounding_tag, shift_left_op>

        template<>
        struct binary_operator<_impl::nearest_rounding_tag, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs << rhs)
            {
                return lhs << rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::divide<nearest_rounding_tag, ...>

        template<class RoundingTag, class Lhs, class Rhs>
        struct divide {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return _impl::for_rep<decltype(lhs/rhs)>(
                        _rounding_impl::binary_operator<RoundingTag, _impl::divide_op>(), lhs, rhs);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::shift_right

        template<class RoundingTag, class Lhs, class Rhs>
        struct shift_right {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs >> rhs)
            {
                return _rounding_impl::binary_operator<RoundingTag, _impl::shift_right_op>{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_BITS_ROUNDING_H
