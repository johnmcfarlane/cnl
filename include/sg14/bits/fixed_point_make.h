
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions of `sg14::make_fixed` and `sg14::make_ufixed`

#if !defined(SG14_MAKE_FIXED_H)
#define SG14_MAKE_FIXED_H 1

#include "fixed_point_type.h"

#include <sg14/bits/type_traits.h>

/// study group 14 of the C++ working group
namespace sg14 {

    /// \brief specializes \ref fixed_point with the given number of integer and fractional digits
    /// \headerfile sg14/fixed_point
    ///
    /// \tparam IntegerDigits specifies minimum value of @ref fixed_point::integer_digits
    /// \tparam FractionalDigits specifies the exact value of @ref fixed_point::fractional_digits
    /// \tparam Narrowest hints at the type of @ref fixed_point::rep
    ///
    /// \remarks The signage of \a Narrowest specifies signage of the resultant fixed-point type.
    /// \remarks Typical choices for \a Narrowest, `signed` and `unsigned`,
    /// result in a type that uses built-in integers for \a fixed_point::rep.
    /// \remarks Resultant type is signed by default.
    ///
    /// \par Example:
    ///
    /// To generate a fixed-point type with a sign bit, 8 fractional bits and at least 7 integer bits:
    /// \snippet snippets.cpp use make_fixed
    ///
    /// \sa make_ufixed
    template<int IntegerDigits, int FractionalDigits = 0, class Narrowest = signed>
    using make_fixed = fixed_point<
            set_digits_t<Narrowest, IntegerDigits+FractionalDigits>,
            -FractionalDigits>;

    /// \brief specializes \ref fixed_point with the given number of integer and fractional digits; produces an unsigned type
    /// \headerfile sg14/fixed_point
    ///
    /// \sa make_fixed
    template<int IntegerDigits, int FractionalDigits = 0, class Narrowest = unsigned>
    using make_ufixed = make_fixed<
            IntegerDigits,
            FractionalDigits,
            typename make_unsigned<Narrowest>::type>;
}

#endif	// SG14_MAKE_FIXED_H
