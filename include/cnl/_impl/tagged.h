
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief free functions performing customizable operations on arithmetic types

#ifndef CNL_IMPL_TAGGED_H
#define CNL_IMPL_TAGGED_H

/// compositional numeric library
namespace cnl {
    template<class Tag, typename Result, typename Input>
    constexpr auto convert(Input const& from)
    -> decltype(cnl::_impl::tagged_convert_operator<Tag, Result, Input>{}(from))
    {
        return cnl::_impl::tagged_convert_operator<Tag, Result, Input>{}(from);
    }

    template<class Tag, typename Lhs, typename Rhs>
    constexpr auto add(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_impl::tagged_binary_operator<Tag, _impl::add_op>{}(lhs, rhs))
    {
        return _impl::tagged_binary_operator<Tag, _impl::add_op>{}(lhs, rhs);
    }

    template<class Tag, typename Lhs, typename Rhs>
    constexpr auto subtract(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_impl::tagged_binary_operator<Tag, _impl::subtract_op>{}(lhs, rhs))
    {
        return _impl::tagged_binary_operator<Tag, _impl::subtract_op>{}(lhs, rhs);
    }

    template<class Tag, typename Lhs, typename Rhs>
    constexpr auto multiply(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_impl::tagged_binary_operator<Tag, _impl::multiply_op>{}(lhs, rhs))
    {
        return _impl::tagged_binary_operator<Tag, _impl::multiply_op>{}(lhs, rhs);
    }

    template<class Tag, typename Dividend, typename Divisor>
    constexpr auto divide(Dividend const& dividend, Divisor const& divisor)
    -> decltype(cnl::_impl::tagged_binary_operator<Tag, _impl::divide_op>{}.template operator()<Dividend, Divisor>(dividend, divisor))
    {
        return cnl::_impl::tagged_binary_operator<Tag, _impl::divide_op>{}.template operator()<Dividend, Divisor>(dividend, divisor);
    }

    template<class Tag, typename Lhs, typename Rhs>
    constexpr auto shift_left(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_impl::tagged_binary_operator<Tag, _impl::shift_left_op>{}(lhs, rhs))
    {
        return _impl::tagged_binary_operator<Tag, _impl::shift_left_op>{}(lhs, rhs);
    }

    template<class Tag, typename Lhs, typename Rhs>
    constexpr auto shift_right(Lhs const& lhs, Rhs const& rhs)
    -> decltype(cnl::_impl::tagged_binary_operator<Tag, _impl::shift_right_op>{}.template operator()<Lhs, Rhs>(lhs, rhs))
    {
        return cnl::_impl::tagged_binary_operator<Tag, _impl::shift_right_op>{}.template operator()<Lhs, Rhs>(lhs, rhs);
    }
}

#endif  // CNL_IMPL_TAGGED_H
