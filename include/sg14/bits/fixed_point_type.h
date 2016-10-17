
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_DEF_H)
#define SG14_FIXED_POINT_DEF_H 1

#include <sg14/cstdint>
#include <sg14/limits>

/// study group 14 of the C++ working group
namespace sg14 {
    // forward declaration
    template<class Rep = int, int Exponent = 0>
    class fixed_point;

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {
            template<class From, class To>
            struct is_implicitly_convertible;

            template<class FromRep, int FromExponent, class ToRep, int ToExponent>
            struct is_implicitly_convertible<fixed_point<FromRep, FromExponent>, fixed_point<ToRep, ToExponent>> {
                using from_limits = std::numeric_limits<FromRep>;
                static constexpr int from_integer_digits = from_limits::digits+FromExponent;

                using to_limits = std::numeric_limits<ToRep>;
                static constexpr int to_integer_digits = to_limits::digits+ToExponent;

                static constexpr bool value =
                        to_limits::is_signed>=from_limits::is_signed && to_integer_digits>=from_integer_digits
                                && ToExponent<=FromExponent;
            };

            ////////////////////////////////////////////////////////////////////////////////
            // sg14::_impl::float_of_same_size

            template<class T>
            using float_of_same_size = set_width_t<float, width<T>::value>;
        }
    }

    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile sg14/fixed_point.h
    ///
    /// \tparam Rep the underlying type used to represent the value
    /// \tparam Exponent the value by which to scale the integer value in order to get the real value
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 3 integer bits and 4 fractional bits:
    /// \snippet snippets.cpp define a fixed_point value

    template<class Rep, int Exponent>
    class fixed_point {
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
                :_r(r)
        {
        }

    public:
        /// default constructor
        fixed_point() { }

        /// constructor taking a fixed-point type explicitly
        template<class FromRep, int FromExponent, typename std::enable_if<!_impl::fp::is_implicitly_convertible<fixed_point<FromRep, FromExponent>, fixed_point>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(const fixed_point<FromRep, FromExponent>& rhs)
                :_r(fixed_point_to_rep(rhs))
        {
        }

        /// constructor taking a fixed-point type implicitly
        template<class FromRep, int FromExponent, typename std::enable_if<_impl::fp::is_implicitly_convertible<fixed_point<FromRep, FromExponent>, fixed_point>::value, int>::type Dummy = 0>
        constexpr fixed_point(const fixed_point<FromRep, FromExponent>& rhs)
                : _r(fixed_point_to_rep(rhs))
        {
        }

        /// constructor taking an integer type explicitly
        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer
                && !_impl::fp::is_implicitly_convertible<fixed_point<S>, fixed_point>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                : fixed_point(fixed_point<S, 0>::from_data(s))
        {
        }

        /// constructor taking an integer type implicitly
        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer
                && _impl::fp::is_implicitly_convertible<fixed_point<S>, fixed_point>::value, int>::type Dummy = 0>
        constexpr fixed_point(S s)
                : fixed_point(fixed_point<S, 0>::from_data(s))
        {
        }

        /// constructor taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_r(floating_point_to_rep(s))
        {
        }

        /// copy assignment operator taking an integer type
        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer
                && _impl::fp::is_implicitly_convertible<fixed_point<S>, fixed_point>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            return operator=(fixed_point<S, 0>::from_data(s));
        }

        /// copy assignment operator taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _r = floating_point_to_rep(s);
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromRep, int FromExponent, typename std::enable_if<_impl::fp::is_implicitly_convertible<fixed_point<FromRep, FromExponent>, fixed_point>::value, int>::type Dummy = 0>
        fixed_point& operator=(const fixed_point<FromRep, FromExponent>& rhs)
        {
            _r = fixed_point_to_rep(rhs);
            return *this;
        }

        /// returns value represented as integral explicitly
        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer, int>::type Dummy = 0>
        explicit constexpr operator S() const
        {
            return rep_to_integral<S>(_r);
        }

        /// returns value represented as integral implicitly
        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer
                && _impl::fp::is_implicitly_convertible<fixed_point, fixed_point<S>>::value, int>::type Dummy = 0>
        constexpr operator S() const
        {
            return rep_to_integral<S>(_r);
        }

        /// returns value represented as floating-point
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr operator S() const
        {
            return rep_to_floating_point<S>(_r);
        }

        /// returns non-zeroness represented as boolean
        explicit constexpr operator bool() const
        {
            return _r!=0;
        }

        template<class Rhs>
        fixed_point& operator*=(const Rhs& rhs);

        template<class Rhs>
        fixed_point& operator/=(const Rhs& rhs);

        /// returns internal representation of value
        constexpr rep data() const
        {
            return _r;
        }

        /// creates an instance given the underlying representation value
        static constexpr fixed_point from_data(rep r)
        {
            return fixed_point(r, 0);
        }

    private:
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        static constexpr S one();

        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer, int>::type Dummy = 0>
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

        ////////////////////////////////////////////////////////////////////////////////
        // variables

        rep _r;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // general-purpose implementation-specific definitions

    namespace _impl {

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_fixed_point

        template<class T>
        struct is_fixed_point;

        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };

        template<class Rep, int Exponent>
        struct is_fixed_point<fixed_point<Rep, Exponent>>
                : public std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::shift_left and sg14::_impl::shift_right

        // performs a shift operation by a fixed number of bits avoiding two pitfalls:
        // 1) shifting by a negative amount causes undefined behavior
        // 2) converting between integer types of different sizes can lose significant bits during shift right

        // Exponent == 0
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        (Exponent==0),
                        int>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            // cast only
            return static_cast<Output>(i);
        }

        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        Exponent==0,
                        int>::type Dummy = 0>
        constexpr Output shift_right(Input i)
        {
            // cast only
            return static_cast<Output>(i);
        }

        // Exponent >= 0
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0),
                        int>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            using larger = typename std::conditional<
                    width<Input>::value<=width<Output>::value,
                    Output, Input>::type;
            return static_cast<Output>(static_cast<larger>(i)*(larger{1} << Exponent));
        }

        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0),
                        int>::type Dummy = 0>
        constexpr Output shift_right(Input i)
        {
            using larger = typename std::conditional<
                    width<Input>::value<=width<Output>::value,
                    Output, Input>::type;
            return static_cast<Output>(static_cast<larger>(i)/(larger{1} << Exponent));
        }

        // Exponent < 0
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        (Exponent<0),
                        int>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            // negate Exponent and flip from left to right
            return shift_right<-Exponent, Output, Input>(i);
        }

        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        Exponent<0,
                        int>::type Dummy = 0>
        constexpr Output shift_right(Input i)
        {
            // negate Exponent and flip from right to left
            return shift_left<-Exponent, Output, Input>(i);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // file-local implementation-specific definitions

    namespace _impl {
        namespace fp {
            namespace type {
                ////////////////////////////////////////////////////////////////////////////////
                // sg14::_impl::fp::type::pow2

                // returns given power of 2
                template<class S, int Exponent, typename std::enable_if<Exponent==0, int>::type Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return 1;
                }

                template<class S, int Exponent, typename std::enable_if<
                        !(Exponent<=0) && (Exponent<8), int>::type Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent-1>()*S(2);
                }

                template<class S, int Exponent, typename std::enable_if<(Exponent>=8), int>::type Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent-8>()*S(256);
                }

                template<class S, int Exponent, typename std::enable_if<
                        !(Exponent>=0) && (Exponent>-8), int>::type Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent+1>()*S(.5);
                }

                template<class S, int Exponent, typename std::enable_if<(Exponent<=-8), int>::type Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent+8>()*S(.003906250);
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point<> member definitions

    template<class Rep, int Exponent>
    template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return _impl::fp::type::pow2<S, -exponent>();
    }

    template<class Rep, int Exponent>
    template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer, int>::type Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return fixed_point<S, 0>::from_data(1);
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::inverse_one()
    {
        static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
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
        static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
        return static_cast<rep>(s*one<S>());
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::rep_to_floating_point(rep r)
    {
        static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
        return S(r)*inverse_one<S>();
    }

    template<class Rep, int Exponent>
    template<class FromRep, int FromExponent>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::fixed_point_to_rep(const fixed_point<FromRep, FromExponent>& rhs)
    {
        return _impl::shift_right<(exponent-FromExponent), rep>(rhs.data());
    }
}

#endif	// SG14_FIXED_POINT_DEF_H
