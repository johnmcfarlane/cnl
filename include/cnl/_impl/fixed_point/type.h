
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fixed_point` type

#if !defined(CNL_FIXED_POINT_DEF_H)
#define CNL_FIXED_POINT_DEF_H 1

#include "../power.h"
#include "../num_traits/width.h"
#include "../number_base.h"
#include "../../constant.h"
#include "../../numeric.h"
#include "declaration.h"
#include "is_fixed_point.h"

/// compositional numeric library
namespace cnl {
    template<typename Numerator, typename Denominator>
    struct fraction;

    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile cnl/fixed_point.h
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
    /// \snippet snippets.cpp define a fixed_point value

    template<typename Rep, int Exponent, int Radix>
    class fixed_point
            : public _impl::number_base<fixed_point<Rep, Exponent, Radix>, Rep> {
        static_assert(Radix>=2, "Radix must be two or greater");

        static_assert(!_impl::is_fixed_point<Rep>::value,
                "fixed_point of fixed_point is not a supported");

        using _base = _impl::number_base<fixed_point<Rep, Exponent, Radix>, Rep>;
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        /// alias to template parameter, \a Rep
        using rep = Rep;

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// value of template parameter, \a Radix
        constexpr static int radix = Radix;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

    private:
        // constructor taking representation explicitly using operator++(int)-style trick
        constexpr fixed_point(rep r, int)
                :_base(r)
        {
        }

    public:
        /// default constructor
        fixed_point() = default;

        /// constructor taking a fixed-point type
        template<class FromRep, int FromExponent>
        constexpr fixed_point(fixed_point<FromRep, FromExponent, Radix> const& rhs)
                : _base(
                static_cast<Rep>(_impl::scale<FromExponent-exponent, Radix>(
                        _impl::make_number<Rep>(cnl::_impl::to_rep(rhs)))))
        {
        }

        /// constructor taking a cnl::constant object
        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        constexpr fixed_point(constant<Value> rhs)
                : fixed_point(_impl::from_rep<fixed_point<typename decltype(rhs)::value_type, 0>>(Value))
        {
        }

        /// constructor taking an integer type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr fixed_point(S const& s)
                : _base(static_cast<Rep>(_impl::scale<-exponent, Radix>(_impl::make_number<Rep>(s))))
        {
        }

        /// constructor taking a floating-point type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr fixed_point(S s)
                :_base(floating_point_to_rep(s))
        {
        }

        /// constructor taking cnl::fraction
        template<typename Numerator, typename Denominator>
        constexpr fixed_point(fraction<Numerator, Denominator> const& f);

        /// copy assignment operator taking a floating-point type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        CNL_RELAXED_CONSTEXPR fixed_point& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromRep, int FromExponent>
        CNL_RELAXED_CONSTEXPR fixed_point& operator=(fixed_point<FromRep, FromExponent, Radix> const& rhs)
        {
            _base::operator=(fixed_point_to_rep(rhs));
            return *this;
        }

        /// copy assignement operator taking cnl::fraction
        template<typename Numerator, typename Denominator>
        CNL_RELAXED_CONSTEXPR fixed_point& operator=(fraction<Numerator, Denominator> const& f);

        /// returns value represented as integral
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::scale<exponent>(_impl::to_rep(*this)));
        }

        /// returns value represented as floating-point
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
            return S(_impl::to_rep(*this))*inverse_one<S>();
        }

        /// creates an instance given the underlying representation value
        template<typename, typename, typename>
        friend struct from_rep;

    private:
        template<class S>
        static constexpr _impl::enable_if_t<numeric_limits<S>::is_iec559, S> one();

        template<class S>
        static constexpr _impl::enable_if_t<numeric_limits<S>::is_integer, S> one();

        template<class S>
        static constexpr S inverse_one();

        template<class S>
        static constexpr S rep_to_integral(rep r);

        template<class S>
        static constexpr rep floating_point_to_rep(S s);

        template<class S>
        static constexpr S rep_to_floating_point(rep r);

        template<class FromRep, int FromExponent>
        static constexpr rep fixed_point_to_rep(fixed_point<FromRep, FromExponent, Radix> const& rhs);
    };

    /// value of template parameter, \a Exponent
    template<typename Rep, int Exponent, int Radix>
    constexpr int fixed_point<Rep, Exponent, Radix>::exponent;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fixed_point::fixed_point deduction guides

#if defined(__cpp_deduction_guides)
    // same as cnl::make_fixed_point
    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    fixed_point(::cnl::constant<Value>)
    -> fixed_point<
            set_digits_t<int, _impl::max(digits_v<int>, _impl::used_digits(Value)-trailing_bits(Value))>,
            trailing_bits(Value)>;

    template<class Integer>
    fixed_point(Integer)
    -> fixed_point<Integer, 0>;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fixed_point<> member definitions

    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr auto fixed_point<Rep, Exponent, Radix>::one()
    -> _impl::enable_if_t<numeric_limits<S>::is_iec559, S>
    {
        return power<S, -exponent, Radix>();
    }

    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr auto fixed_point<Rep, Exponent, Radix>::one()
    -> _impl::enable_if_t<numeric_limits<S>::is_integer, S>
    {
        return _impl::from_rep<fixed_point<S, 0>>(1);
    }

    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr S fixed_point<Rep, Exponent, Radix>::inverse_one()
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return power<S, exponent, Radix>();
    }

    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr typename fixed_point<Rep, Exponent, Radix>::rep
    fixed_point<Rep, Exponent, Radix>::floating_point_to_rep(S s)
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return static_cast<rep>(s*one<S>());
    }

    template<typename Rep, int Exponent, int Radix>
    template<class FromRep, int FromExponent>
    constexpr typename fixed_point<Rep, Exponent, Radix>::rep
    fixed_point<Rep, Exponent, Radix>::fixed_point_to_rep(fixed_point<FromRep, FromExponent, Radix> const& rhs)
    {
        return _impl::scale<FromExponent-exponent>(_impl::to_rep(rhs));
    }
}

#endif  // CNL_FIXED_POINT_DEF_H
