
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H

#include "../native_tag.h"

/// compositional numeric library
namespace cnl {
    /// \brief tag to match the rounding behavior of fundamental arithmetic types
    ///
    /// Arithmetic operations using this tag should behave the same as equivalent operators.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::nearest_rounding_tag
    struct native_rounding_tag : public _impl::native_tag {
    };
}

#endif  // CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H
