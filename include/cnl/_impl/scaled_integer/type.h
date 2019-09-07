
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::scaled_integer` type

#if !defined(CNL_IMPL_SCALED_INTEGER_DEFINITION_H)
#define CNL_IMPL_SCALED_INTEGER_DEFINITION_H 1

#include "../number_base.h"
#include "../num_traits/width.h"
#include "../operators/tagged.h"
#include "../power_value.h"
#include "../scaled/convert_operator.h"
#include "../scaled/power.h"
#include "../../constant.h"
#include "../../numeric.h"
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
    /// \note Currently, only binary support and partial decimal support is provided.
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 4 integer bits and 3 fractional bits:
    /// \snippet snippets.cpp define a scaled_integer value

    template<typename Rep, class Scale>
    class scaled_integer
            : public _impl::number_base<scaled_integer<Rep, Scale>, Rep> {
        static_assert(!_impl::is_scaled_integer<Rep>::value,
                "scaled_integer of scaled_integer is not a supported");

        using _base = _impl::number_base<scaled_integer<Rep, Scale>, Rep>;
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        /// alias to template parameter, \a Rep
        using rep = Rep;

        /// alias to template parameter, \a Rep
        using scale = Scale;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

    private:
        // constructor taking representation explicitly using operator++(int)-style trick
        constexpr scaled_integer(rep r, int)
                :_base(r)
        {
        }

    public:
        /// default constructor
        scaled_integer() = default;

        /// constructor taking a scaled_integer type
        template<typename FromRep, class FromScale>
        constexpr scaled_integer(scaled_integer<FromRep, FromScale> const& rhs)
                : _base(convert<decltype(FromScale{}/scale{}), Rep>(
                        _impl::from_value<Rep>(cnl::_impl::to_rep(rhs))))
        {
        }

        /// constructor taking an integer type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer||_impl::is_constant<S>::value, int> Dummy = 0>
        constexpr scaled_integer(S const& s)
                : _base(convert<decltype(typename scale::identity{}/scale{}), Rep>(_impl::from_value<Rep>(s)))
        {
        }

        /// constructor taking a floating-point type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr scaled_integer(S s)
                :_base(convert<decltype(typename scale::identity{}/scale{}), rep>(s))
        {
        }

        /// constructor taking cnl::fraction
        template<typename Numerator, typename Denominator>
        constexpr scaled_integer(fraction<Numerator, Denominator> const& f);

        /// copy assignment operator taking a floating-point type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        CNL_RELAXED_CONSTEXPR scaled_integer& operator=(S s)
        {
            _base::operator=(convert<decltype(typename scale::identity{}/scale{}), rep>(s));
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<typename FromRep, class FromScale>
        CNL_RELAXED_CONSTEXPR scaled_integer& operator=(scaled_integer<FromRep, FromScale> const& rhs)
        {
            _base::operator=(convert<decltype(FromScale{}/scale{}), Rep>(
                            _impl::from_value<Rep>(cnl::_impl::to_rep(rhs))));
            return *this;
        }

        /// copy assignement operator taking cnl::fraction
        template<typename Numerator, typename Denominator>
        CNL_RELAXED_CONSTEXPR scaled_integer& operator=(fraction<Numerator, Denominator> const& f);

        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer||numeric_limits<S>::is_iec559, int> Dummy = 0>
        CNL_NODISCARD explicit constexpr operator S() const
        {
            return convert<scale, S>(_impl::to_rep(*this));
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
