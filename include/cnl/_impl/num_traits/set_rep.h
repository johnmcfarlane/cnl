
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_SET_REP_H)
#define CNL_IMPL_NUM_TRAITS_SET_REP_H

/// compositional numeric library
namespace cnl {
    /// \brief meta-function object that transforms a component from one Rep type to another
    ///
    /// \tparam T component to transform
    /// \tparam OutRep new representational type being wrapped by the resultant type
    ///
    /// \sa to_rep, from_value, set_tag, rep
    template<typename T, typename OutRep>
    struct set_rep;

    namespace _impl {
        template<typename T, class OutRep>
        using set_rep_t = typename set_rep<T, OutRep>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_REP_H
