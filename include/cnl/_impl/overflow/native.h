
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_NATIVE_H)
#define CNL_IMPL_OVERFLOW_NATIVE_H

#include "../custom_operator/native_tag.h"
#include "../polarity.h"
#include "is_overflow_tag.h"
#include "is_tag.h"
#include "overflow_operator.h"

/// compositional numeric library
namespace cnl {
    /// \brief tag to match the overflow behavior of fundamental arithmetic types
    ///
    /// Arithmetic operations using this tag should behave the same as equivalent operators.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::convert, cnl::divide, cnl::left_shift,
    /// cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag
    struct native_overflow_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_overflow_tag<native_overflow_tag> : std::true_type {
        };

        template<typename Operator, polarity Polarity>
        struct overflow_operator<Operator, native_overflow_tag, Polarity> : Operator {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_NATIVE_H
