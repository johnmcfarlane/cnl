
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_SET_TAG_H)
#define CNL_IMPL_NUM_TRAITS_SET_TAG_H

/// compositional numeric library
namespace cnl {
    /// \brief meta-function object that transforms a component from one Tag type to another
    ///
    /// \tparam T component to transform
    /// \tparam OutRep new behavior type being wrapped by the resultant type
    ///
    /// \sa to_rep, from_rep, set_rep, tag
    template<typename T, class OutTag, class Enable = void>
    struct set_tag;

    namespace _impl {
        template<typename T, class OutTag>
        using set_tag_t = typename set_tag<T, OutTag>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_TAG_H
