
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fixed_point` type

#if !defined(CNL_FIXED_POINT_DEF_H)
#define CNL_FIXED_POINT_DEF_H 1

#include <cnl/bits/number_base.h>
#include <cnl/constant.h>
#include <cnl/numeric.h>

/// compositional numeric library
namespace cnl {
    // forward declaration
    template<class Rep = int, int Exponent = 0>
    class fixed_point;

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_fixed_point

        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };

        template<class Rep, int Exponent>
        struct is_fixed_point<fixed_point<Rep, Exponent>>
                : public std::true_type {
        };

        namespace fp {
            ////////////////////////////////////////////////////////////////////////////////
            // cnl::_impl::float_of_size

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
            // cnl::_impl::float_of_same_size

            template<class T>
            using float_of_same_size = typename float_of_size<digits<T>::value + is_signed<T>::value>::type;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::from_value

    template <class Rep, int Exponent, class Value>
    struct from_value<fixed_point<Rep, Exponent>, Value> {
        using type = fixed_point<Value>;
    };

    template <class Rep, int Exponent, class ValueRep, int ValueExponent>
    struct from_value<fixed_point<Rep, Exponent>, fixed_point<ValueRep, ValueExponent>> {
        using type = fixed_point<from_value_t<Rep, ValueRep>, ValueExponent>;
    };

    template<class Rep, int Exponent, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<fixed_point<Rep, Exponent>, constant<Value>> {
        // same as deduction guide
        using type = fixed_point<
                set_digits_t<int, _impl::max(digits<int>::value, used_bits(Value)-trailing_bits(Value))>,
                trailing_bits(Value)>;
    };

    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile cnl/fixed_point.h
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
        constexpr fixed_point(fixed_point<FromRep, FromExponent> const& rhs)
                : _base(
                static_cast<Rep>(_impl::shift<FromExponent-exponent>(_impl::from_value<Rep>(_impl::to_rep(rhs)))))
        {
        }

        /// constructor taking a cnl::constant object
        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        constexpr fixed_point(constant<Value>)
                : fixed_point(_impl::from_rep<fixed_point<decltype(Value), 0>>(Value))
        {
        }

        /// constructor taking an integer type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr fixed_point(S const& s)
                : _base(static_cast<Rep>(_impl::shift<-exponent>(_impl::from_value<Rep>(s))))
        {
        }

        /// constructor taking a floating-point type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr fixed_point(S s)
                :_base(floating_point_to_rep(s))
        {
        }

        /// copy assignment operator taking a floating-point type
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        CNL_COPY_CONSTEXPR fixed_point& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromRep, int FromExponent>
        CNL_COPY_CONSTEXPR fixed_point& operator=(fixed_point<FromRep, FromExponent> const& rhs)
        {
            _base::operator=(fixed_point_to_rep(rhs));
            return *this;
        }

        /// returns value represented as integral
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::shift<exponent>(_impl::to_rep(*this)));
        }

        /// returns value represented as floating-point
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
            return S(_impl::to_rep(*this))*inverse_one<S>();
        }

        /// creates an instance given the underlying representation value
        template<class, class, class>
        friend struct from_rep;

    private:
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        static constexpr S one();

        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
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
        static constexpr rep fixed_point_to_rep(fixed_point<FromRep, FromExponent> const& rhs);
    };

    /// value of template parameter, \a Exponent
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::exponent;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fixed_point::fixed_point deduction guides

#if defined(__cpp_deduction_guides)
    // same as from_value
    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    fixed_point(::cnl::constant<Value>)
    -> fixed_point<set_digits_t<int, _impl::max(digits<int>::value, used_bits(Value)-trailing_bits(Value))>, trailing_bits(Value)>;

    template<class Integer>
    fixed_point(Integer)
    -> fixed_point<Integer, 0>;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // general-purpose implementation-specific definitions

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // file-local implementation-specific definitions

        namespace fp {
            namespace type {
                ////////////////////////////////////////////////////////////////////////////////
                // cnl::_impl::fp::type::pow2

                // returns given power of 2
                template<class S, int Exponent, enable_if_t<Exponent==0, int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return S{1.};
                }

                template<class S, int Exponent,
                        enable_if_t<!(Exponent<=0) && (Exponent<8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent-1>()*S(2);
                }

                template<class S, int Exponent, enable_if_t<(Exponent>=8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent-8>()*S(256);
                }

                template<class S, int Exponent,
                        enable_if_t<!(Exponent>=0) && (Exponent>-8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent+1>()*S(.5);
                }

                template<class S, int Exponent, enable_if_t<(Exponent<=-8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
                    return pow2<S, Exponent+8>()*S(.003906250);
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fixed_point<> member definitions

    template<class Rep, int Exponent>
    template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return _impl::fp::type::pow2<S, -exponent>();
    }

    template<class Rep, int Exponent>
    template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return _impl::from_rep<fixed_point<S, 0>>(1);
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::inverse_one()
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return _impl::fp::type::pow2<S, exponent>();
    }

    template<class Rep, int Exponent>
    template<class S>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::floating_point_to_rep(S s)
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return static_cast<rep>(s*one<S>());
    }

    template<class Rep, int Exponent>
    template<class FromRep, int FromExponent>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::fixed_point_to_rep(fixed_point<FromRep, FromExponent> const& rhs)
    {
        return _impl::shift<FromExponent-exponent>(_impl::to_rep(rhs));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::from_rep<fixed_point<>> specialization

    template<class Rep, int Exponent, class Integer>
    struct from_rep<fixed_point<Rep, Exponent>, Integer> {
        constexpr auto operator()(Integer const& rep) const
        -> fixed_point<Integer, Exponent> {
            return fixed_point<Integer, Exponent>(rep, 0);
        }
    };
}

#endif  // CNL_FIXED_POINT_DEF_H
