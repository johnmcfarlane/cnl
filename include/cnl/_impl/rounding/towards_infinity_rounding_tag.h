
//          Copyright Heikki Berg 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_TOWARDS_INFINITY_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_TOWARDS_INFINITY_ROUNDING_TAG_H

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify towards infinity rounding behavior in arithmetic operations
    ///
    /// Arithmetic operations using this tag round to nearest representatable value towards infinity in
    /// situations where the precise value cannot be represented.
    /// Differs from nearest rounding on negative values exactly at midpoint -n.5, where nearest rounding rounds
    /// downward towards negative infinity -(n+1) and towards infinity rounds upwards -n.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::towards_infinity_rounding_tag
    struct towards_infinity_rounding_tag {
    };
}

#endif  // CNL_IMPL_ROUNDING_TOWARDS_INFINITY_ROUNDING_TAG_H
