
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::scaled_integer` type

#if !defined(CNL_IMPL_SCALED_INTEGER_DEFINITION_H)
#define CNL_IMPL_SCALED_INTEGER_DEFINITION_H

#include "../../fraction.h"
#include "../../numeric.h"
#include "../integer.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_digits.h"
#include "../number.h"
#include "../scaled/inc_dec_operator.h"
#include "../scaled/power.h"
#include "../used_digits.h"
#include "declaration.h"
#include "is_scaled_integer.h"

/// compositional numeric library
namespace cnl {
    template<typename Numerator, typename Denominator>
    struct fraction;

    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Rep the underlying type used to represent the value; defaults to `int`
    /// \tparam Exponent the exponent used to scale the integer value; defaults to `0`
    /// \tparam Radix the base used to scale the integer value; defaults to radix of `Rep`
    ///
    /// Uses an integer to approximate a real number.
    /// Scales the integer by `pow(Radix, Exponent)` to produce the scaled number.
    /// By default, represents an `int` with no fractional digits using binary arithmetic.
    ///
    /// \note Currently, only binary support and partial decimal support is tested.
    ///
    /// \note The type makes no effort to avoid errors resulting from overflow or underflow.
    /// Consider specializing with:
    /// * \ref rounding_integer to reduce precision loss;
    /// * \ref elastic_integer to avoid out-of-range errors;
    /// * \ref overflow_integer to detect out-of-range errors; and
    /// * \ref static_integer to combine all of the above.
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 4 integer bits and 3 fractional bits:
    /// \snippet snippets.cpp define a scaled_integer value

    template<typename Rep, class Scale>
    class scaled_integer
            : public _impl::number<Rep, Scale> {
        static_assert(!_impl::is_scaled_integer<Rep>::value,
                "scaled_integer of scaled_integer is not a supported");

        using _base = _impl::number<Rep, Scale>;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

    private:
        // constructor taking representation explicitly using operator++(int)-style trick
        constexpr scaled_integer(Rep r, int)
                :_base(r, 0)
        {
        }

    public:
        scaled_integer() = default;

        /// constructor not taking cnl::fraction
        template<typename Number>
        constexpr scaled_integer(Number const& n)  // NOLINT(hicpp-explicit-conversions, google-explicit-constructor)
                : _base(n) { }

        /// copy assignement operator
        template<typename S>
        constexpr scaled_integer& operator=(S const& rhs)
        {
            _base::operator=(rhs);
            return *this;
        }

        /// creates an instance given the underlying representation value
        template<typename, typename, typename>
        friend struct from_rep;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::scaled_integer::scaled_integer deduction guides

#if defined(__cpp_deduction_guides)
    // same as cnl::make_scaled_integer
    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    scaled_integer(::cnl::constant<Value>)
    -> scaled_integer<
            set_digits_t<int, _impl::max(digits_v<int>, _impl::used_digits(Value)-trailing_bits(Value))>,
            power<trailing_bits(Value)>>;

    template<class Integer>
    scaled_integer(Integer)
    -> scaled_integer<Integer, power<>>;
#endif
}

#endif  // CNL_IMPL_SCALED_INTEGER_DEFINITION_H
