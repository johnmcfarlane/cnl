
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_TAG_OF_H)
#define CNL_IMPL_NUM_TRAITS_TAG_OF_H

/// compositional numeric library
namespace cnl {
    /// \brief meta-function object that returns the Tag type of a component
    ///
    /// \tparam T component whose behavioral tag type to return
    ///
    /// \note The behavioral tag or 'Tag' type of a numeric component is central to CNL's method of
    /// delivering an arithmetic API.
    ///
    /// \sa set_tag, rep_of
    template<typename T>
    struct tag_of;

    namespace _impl {
        template<typename T>
        using tag_of_t = typename tag_of<T>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_TAG_OF_H
