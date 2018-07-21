
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_BITS_NUM_TRAITS_TO_REP
#define CNL_BITS_NUM_TRAITS_TO_REP

#include <type_traits>

namespace cnl {
    /// \brief Returns the value encapsulated in \c number
    /// \param number the 'outer' object
    /// \return the 'inner' value
    /// \sa from_rep, from_value
    template<class Number>
    constexpr Number to_rep(Number const& number) {
        return number;
    }

    namespace _impl {
        using cnl::to_rep;

        template<class Number>
        using to_rep_t = decltype(to_rep(std::declval<Number>()));
    }
}

#endif  // CNL_BITS_NUM_TRAITS_TO_REP
