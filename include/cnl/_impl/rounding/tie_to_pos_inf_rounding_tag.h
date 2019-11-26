
//          Copyright Heikki Berg 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_TIE_TO_POS_INF_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_TIE_TO_POS_INF_ROUNDING_TAG_H

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify tie to positive inf rounding behavior in arithmetic operations
    ///
    /// Arithmetic operations using this tag round round the mid-point value to positive infinity direction
    /// and round all other values to their nearest representable value.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::tie_to_pos_inf_rounding_tag
    struct tie_to_pos_inf_rounding_tag {
    };
}

#endif  // CNL_IMPL_ROUNDING_TIE_TO_POS_INF_ROUNDING_TAG_H
