
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential `fractional` definitions related to the `fixed_point` type

#if !defined(CNL_FIXED_POINT_FRACTIONAL_H)
#define CNL_FIXED_POINT_FRACTIONAL_H 1

#include "named.h"

#include <cnl/fractional.h>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_fixed_point<cnl::fractional<>>

    /// \brief makes a \ref fixed_point object from a given \ref fractional
    ///
    /// \tparam Numerator the type of the numerator of the fractional
    /// \tparam Denominator the type of the denominator of the fractional
    ///
    /// \param value the fractionalfrom which to make the \ref fixed_point object
    ///
    /// \note This function is deprecated after C++17
    /// in favor of class template deduction.
    template<typename Numerator, typename Denominator>
    constexpr auto make_fixed_point(fractional<Numerator, Denominator> const& value)
    -> decltype(divide(value.numerator, value.denominator))
    {
        return divide(value.numerator, value.denominator);
    }


    template<typename Rep, int Exponent>
    template<typename Numerator, typename Denominator>
    constexpr fixed_point<Rep, Exponent>::fixed_point(fractional<Numerator, Denominator> const& f)
    : fixed_point(divide(f.numerator, f.denominator))
    {
    }

#if defined(__cpp_deduction_guides)
    template<typename Numerator, typename Denominator>
    fixed_point(fractional<Numerator, Denominator>)
    -> fixed_point<
            typename decltype(cnl::divide(std::declval<Numerator>(), std::declval<Denominator>()))::rep,
            decltype(cnl::divide(std::declval<Numerator>(), std::declval<Denominator>()))::exponent>;
#endif
}

#endif  // CNL_FIXED_POINT_FRACTIONAL_H
