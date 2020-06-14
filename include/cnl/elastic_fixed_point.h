
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_fixed_point` type

#if !defined(CNL_ELASTIC_FIXED_POINT_H)
#define CNL_ELASTIC_FIXED_POINT_H

#include "_impl/num_traits/adopt_signedness.h"
#include "elastic_integer.h"
#include "fixed_point.h"
#include "limits.h"  // NOLINT(modernize-deprecated-headers,  hicpp-deprecated-headers)

#include <type_traits>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl-specific definitions

    /// \brief literal real number approximation that uses fixed-point arithmetic and auto-widens to avoid overflow
    ///
    /// \tparam Digits the total number of integer and fractional digits stored
    /// \tparam Exponent the exponent by which the number is scale
    /// \tparam Narrowest the most narrow integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int Digits, int Exponent = 0, class Narrowest = signed>
    using elastic_fixed_point = fixed_point<elastic_integer<Digits, Narrowest>, Exponent>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_fixed_point

    /// \brief generate an \ref cnl::elastic_fixed_point object of given value
    ///
    /// \tparam Narrowest the narrowest type to use as storage
    /// in the resultant \ref cnl::elastic_fixed_point object
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    ///
    /// \return the given value to be represented using an \ref cnl::elastic_fixed_point type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.

    template<
            typename Narrowest = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE Value = 0>
    CNL_NODISCARD constexpr auto
    make_elastic_fixed_point(constant<Value>)
    -> elastic_fixed_point<
            _impl::max(digits<constant<Value>>::value-trailing_bits(Value), 1),
            trailing_bits(Value),
            Narrowest>
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_fixed_point

    ///
    /// \tparam Narrowest the most narrow storage type of the resultant \ref cnl::elastic_fixed_point object
    /// \tparam Integral the type of value
    ///
    /// \param value the value with which to initialize the elastic_fixed_point object
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \brief generate an \ref cnl::elastic_fixed_point object of given value
    template<typename Narrowest = void, typename Integral = int>
    CNL_NODISCARD constexpr auto
    make_elastic_fixed_point(Integral const& value)
    -> elastic_fixed_point<
            numeric_limits<Integral>::digits,
            0,
            typename std::conditional<
                    std::is_same<void, Narrowest>::value,
                    _impl::adopt_signedness_t<int, Integral>,
                    Narrowest>::type>
    {
        return {value};
    }

    template<typename Narrowest = void, typename Rep = int, int Exponent = 0, int Radix = 2>
    CNL_NODISCARD constexpr auto
    make_elastic_fixed_point(fixed_point<Rep, Exponent, Radix> const& value)
    -> elastic_fixed_point<
            numeric_limits<Rep>::digits,
            Exponent,
            typename std::conditional<
                    std::is_same<void, Narrowest>::value,
                    _impl::adopt_signedness_t<int, Rep>,
                    Narrowest>::type>
    {
        return {value};
    }
}

#endif  // CNL_ELASTIC_FIXED_POINT_H
