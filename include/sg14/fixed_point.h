
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::fixed_point` type

#if !defined(_SG14_FIXED_POINT)
#define _SG14_FIXED_POINT 1

#include "type_traits.h"

#include "bits/common.h"

////////////////////////////////////////////////////////////////////////////////
// _SG14_FIXED_POINT_EXCEPTIONS_ENABLED macro definition 

#if defined(_SG14_FIXED_POINT_EXCEPTIONS_ENABLED)
#error _SG14_FIXED_POINT_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define _SG14_FIXED_POINT_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define _SG14_FIXED_POINT_EXCEPTIONS_ENABLED
#endif
#else
#define _SG14_FIXED_POINT_EXCEPTIONS_ENABLED
#endif

#if defined(_SG14_FIXED_POINT_EXCEPTIONS_ENABLED)

#include <stdexcept>

#endif

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // general-purpose _fixed_point_impl definitions

    namespace _fixed_point_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::float_of_same_size

        template<class T>
        using float_of_same_size = set_width_t<float, width<T>::value>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::next_size

        // given an integral type, IntType,
        // provides the integral type of the equivalent type with twice the width
        template<class IntType>
        using next_size = typename sg14::set_width_t<IntType, width<IntType>::value*2>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::previous_size

        // given an integral type, IntType,
        // provides the integral type of the equivalent type with half the width
        template<class IntType>
        using previous_size = typename sg14::set_width_t<IntType, width<IntType>::value/2>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::shift_left and sg14::_fixed_point_impl::shift_right

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
            static_assert(is_integral<Input>::value, "Input must be integral type");
            static_assert(is_integral<Output>::value, "Output must be integral type");

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
            static_assert(is_integral<Input>::value, "Input must be integral type");
            static_assert(is_integral<Output>::value, "Output must be integral type");

            // cast only
            return static_cast<Output>(i);
        }

        // sizeof(Input) >= sizeof(Output)
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0) && sizeof(Output)<=sizeof(Input) && is_unsigned<Input>::value,
                        int>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            return shift_left<0, Output, Input>(i) << Exponent;
        }

        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0) && sizeof(Output)<=sizeof(Input),
                        int>::type Dummy = 0>
        constexpr Output shift_right(Input i)
        {
            return shift_right<0, Output, Input>(i >> Exponent);
        }

        // sizeof(Input) < sizeof(Output)
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0) && !(sizeof(Output)<=sizeof(Input)) && is_unsigned<Input>::value,
                        char>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            return shift_left<0, Output, Input>(i) << Exponent;
        }

        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0) && !(sizeof(Output)<=sizeof(Input)),
                        char>::type Dummy = 0>
        constexpr Output shift_right(Input i)
        {
            return shift_right<0, Output, Input>(i) >> Exponent;
        }

        // is_signed<Input>
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0) && is_signed<Input>::value,
                        int>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            using unsigned_input = typename make_unsigned<Input>::type;
            using signed_output = typename make_signed<Output>::type;

            return static_cast<Output>((i>=0)
                                       ? shift_left<Exponent, signed_output>(static_cast<unsigned_input>(i))
                                       : -shift_left<Exponent, signed_output>(static_cast<unsigned_input>(-i)));
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

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::pow2

        // returns given power of 2
        template<class S, int Exponent, typename std::enable_if<Exponent==0, int>::type Dummy = 0>
        constexpr S pow2()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return 1;
        }

        template<class S, int Exponent, typename std::enable_if<!(Exponent<=0), int>::type Dummy = 0>
        constexpr S pow2()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return pow2<S, Exponent-1>()*S(2);
        }

        template<class S, int Exponent, typename std::enable_if<!(Exponent>=0), int>::type Dummy = 0>
        constexpr S pow2()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return pow2<S, Exponent+1>()*S(.5);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::capacity

        // has value that, given a value N,
        // returns number of bits necessary to represent it in binary
        template<unsigned N>
        struct capacity;

        template<>
        struct capacity<0> {
            static constexpr int value = 0;
        };

        template<unsigned N>
        struct capacity {
            static constexpr int value = capacity<N/2>::value+1;
        };
    }

    /// \brief literal real number approximation that uses fixed-point arithmetic
    ///
    /// \tparam Rep the underlying type used to represent the value
    /// \tparam Exponent the value by which to scale the integer value in order to get the real value
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 3 integer bits and 4 fractional bits:
    /// \snippet snippets.cpp define a fixed_point value

    template<class Rep = int, int Exponent = 0>
    class fixed_point {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        /// alias to template parameter, \a Rep
        using repr_type = Rep;

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// number of binary digits this type can represent;
        /// equivalent to [std::numeric_limits::digits](http://en.cppreference.com/w/cpp/types/numeric_limits/digits)
        constexpr static int digits = width<Rep>::value-is_signed<Rep>::value;

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
        constexpr fixed_point(repr_type repr, int)
                :_r(repr)
        {
        }

    public:
        /// default constructor
        fixed_point() { }

        /// constructor taking an integer type
        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_r(integral_to_repr(s))
        {
        }

        /// constructor taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_r(floating_point_to_repr(s))
        {
        }

        /// constructor taking a fixed-point type
        template<class FromRep, int FromExponent>
        explicit constexpr fixed_point(const fixed_point<FromRep, FromExponent>& rhs)
                :_r(fixed_point_to_repr(rhs))
        {
        }

        /// copy assignment operator taking an integer type
        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _r = integral_to_repr(s);
            return *this;
        }

        /// copy assignment operator taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _r = floating_point_to_repr(s);
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromRep, int FromExponent>
        fixed_point& operator=(const fixed_point<FromRep, FromExponent>& rhs)
        {
            _r = fixed_point_to_repr(rhs);
            return *this;
        }

        /// returns value represented as integral
        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        explicit constexpr operator S() const
        {
            return repr_to_integral<S>(_r);
        }

        /// returns value represented as floating-point
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr operator S() const
        {
            return repr_to_floating_point<S>(_r);
        }

        /// returns non-zeroness represented as boolean
        explicit constexpr operator bool() const
        {
            return _r!=0;
        }

        template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy = 0>
        fixed_point& operator*=(const Rhs& rhs);

        template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy = 0>
        fixed_point& operator/=(const Rhs& rhs);

        /// returns internal representation of value
        constexpr repr_type data() const
        {
            return _r;
        }

        /// creates an instance given the underlying representation value
        static constexpr fixed_point from_data(repr_type repr)
        {
            return fixed_point(repr, 0);
        }

    private:
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        static constexpr S one()
        {
            return _fixed_point_impl::pow2<S, -exponent>();
        }

        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        static constexpr S one()
        {
            return integral_to_repr<S>(1);
        }

        template<class S>
        static constexpr S inverse_one()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return _fixed_point_impl::pow2<S, exponent>();
        }

        template<class S>
        static constexpr repr_type integral_to_repr(S s)
        {
            static_assert(is_integral<S>::value, "S must be unsigned integral type");

            return _fixed_point_impl::shift_right<exponent, repr_type>(s);
        }

        template<class S>
        static constexpr S repr_to_integral(repr_type r)
        {
            static_assert(is_integral<S>::value, "S must be unsigned integral type");

            return _fixed_point_impl::shift_left<exponent, S>(r);
        }

        template<class S>
        static constexpr repr_type floating_point_to_repr(S s)
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return static_cast<repr_type>(s*one<S>());
        }

        template<class S>
        static constexpr S repr_to_floating_point(repr_type r)
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return S(r)*inverse_one<S>();
        }

        template<class FromRep, int FromExponent>
        static constexpr repr_type fixed_point_to_repr(const fixed_point<FromRep, FromExponent>& rhs)
        {
            return _fixed_point_impl::shift_right<(exponent-FromExponent), repr_type>(rhs.data());
        }

        ////////////////////////////////////////////////////////////////////////////////
        // variables

        repr_type _r;
    };

    /// \brief Produce a fixed-point type with the given number of integer and fractional digits.
    ///
    /// \tparam IntegerDigits specifies minimum value of @ref fixed_point::integer_digits
    /// \tparam FractionalDigits specifies the exact value of @ref fixed_point::fractional_digits
    /// \tparam Archetype hints at the type of @ref fixed_point::repr_type
    ///
    /// \remarks The signage of \a Archetype specifies signage of the resultant fixed-point type.
    /// \remarks Typical choices for \a Archetype, `signed` and `unsigned`,
    /// result in a type that uses built-in integers for \a fixed_point::repr_type.
    /// \remarks Resultant type is signed by default.
    ///
    /// \par Example:
    ///
    /// To generate a fixed-point type with a sign bit, 8 fractional bits and at least 7 integer bits:
    /// \snippet snippets.cpp use make_fixed
    ///
    /// \sa make_ufixed
    template<int IntegerDigits, int FractionalDigits = 0, class Archetype = signed>
    using make_fixed = fixed_point<
            set_width_t<Archetype, IntegerDigits+FractionalDigits+is_signed<Archetype>::value>,
            -FractionalDigits>;

    /// \brief Produce an unsigned fixed-point type with the given number of integer and fractional digits.
    ///
    /// \sa make_fixed
    template<int IntegerDigits, int FractionalDigits = 0, class Archetype = unsigned>
    using make_ufixed = make_fixed<
            IntegerDigits,
            FractionalDigits,
            typename make_unsigned<Archetype>::type>;

    /// produces equivalent fixed-point type at a new width
    ///
    /// \tparam Rep the \a Rep parameter of @ref fixed_point
    /// \tparam Exponent the \a Exponent parameter of @ref fixed_point
    /// \tparam MinNumBits the desired size of the resultant type such that `(width<type>::value >= MinNumBytes)`
    ///
    /// \sa set_width_t
    template<class Rep, int Exponent, _width_type MinNumBits>
    struct set_width<fixed_point<Rep, Exponent>, MinNumBits> {
        /// resultant type; a fixed_point specialization that is at least \a MinNumBits bytes in width
        using type = fixed_point<set_width_t<Rep, MinNumBits>, Exponent>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point-aware _fixed_point_impl definitions

    namespace _fixed_point_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::is_fixed_point

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
        // sg14::_fixed_point_impl::widen_integer_result / widen_integer

        // given template parameters of a fixed_point specialization,
        // yields alternative specialization with twice the capacity
        // and the same number of factional bits; requires no bit shift
        template<class FixedPoint>
        struct widen_integer_result {
            using type = fixed_point<
                    _fixed_point_impl::next_size<typename FixedPoint::repr_type>,
                    FixedPoint::exponent>;
        };

        template<class FixedPoint>
        using widen_integer_result_t = typename widen_integer_result<FixedPoint>::type;

        // as widen_integer_result but widens parameter
        template<class FixedPoint>
        widen_integer_result_t<FixedPoint>
        constexpr widen_integer(const FixedPoint& from)
        {
            return widen_integer_result_t<FixedPoint>(from);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::widen_fractional_result / widen_fractional

        // given template parameters of a fixed_point specialization,
        // yields alternative specialization with twice the capacity
        // and the same number of integer bits
        template<class FixedPoint>
        struct widen_fractional_result {
            using prev_repr_type = typename FixedPoint::repr_type;
            using next_repr_type = _fixed_point_impl::next_size<prev_repr_type>;

            using type = fixed_point<
                    next_repr_type,
                    FixedPoint::exponent+static_cast<int>(width<prev_repr_type>::value)
                            -static_cast<int>(width<next_repr_type>::value)>;
        };

        template<class FixedPoint>
        using widen_fractional_result_t = typename widen_fractional_result<FixedPoint>::type;

        // as widen_fractional_result but widens parameter
        template<class FixedPoint>
        widen_fractional_result_t<FixedPoint>
        constexpr widen_fractional(const FixedPoint& from)
        {
            return widen_fractional_result_t<FixedPoint>(from);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // default arithmtic policy

        struct default_arithmetic_policy {
            template<class Lhs, class Rhs>
            using exponent = std::integral_constant<
                    int,
                    (Lhs::integer_digits>Rhs::integer_digits)
                    ? Lhs::exponent
                    : (Rhs::integer_digits>Lhs::integer_digits)
                      ? Rhs::exponent
                      : _impl::max<int>(Lhs::exponent, Rhs::exponent)>;

            template<class Lhs, class Rhs>
            using common_type = fixed_point<
                    typename sg14::common_type<typename Lhs::repr_type, typename Rhs::repr_type>::type,
                    exponent<Lhs, Rhs>::value>;

            template<class Lhs, class Rhs>
            struct operator_base {
                static_assert(is_fixed_point<Lhs>::value, "only fixed-point types supported");
                static_assert(is_fixed_point<Rhs>::value, "only fixed-point types supported");

                using lhs_type = Lhs;
                using rhs_type = Rhs;
            };

            template<class Rhs>
            struct negate {
                using result_type = fixed_point<
                        decltype(-std::declval<typename Rhs::repr_type>()),
                        Rhs::exponent>;

                using rhs_type = Rhs;
            };

            template<class Lhs, class Rhs>
            struct add : operator_base<Lhs, Rhs> {
                using result_type = fixed_point<
                        decltype(std::declval<typename Lhs::repr_type>()+std::declval<typename Rhs::repr_type>()),
                        exponent<Lhs, Rhs>::value>;
            };

            template<class Lhs, class Rhs>
            struct subtract : operator_base<Lhs, Rhs> {
                using result_type = fixed_point<
                        decltype(std::declval<typename Lhs::repr_type>()-std::declval<typename Rhs::repr_type>()),
                        exponent<Lhs, Rhs>::value>;
            };

            template<class Lhs, class Rhs>
            struct multiply : operator_base<Lhs, Rhs> {
                using result_type = fixed_point<
                        decltype(std::declval<typename Lhs::repr_type>()*std::declval<typename Rhs::repr_type>()),
                        exponent<Lhs, Rhs>::value>;
                using lhs_type = widen_integer_result_t<Lhs>;
            };

            template<class Lhs, class Rhs>
            struct divide : operator_base<Lhs, Rhs> {
                using result_type = fixed_point<
                        decltype(std::declval<typename Lhs::repr_type>()/std::declval<typename Rhs::repr_type>()),
                        exponent<Lhs, Rhs>::value>;
                using lhs_type = widen_fractional_result_t<Lhs>;
            };
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::_common_type_mixed

        template<class Lhs, class Rhs, class _Enable = void>
        struct _common_type_mixed;

        // given a fixed-point and a integer type,
        // generates a fixed-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, int LhsExponent, class RhsInteger>
        struct _common_type_mixed<
                fixed_point<LhsRep, LhsExponent>,
                RhsInteger,
                typename std::enable_if<is_integral<RhsInteger>::value>::type> {
            using type = fixed_point<typename sg14::common_type<LhsRep, RhsInteger>::type, LhsExponent>;
        };

        // given a fixed-point and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, int LhsExponent, class Float>
        struct _common_type_mixed<
                fixed_point<LhsRep, LhsExponent>,
                Float,
                typename std::enable_if<std::is_floating_point<Float>::value>::type>
                : sg14::common_type<float_of_same_size<LhsRep>, Float> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::common_type - like common_type for fixed-point types

        template<class ... T>
        struct common_type;

        template<class Rep, int Exponent>
        struct common_type<fixed_point<Rep, Exponent>> {
            using type = fixed_point<
                    typename sg14::common_type<Rep>::type,
                    Exponent>;
        };

        template<class LhsRep, int LhsExponent, class Rhs>
        struct common_type<fixed_point<LhsRep, LhsExponent>, Rhs> {
            static_assert(!_fixed_point_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");
            using type = typename _common_type_mixed<fixed_point<LhsRep, LhsExponent>, Rhs>::type;
        };

        template<class Lhs, class RhsRep, int RhsExponent>
        struct common_type<Lhs, fixed_point<RhsRep, RhsExponent>> {
            static_assert(!_fixed_point_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");
            using type = typename _common_type_mixed<fixed_point<RhsRep, RhsExponent>, Lhs>::type;
        };

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct common_type<
                fixed_point<LhsRep, LhsExponent>,
                fixed_point<RhsRep, RhsExponent>> {
            using type =
            typename default_arithmetic_policy::common_type<
                    fixed_point<LhsRep, LhsExponent>,
                    fixed_point<RhsRep, RhsExponent>>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::common_type_t

        // similar to sg14::common_type
        // but one or both input types must be fixed_point
        template<class ... T>
        using common_type_t = typename common_type<typename std::decay<T>::type ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // arithmetic result types

        template<typename LhsFixedPoint, typename RhsFixedPoint>
        using subtract_result_repr = typename make_signed<typename sg14::common_type<LhsFixedPoint, RhsFixedPoint>::type>::type;

        template<typename Rep>
        using square_result_repr = typename make_unsigned<Rep>::type;

        template<typename FixedPoint>
        using sqrt_result_repr = typename make_unsigned<FixedPoint>::type;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // named fixed-point arithmetic - used by all other fixed-point arithmetic fns

    // sg14::_fixed_point_impl::negate
    template<class Result, class Rhs>
    constexpr Result negate(const Rhs& rhs)
    {
        static_assert(is_signed<typename Result::repr_type>::value, "unary negation of unsigned value");

        return Result::from_data(-static_cast<Result>(rhs).data());
    }

    // sg14::add
    template<class Result, class Lhs, class Rhs>
    constexpr Result add(const Lhs& lhs, const Rhs& rhs)
    {
        return Result::from_data(
                static_cast<typename Result::repr_type>(
                        static_cast<Result>(lhs).data()
                                +static_cast<Result>(rhs).data()));
    }

    // sg14::subtract
    template<class Result, class Lhs, class Rhs>
    constexpr Result subtract(const Lhs& lhs, const Rhs& rhs)
    {
        return Result::from_data(
                static_cast<Result>(lhs).data()
                        -static_cast<Result>(rhs).data());
    }

    // sg14::multiply
    template<class Result, class Lhs, class Rhs>
    constexpr Result multiply(const Lhs& lhs, const Rhs& rhs)
    {
        using result_repr_type = typename Result::repr_type;
        return Result::from_data(
                _fixed_point_impl::shift_left<
                        (Lhs::exponent+Rhs::exponent-Result::exponent),
                        result_repr_type>(lhs.data()*rhs.data()));
    }

    // sg14::divide
    template<class Result, class Lhs, class Rhs>
    constexpr Result divide(const Lhs& lhs, const Rhs& rhs)
    {
        using result_repr_type = typename Result::repr_type;
        return Result::from_data(
                _fixed_point_impl::shift_left<
                        (Lhs::exponent-Rhs::exponent-Result::exponent),
                        result_repr_type>(lhs.data()/rhs.data()));
    }

    namespace _fixed_point_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // policy-based fixed-point arithmetic - customizable arithmetic

        // sg14::_fixed_point_impl::policy_negate
        template<class Policy, class Rhs>
        constexpr auto policy_negate(const Rhs& rhs)
        -> typename Policy::template negate<Rhs>::result_type
        {
            using operator_policy = typename Policy::template negate<Rhs>;
            return negate<typename operator_policy::result_type>(
                    static_cast<typename operator_policy::rhs_type>(rhs));
        }

        // sg14::_fixed_point_impl::policy_add
        template<class Policy, class Lhs, class Rhs>
        constexpr auto policy_add(const Lhs& lhs, const Rhs& rhs)
        -> typename Policy::template add<Lhs, Rhs>::result_type
        {
            using operator_policy = typename Policy::template add<Lhs, Rhs>;
            return add<typename operator_policy::result_type>(
                    static_cast<typename operator_policy::lhs_type>(lhs),
                    static_cast<typename operator_policy::rhs_type>(rhs));
        }

        // sg14::_fixed_point_impl::policy_subtract
        template<class Policy, class Lhs, class Rhs>
        constexpr auto policy_subtract(const Lhs& lhs, const Rhs& rhs)
        -> typename Policy::template subtract<Lhs, Rhs>::result_type
        {
            using operator_policy = typename Policy::template subtract<Lhs, Rhs>;
            return subtract<typename operator_policy::result_type>(
                    static_cast<typename operator_policy::lhs_type>(lhs),
                    static_cast<typename operator_policy::rhs_type>(rhs));
        }

        // sg14::_fixed_point_impl::policy_multiply
        template<class Policy, class Lhs, class Rhs>
        constexpr auto policy_multiply(const Lhs& lhs, const Rhs& rhs)
        -> typename Policy::template multiply<Lhs, Rhs>::result_type
        {
            using operator_policy = typename Policy::template multiply<Lhs, Rhs>;
            return multiply<typename operator_policy::result_type>(
                    static_cast<typename operator_policy::lhs_type>(lhs),
                    static_cast<typename operator_policy::rhs_type>(rhs));
        }

        // sg14::_fixed_point_impl::policy_divide
        template<class Policy, class Lhs, class Rhs>
        constexpr auto policy_divide(const Lhs& lhs, const Rhs& rhs)
        -> typename Policy::template divide<Lhs, Rhs>::result_type
        {
            using operator_policy = typename Policy::template divide<Lhs, Rhs>;
            return divide<typename operator_policy::result_type>(
                    static_cast<typename operator_policy::lhs_type>(lhs),
                    static_cast<typename operator_policy::rhs_type>(rhs));
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) comparison operators

    template<class Rep, int Exponent>
    constexpr bool operator==(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()==rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator!=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()!=rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator<(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()<rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator>(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()>rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator>=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()>=rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator<=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()<=rhs.data();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) arithmetic operators

    // negate
    template<class RhsRep, int RhsExponent>
    constexpr auto operator-(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> typename _fixed_point_impl::default_arithmetic_policy::negate<
            fixed_point<RhsRep, RhsExponent>>::result_type
    {
        return _fixed_point_impl::policy_negate<_fixed_point_impl::default_arithmetic_policy>(rhs);
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator+(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> typename _fixed_point_impl::default_arithmetic_policy::add<
            fixed_point<LhsRep, LhsExponent>,
            fixed_point<RhsRep, RhsExponent>>::result_type
    {
        return _fixed_point_impl::policy_add<_fixed_point_impl::default_arithmetic_policy>(lhs, rhs);
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator-(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> typename _fixed_point_impl::default_arithmetic_policy::subtract<
            fixed_point<LhsRep, LhsExponent>,
            fixed_point<RhsRep, RhsExponent>>::result_type
    {
        return _fixed_point_impl::policy_subtract<_fixed_point_impl::default_arithmetic_policy>(lhs, rhs);
    }

    // fixed-point, fixed-point -> fixed-point
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> typename _fixed_point_impl::default_arithmetic_policy::multiply<
            fixed_point<LhsRep, LhsExponent>,
            fixed_point<RhsRep, RhsExponent>>::result_type
    {
        return _fixed_point_impl::policy_multiply<_fixed_point_impl::default_arithmetic_policy>(lhs, rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> typename _fixed_point_impl::default_arithmetic_policy::divide<
            fixed_point<LhsRep, LhsExponent>,
            fixed_point<RhsRep, RhsExponent>>::result_type
    {
        return _fixed_point_impl::policy_divide<_fixed_point_impl::default_arithmetic_policy>(lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    template<class Lhs, class Rhs>
    constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)==static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)!=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<=static_cast<common_type>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ non-fixed_point) arithmetic operators

    template<class Lhs, class Rhs>
    constexpr auto operator+(
            const Lhs& lhs,
            const Rhs& rhs)
    -> _fixed_point_impl::common_type_t<Lhs, Rhs>
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)+static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator-(
            const Lhs& lhs,
            const Rhs& rhs)
    -> _fixed_point_impl::common_type_t<Lhs, Rhs>
    {
        using common_type = _fixed_point_impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)-static_cast<common_type>(rhs);
    }

    // fixed-point, integer -> fixed-point
    template<
            class Rep, int Exponent,
            class Integer,
            typename = typename std::enable_if<is_integral<Integer>::value>::type>
    constexpr auto operator*(const fixed_point<Rep, Exponent>& lhs, const Integer& rhs)
    -> fixed_point<decltype(std::declval<Rep>()*std::declval<Integer>()), Exponent>
    {
        using repr_type = fixed_point<decltype(std::declval<Rep>()*std::declval<Integer>()), Exponent>;
        return multiply<repr_type>(lhs, fixed_point<Integer>(rhs));
    }

    template<
            class Rep, int Exponent,
            class Integer,
            typename = typename std::enable_if<is_integral<Integer>::value>::type>
    constexpr auto operator/(const fixed_point<Rep, Exponent>& lhs, const Integer& rhs)
    -> fixed_point<decltype(std::declval<Rep>()/std::declval<Integer>()), Exponent>
    {
        using result_type = fixed_point<decltype(std::declval<Rep>()/std::declval<Integer>()), Exponent>;
        return divide<result_type>(lhs, fixed_point<Integer>(rhs));
    }

    // integer. fixed-point -> fixed-point
    template<
            class Integer,
            class Rep, int Exponent,
            typename = typename std::enable_if<is_integral<Integer>::value>::type>
    constexpr auto operator*(const Integer& lhs, const fixed_point<Rep, Exponent>& rhs)
    -> fixed_point<decltype(std::declval<Integer>()*std::declval<Rep>()), Exponent>
    {
        using result_type = fixed_point<decltype(std::declval<Integer>()*std::declval<Rep>()), Exponent>;
        return multiply<result_type>(fixed_point<Integer>(lhs), rhs);
    }

    template<
            class Integer,
            class Rep, int Exponent,
            typename = typename std::enable_if<is_integral<Integer>::value>::type>
    constexpr auto operator/(const Integer& lhs, const fixed_point<Rep, Exponent>& rhs)
    -> fixed_point<decltype(std::declval<Integer>()/std::declval<Rep>()), Exponent>
    {
        using result_type = fixed_point<decltype(std::declval<Integer>()/std::declval<Rep>()), Exponent>;
        return divide<result_type>(fixed_point<Integer>(lhs), rhs);
    }

    // fixed-point, floating-point -> floating-point
    template<class LhsRep, int LhsExponent, class Float>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const Float& rhs)
    -> _fixed_point_impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type>
    {
        using result_type = _fixed_point_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, Float>;
        return static_cast<result_type>(lhs)*rhs;
    }

    template<class LhsRep, int LhsExponent, class Float>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const Float& rhs)
    -> _fixed_point_impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type>
    {
        using result_type = _fixed_point_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, Float>;
        return static_cast<result_type>(lhs)/rhs;
    }

    // floating-point, fixed-point -> floating-point
    template<class Float, class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const Float& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _fixed_point_impl::common_type_t<
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _fixed_point_impl::common_type_t<fixed_point<RhsRep, RhsExponent>, Float>;
        return lhs*static_cast<result_type>(rhs);
    }

    template<class Float, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const Float& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _fixed_point_impl::common_type_t<
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _fixed_point_impl::common_type_t<fixed_point<RhsRep, RhsExponent>, Float>;
        return lhs/
                static_cast<result_type>(rhs);
    }

    template<class LhsRep, int Exponent, class Rhs>
    fixed_point<LhsRep, Exponent>& operator+=(fixed_point<LhsRep, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs = lhs+fixed_point<LhsRep, Exponent>(rhs);
    }

    template<class LhsRep, int Exponent, class Rhs>
    fixed_point<LhsRep, Exponent>& operator-=(fixed_point<LhsRep, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs = lhs-fixed_point<LhsRep, Exponent>(rhs);
    }

    template<class LhsRep, int Exponent>
    template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator*=(const Rhs& rhs)
    {
        _r *= static_cast<repr_type>(rhs);
        return *this;
    }

    template<class LhsRep, int Exponent>
    template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator/=(const Rhs& rhs)
    {
        _r /= static_cast<repr_type>(rhs);
        return *this;
    }
}

#include "bits/fixed_point_extras.h"

#endif	// defined(_SG14_FIXED_POINT)
