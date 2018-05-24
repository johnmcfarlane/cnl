
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief cnl::_impl::convert and related definitions

#if !defined(CNL_BITS_CONVERT_H)
#define CNL_BITS_CONVERT_H

////////////////////////////////////////////////////////////////////////////////
// tag error message

template<typename Type>
struct CNL_ERROR___cannot_use {
    // It you see this type in an error message,
    // you probably passed a non-tag type as a tag parameter.
    struct as_a_tag;
};

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::convert primary template

        template<class RoundingTag, class Result, class Input>
        struct convert : public CNL_ERROR___cannot_use<RoundingTag>::as_a_tag {
        };
    }
}

#endif  // CNL_BITS_CONVERT_H
