
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
}

#endif  // CNL_IMPL_TAGGED_H
