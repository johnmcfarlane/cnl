
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_NATIVE_H)
#define CNL_IMPL_OVERFLOW_NATIVE_H

#include "common.h"
#include "overflow_operator.h"
#include "../polarity.h"
#include "../operators/native_tag.h"

/// compositional numeric library
namespace cnl {
    /// \brief tag to match the overflow behavior of fundamental arithmetic types
    ///
    /// Arithmetic operations using this tag should behave the same as equivalent operators.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag, cnl::undefined_overflow_tag
    using native_overflow_tag = _impl::native_tag;
}

#endif  // CNL_IMPL_OVERFLOW_NATIVE_H
