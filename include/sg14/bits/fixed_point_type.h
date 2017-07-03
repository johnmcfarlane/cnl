
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_DEF_H)
#define SG14_FIXED_POINT_DEF_H 1

#include <sg14/auxiliary/const_integer.h>
#include <sg14/bits/number_base.h>

/// study group 14 of the C++ working group
namespace sg14 {
    // forward declaration
    template<class Rep = int, int Exponent = 0>
    class fixed_point;

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {
            ////////////////////////////////////////////////////////////////////////////////
            // sg14::_impl::float_of_size

            template<int NumBits, class Enable = void>
            struct float_of_size;

            template<int NumBits>
            struct float_of_size<NumBits, enable_if_t<NumBits <= sizeof(float)*CHAR_BIT>> {
                using type = float;
            };

            template<int NumBits>
            struct float_of_size<NumBits, enable_if_t<sizeof(float)*CHAR_BIT < NumBits && NumBits <= sizeof(double)*CHAR_BIT>> {
                using type = double;
            };

            template<int NumBits>
            struct float_of_size<NumBits, enable_if_t<sizeof(double)*CHAR_BIT < NumBits && NumBits <= sizeof(long double)*CHAR_BIT>> {
                using type = long double;
            };

            ////////////////////////////////////////////////////////////////////////////////
            // sg14::_impl::float_of_same_size

            template<class T>
            using float_of_same_size = typename float_of_size<digits<T>::value + is_signed<T>::value>::type;
        }
    }

    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile sg14/fixed_point
    ///
    /// \tparam Rep the underlying type used to represent the value
    /// \tparam Exponent the value by which to scale the integer value in order to get the real value
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 3 integer bits and 4 fractional bits:
    /// \snippet snippets.cpp define a fixed_point value

    template<class Rep, int Exponent>
    class fixed_point
            : public _impl::number_base<fixed_point<Rep, Exponent>, Rep> {
        using _base = _impl::number_base<fixed_point<Rep, Exponent>, Rep>;
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        /// alias to template parameter, \a Rep
        using rep = Rep;

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// number of binary digits this type can represent;
        /// equivalent to [std::numeric_limits::digits](http://en.cppreference.com/w/cpp/types/numeric_limits/digits)
        constexpr static int digits = std::numeric_limits<Rep>::digits;

        /// number of binary digits devoted to integer part of value;
        /// can be negative for specializations with especially small ranges
        constexpr static int integer_digits = digits+exponent;

        /// number of binary digits devoted to fractional part of value;
        /// can be negative for specializations with especially large ranges
        constexpr static int fractional_digits = -exponent;

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
        constexpr fixed_point() : _base() { }

        /// constructor taking a fixed-point type
        template<class FromRep, int FromExponent>
        constexpr fixed_point(const fixed_point<FromRep, FromExponent>& rhs)
                : _base(fixed_point_to_rep(rhs))
        {
        }

        /// constructor taking an integral_constant type
        template<class Integral, Integral Constant>
        constexpr fixed_point(const std::integral_constant<Integral, Constant>&)
                : fixed_point(fixed_point<Integral, 0>::from_data(Constant))
        {
        }

        /// constructor taking an integer type
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr fixed_point(const S& s)
            : fixed_point(fixed_point<S, 0>::from_data(s))
        {
        }

        /// constructor taking an integral_constant type
        template<class Integral, Integral Value, int Digits>
        constexpr fixed_point(const_integer<Integral, Value, Digits, Exponent> ci)
            : _base(ci << Exponent)
        {
        }

        /// constructor taking a floating-point type
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr fixed_point(S s)
                :_base(floating_point_to_rep(s))
        {
        }

        /// copy assignment operator taking an integer type
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        fixed_point& operator=(S s)
        {
            return operator=(fixed_point<S, 0>::from_data(s));
        }

        /// copy assignment operator taking a floating-point type
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_iec559, int> Dummy = 0>
        fixed_point& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromRep, int FromExponent>
        fixed_point& operator=(const fixed_point<FromRep, FromExponent>& rhs)
        {
            _base::operator=(fixed_point_to_rep(rhs));
            return *this;
        }

        /// returns value represented as bool
        template<typename R = Rep>
        constexpr operator typename std::enable_if<std::is_convertible<Rep, bool>::value, bool>() const
        {
            return static_cast<bool>(_base::data());
        }

        /// returns value represented as integral
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr operator S() const
        {
            return rep_to_integral<S>(_base::data());
        }

        /// returns value represented as floating-point
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_iec559, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            return rep_to_floating_point<S>(_base::data());
        }

        /// creates an instance given the underlying representation value
        static constexpr fixed_point from_data(rep const & r)
        {
            return fixed_point(r, 0);
        }

    private:
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_iec559, int> Dummy = 0>
        static constexpr S one();

        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        static constexpr S one();

        template<class S>
        static constexpr S inverse_one();

        template<class S>
        static constexpr S rep_to_integral(rep r);

        template<class S>
        static constexpr rep floating_point_to_rep(S s);

        template<class S>
        static constexpr S rep_to_floating_point(rep r);

        template<class FromRep, int FromExponent>
        static constexpr rep fixed_point_to_rep(const fixed_point<FromRep, FromExponent>& rhs);
    };

    /// value of template parameter, \a Exponent
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::exponent;

    /// number of binary digits this type can represent;
    /// equivalent to [std::numeric_limits::digits](http://en.cppreference.com/w/cpp/types/numeric_limits/digits)
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::digits;

    /// number of binary digits devoted to integer part of value;
    /// can be negative for specializations with especially small ranges
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::integer_digits;

    /// number of binary digits devoted to fractional part of value;
    /// can be negative for specializations with especially large ranges
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // general-purpose implementation-specific definitions

    namespace _impl {

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_fixed_point

        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };

        template<class Rep, int Exponent>
        struct is_fixed_point<fixed_point<Rep, Exponent>>
                : public std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::shift_left

        // performs a shift operation by a fixed number of bits avoiding two pitfalls:
        // 1) shifting by a negative amount causes undefined behavior
        // 2) converting between integer types of different sizes can lose significant bits during shift right

        // Exponent == 0
        template<int exp, class Output, class Input>
        constexpr Output shift_left(Input i)
        {
            using larger = typename std::conditional<
                    digits<Input>::value<=digits<Output>::value,
                    Output, Input>::type;

            return (exp>-std::numeric_limits<larger>::digits)
                   ? static_cast<Output>(_impl::scale<larger>(static_cast<larger>(i), 2, exp))
                   : Output{0};
        }

        ////////////////////////////////////////////////////////////////////////////////
        // file-local implementation-specific definitions

        namespace fp {
            namespace type {
                ////////////////////////////////////////////////////////////////////////////////
                // sg14::_impl::fp::type::pow2

                // returns given power of 2
                template<class S, int Exponent, enable_if_t<Exponent==0, int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return S{1.};
                }

                template<class S, int Exponent, 
                        enable_if_t<!(Exponent<=0) && (Exponent<8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent-1>()*S(2);
                }

                template<class S, int Exponent, enable_if_t<(Exponent>=8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent-8>()*S(256);
                }

                template<class S, int Exponent, 
                        enable_if_t<!(Exponent>=0) && (Exponent>-8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent+1>()*S(.5);
                }

                template<class S, int Exponent, enable_if_t<(Exponent<=-8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent+8>()*S(.003906250);
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point<> member definitions

    template<class Rep, int Exponent>
    template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_iec559, int> Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return _impl::fp::type::pow2<S, -exponent>();
    }

    template<class Rep, int Exponent>
    template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return fixed_point<S, 0>::from_data(1);
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::inverse_one()
    {
        static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
        return _impl::fp::type::pow2<S, exponent>();
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::rep_to_integral(rep r)
    {
        static_assert(std::numeric_limits<S>::is_integer, "S must be integral type");

        return _impl::shift_left<exponent, S>(r);
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::floating_point_to_rep(S s)
    {
        static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
        return static_cast<rep>(s*one<S>());
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::rep_to_floating_point(rep r)
    {
        static_assert(std::numeric_limits<S>::is_iec559, "S must be floating-point type");
        return S(r)*inverse_one<S>();
    }

    template<class Rep, int Exponent>
    template<class FromRep, int FromExponent>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::fixed_point_to_rep(const fixed_point<FromRep, FromExponent>& rhs)
    {
        return _impl::shift_left<FromExponent-exponent, rep>(rhs.data());
    }
}

#endif	// SG14_FIXED_POINT_DEF_H
