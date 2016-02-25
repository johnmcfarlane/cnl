
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::fixed_point` type

#if !defined(_SG14_FIXED_POINT)
#define _SG14_FIXED_POINT 1

#include <climits>
#include <cinttypes>

#include "type_traits.h"

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
    // general-purpose _impl definitions

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // num_bits

        template<typename T>
        constexpr int num_bits()
        {
            return sizeof(T)*CHAR_BIT;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::make_float

        template<int NumBytes>
        struct _make_float;

        // specializations
        template<>
        struct _make_float<1> {
            using type = float;
        };
        template<>
        struct _make_float<2> {
            using type = float;
        };
        template<>
        struct _make_float<4> {
            using type = float;
        };
        template<>
        struct _make_float<8> {
            using type = double;
        };
        template<>
        struct _make_float<16> {
            using type = long double;
        };

        template<int NumBytes>
        using make_float = typename _make_float<NumBytes>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::next_size

        // given an integral type, IntType,
        // provides the integral type of the equivalent type with twice the size
        template<class IntType>
        using next_size = typename sg14::resize_t<IntType, sizeof(IntType)*2>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::previous_size

        // given an integral type, IntType,
        // provides the integral type of the equivalent type with half the size
        template<class IntType>
        using previous_size = typename sg14::resize_t<IntType, sizeof(IntType)/2>;

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
            static_assert(std::is_integral<Input>::value, "Input must be integral type");
            static_assert(std::is_integral<Output>::value, "Output must be integral type");

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
            static_assert(std::is_integral<Input>::value, "Input must be integral type");
            static_assert(std::is_integral<Output>::value, "Output must be integral type");

            // cast only
            return static_cast<Output>(i);
        }

        // sizeof(Input) >= sizeof(Output)
        template<
                int Exponent,
                class Output,
                class Input,
                typename std::enable_if<
                        !(Exponent<=0) && sizeof(Output)<=sizeof(Input) && std::is_unsigned<Input>::value,
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
                        !(Exponent<=0) && !(sizeof(Output)<=sizeof(Input)) && std::is_unsigned<Input>::value,
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
                        !(Exponent<=0) && std::is_signed<Input>::value,
                        int>::type Dummy = 0>
        constexpr Output shift_left(Input i)
        {
            using unsigned_input = typename std::make_unsigned<Input>::type;
            using signed_output = typename std::make_signed<Output>::type;

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
        // sg14::_impl::pow2

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
        // sg14::_impl::max

        template<class T>
        constexpr const T& max(const T& a, const T& b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_repr_type

        // given two integral types, produces a common type with enough capacity to
        // store values of either (except when one is signed and both are same size)
        template<class LhsReprType, class RhsReprType>
        using common_repr_type = typename sg14::resize_t<
                typename std::conditional<
                        std::is_signed<LhsReprType>::value | std::is_signed<RhsReprType>::value,
                        typename std::make_signed<typename std::common_type<LhsReprType, RhsReprType>::type>::type,
                        typename std::make_unsigned<typename std::common_type<LhsReprType, RhsReprType>::type>::type>::type,
                _impl::max(sizeof(LhsReprType), sizeof(RhsReprType))>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::capacity

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

        ////////////////////////////////////////////////////////////////////////////////
        // _impl::sufficient_repr

        // given a required number of bits a type should have and whether it is signed,
        // provides a built-in integral type with necessary capacity
        template<unsigned RequiredBits, class Archetype>
        using sufficient_repr
        = sg14::resize_t<Archetype, 1 << (capacity<((RequiredBits+7)/8)-1>::value)>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::sqrt helper functions

        template<class ReprType>
        constexpr ReprType sqrt_bit(
                ReprType n,
                ReprType bit = ReprType(1) << (num_bits<ReprType>()-2))
        {
            return (bit>n) ? sqrt_bit<ReprType>(n, bit >> 2) : bit;
        }

        template<class ReprType>
        constexpr ReprType sqrt_solve3(
                ReprType n,
                ReprType bit,
                ReprType result)
        {
            return bit
                   ? (n>=result+bit)
                     ? sqrt_solve3<ReprType>(n-(result+bit), bit >> 2, (result >> 1)+bit)
                     : sqrt_solve3<ReprType>(n, bit >> 2, result >> 1)
                   : result;
        }

        template<class ReprType>
        constexpr ReprType sqrt_solve1(ReprType n)
        {
            return sqrt_solve3<ReprType>(n, sqrt_bit<ReprType>(n), 0);
        }
    }

    /// \brief literal arithmetic type which approximates a real number
    ///
    /// \tparam ReprType the underlying type used to represent the value
    /// \tparam Exponent the value by which to scale the integer value in order to get the real value
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 3 integer bits and 4 fractional bits:
    /// \snippet snippets.cpp define a fixed_point value

    template<class ReprType = int, int Exponent = 0>
    class fixed_point {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        /// alias to template parameter, \a ReprType
        using repr_type = ReprType;

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// number of binary digits this type can represent;
        /// equivalent to [std::numeric_limits::digits](http://en.cppreference.com/w/cpp/types/numeric_limits/digits)
        constexpr static int digits = _impl::num_bits<ReprType>()-std::is_signed<repr_type>::value;

        /// number of binary digits devoted to integer part of value;
        /// can be negative for specializations with especially small ranges
        constexpr static int integer_digits = digits+exponent;

        /// number of binary digits devoted to fractional part of value;
        /// can be negative for specializations with especially large ranges
        constexpr static int fractional_digits = digits-integer_digits;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

    private:
        // constructor taking representation explicitly using operator++(int)-style trick
        constexpr fixed_point(repr_type repr, int)
                :_repr(repr)
        {
        }

    public:
        /// default constructor
        fixed_point() { }

        /// constructor taking an integer type
        template<class S, typename std::enable_if<std::is_integral<S>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_repr(integral_to_repr(s))
        {
        }

        /// constructor taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_repr(floating_point_to_repr(s))
        {
        }

        /// constructor taking a fixed-point type
        template<class FromReprType, int FromExponent>
        explicit constexpr fixed_point(const fixed_point<FromReprType, FromExponent>& rhs)
                :_repr(fixed_point_to_repr(rhs))
        {
        }

        /// copy assignment operator taking an integer type
        template<class S, typename std::enable_if<std::is_integral<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _repr = integral_to_repr(s);
            return *this;
        }

        /// copy assignment operator taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _repr = floating_point_to_repr(s);
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromReprType, int FromExponent>
        fixed_point& operator=(const fixed_point<FromReprType, FromExponent>& rhs)
        {
            _repr = fixed_point_to_repr(rhs);
            return *this;
        }

        /// returns value represented as floating-point
        template<class S, typename std::enable_if<std::is_integral<S>::value, int>::type Dummy = 0>
        explicit constexpr operator S() const
        {
            return repr_to_integral<S>(_repr);
        }

        /// returns value represented as integral
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr operator S() const
        {
            return repr_to_floating_point<S>(_repr);
        }

        /// returns non-zeroness represented as boolean
        explicit constexpr operator bool() const
        {
            return _repr!=0;
        }

        template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy = 0>
        fixed_point& operator*=(const Rhs& rhs);

        template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy = 0>
        fixed_point& operator/=(const Rhs& rhs);

        /// returns internal representation of value
        constexpr repr_type data() const
        {
            return _repr;
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
            return _impl::pow2<S, -exponent>();
        }

        template<class S, typename std::enable_if<std::is_integral<S>::value, int>::type Dummy = 0>
        static constexpr S one()
        {
            return integral_to_repr<S>(1);
        }

        template<class S>
        static constexpr S inverse_one()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return _impl::pow2<S, exponent>();
        }

        template<class S>
        static constexpr repr_type integral_to_repr(S s)
        {
            static_assert(std::is_integral<S>::value, "S must be unsigned integral type");

            return _impl::shift_right<exponent, repr_type>(s);
        }

        template<class S>
        static constexpr S repr_to_integral(repr_type r)
        {
            static_assert(std::is_integral<S>::value, "S must be unsigned integral type");

            return _impl::shift_left<exponent, S>(r);
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

        template<class FromReprType, int FromExponent>
        static constexpr repr_type fixed_point_to_repr(const fixed_point<FromReprType, FromExponent>& rhs)
        {
            return _impl::shift_right<(exponent-FromExponent), repr_type>(rhs.data());
        }

        ////////////////////////////////////////////////////////////////////////////////
        // variables

        repr_type _repr;
    };

    /// \brief Produce a fixed-point type with the given number of integer and fractional digits.
    ///
    /// \tparam IntegerDigits specifies minimum value of @ref fixed_point::integer_digits
    /// \tparam FractionalDigits specifies minimum value of @ref fixed_point::fractional_digits
    /// \tparam Archetype hints at the type of @ref fixed_point::repr_type
    ///
    /// \remarks The signage of \a Archetype specifies signage of the resultant fixed-point type.
    /// \remarks Typical choices for \a Archetype, `signed` and `unsigned`,
    /// result in a type that uses built-in integers for \a fixed_point::repr_type.
    /// \remarks Resultant type is signed by default.
    ///
    /// \par Example:
    ///
    /// To generate a 2-byte fixed-point type with a sign bit, 7 integer bits and 8 fractional bits:
    /// \snippet snippets.cpp use make_fixed
    ///
    /// \sa make_ufixed
    template<unsigned IntegerDigits, unsigned FractionalDigits = 0, class Archetype = signed>
    using make_fixed = fixed_point<
            _impl::sufficient_repr<IntegerDigits+FractionalDigits+std::is_signed<Archetype>::value, Archetype>,
            int(IntegerDigits)
                    +int(std::is_signed<Archetype>::value)
                    -_impl::num_bits<
                            _impl::sufficient_repr<
                                    IntegerDigits+FractionalDigits+int(std::is_signed<Archetype>::value),
                                    Archetype>>()>;

    /// \brief Produce an unsigned fixed-point type with the given number of integer and fractional digits.
    ///
    /// \sa make_fixed
    template<unsigned IntegerDigits, unsigned FractionalDigits = 0, class Archetype = unsigned>
    using make_ufixed = make_fixed<
            IntegerDigits,
            FractionalDigits,
            typename std::make_unsigned<Archetype>::type>;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_fixed_from_repr

    // yields a fixed_point with Exponent calculated such that
    // fixed_point<ReprType, Exponent>::integer_bits == IntegerDigits
    template<class ReprType, int IntegerDigits>
    using make_fixed_from_repr = fixed_point<
            ReprType,
            IntegerDigits+std::is_signed<ReprType>::value-(signed) sizeof(ReprType)*CHAR_BIT>;

    /// produces equivalent fixed-point type at a new size
    ///
    /// \tparam ReprType the \a ReprType parameter of @ref fixed_point
    /// \tparam Exponent the \a Exponent parameter of @ref fixed_point
    /// \tparam NumBytes the desired size of the resultant type such that `(sizeof(type) >= NumBytes)`
    ///
    /// \sa resize_t
    template<class ReprType, int Exponent, int NumBytes>
    struct resize<fixed_point<ReprType, Exponent>, NumBytes> {
        /// resultant type; a fixed_point specialization that is at least \a NumBytes bytes in size
        using type = fixed_point<resize_t<ReprType, NumBytes>, Exponent>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::promote_result / promote

    // given template parameters of a fixed_point specialization,
    // yields alternative specialization with twice the fractional bits
    // and twice the integral/sign bits
    template<class FixedPoint>
    using promote_result = fixed_point<
            _impl::next_size<typename FixedPoint::repr_type>,
            FixedPoint::exponent*2>;

    // as promote_result but promotes parameter, from
    template<class FixedPoint>
    promote_result<FixedPoint>
    constexpr promote(const FixedPoint& from)
    {
        return promote_result<FixedPoint>(from);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::demote_result / demote

    // given template parameters of a fixed_point specialization,
    // yields alternative specialization with half the fractional bits
    // and half the integral/sign bits (assuming Exponent is even)
    template<class FixedPoint>
    using demote_result = fixed_point<
            _impl::previous_size<typename FixedPoint::repr_type>,
            FixedPoint::exponent/2>;

    // as demote_result but demotes parameter, from
    template<class FixedPoint>
    demote_result<FixedPoint>
    constexpr demote(const FixedPoint& from)
    {
        return demote_result<FixedPoint>(from);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point-aware _impl definitions

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_fixed_point

        template<class T>
        struct is_fixed_point;

        template<class T>
        struct is_fixed_point
                : public std::integral_constant<bool, false> {
        };

        template<class ReprType, int Exponent>
        struct is_fixed_point<fixed_point<ReprType, Exponent>>
                : public std::integral_constant<bool, true> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::_common_type

        template<class Lhs, class Rhs, class _Enable = void>
        struct _common_type;

        // given two fixed-point, produces the type that is best suited to both of them
        template<class LhsReprType, int LhsExponent, class RhsReprType, int RhsExponent>
        struct _common_type<fixed_point<LhsReprType, LhsExponent>, fixed_point<RhsReprType, RhsExponent>> {
            using type = make_fixed_from_repr<
                    _impl::common_repr_type<LhsReprType, RhsReprType>,
                    _impl::max<int>(
                            fixed_point<LhsReprType, LhsExponent>::integer_digits,
                            fixed_point<RhsReprType, RhsExponent>::integer_digits)>;
        };

        // given a fixed-point and a integer type,
        // generates a fixed-point type that is as big as both of them (or as close as possible)
        template<class LhsReprType, int LhsExponent, class RhsInteger>
        struct _common_type<
                fixed_point<LhsReprType, LhsExponent>,
                RhsInteger,
                typename std::enable_if<std::is_integral<RhsInteger>::value>::type> {
            using type = fixed_point<LhsReprType, LhsExponent>;
        };

        // given a fixed-point and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsReprType, int LhsExponent, class Float>
        struct _common_type<
                fixed_point<LhsReprType, LhsExponent>,
                Float,
                typename std::enable_if<std::is_floating_point<Float>::value>::type>
                : std::common_type<_impl::make_float<sizeof(LhsReprType)>, Float> {
        };

        // when first type is not fixed-point and second type is, reverse the order
        template<class Lhs, class RhsReprType, int RhsExponent>
        struct _common_type<Lhs, fixed_point<RhsReprType, RhsExponent>>
                : _common_type<fixed_point<RhsReprType, RhsExponent>, Lhs> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_type

        // similar to std::common_type
        // but one or both input types must be fixed_point
        template<class Lhs, class Rhs>
        using common_type = typename _common_type<Lhs, Rhs>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // arithmetic result types

        template<typename LhsFixedPoint, typename RhsFixedPoint>
        using subtract_result_repr = typename std::make_signed<common_repr_type<LhsFixedPoint, RhsFixedPoint>>::type;

        template<typename Repr>
        using square_result_repr = typename std::make_unsigned<Repr>::type;

        template<typename FixedPoint>
        using sqrt_result_repr = typename std::make_unsigned<FixedPoint>::type;

        // all other operations
        template<typename LhsFixedPoint, typename RhsFixedPoint>
        using arithmetic_result_repr = common_repr_type<LhsFixedPoint, RhsFixedPoint>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::promote_fast_result / promote_fast

        // given template parameters of a fixed_point specialization,
        // yields alternative specialization with twice the capacity
        // and the same number of factional bits; requires no bit shift
        template<class FixedPoint>
        using promote_fast_result = fixed_point<
                _impl::next_size<typename FixedPoint::repr_type>,
                FixedPoint::exponent>;

        // as promote_result but promotes parameter, from
        template<class FixedPoint>
        promote_fast_result<FixedPoint>
        constexpr promote_fast(const FixedPoint& from)
        {
            return promote_fast_result<FixedPoint>(from);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::multiply

        template<class Result, class Lhs, class Rhs>
        constexpr Result multiply(const Lhs& lhs, const Rhs& rhs)
        {
            using result_repr_type = typename Result::repr_type;
            return Result::from_data(
                    _impl::shift_left<(Lhs::exponent+Rhs::exponent-Result::exponent), result_repr_type>(
                            static_cast<result_repr_type>(lhs.data())
                                    *static_cast<result_repr_type>(rhs.data())));
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::divide

        template<class FixedPointQuotient, class FixedPointDividend, class FixedPointDivisor>
        constexpr FixedPointQuotient divide(const FixedPointDividend& lhs, const FixedPointDivisor& rhs)
        {
            using result_repr_type = typename FixedPointQuotient::repr_type;

            // a fixed-point type which is capable of holding the value passed in to lhs
            // and the result of the lhs / rhs; depending greatly on the exponent of each
            using intermediate_type = make_fixed<
                    _impl::max(FixedPointQuotient::integer_digits,
                            FixedPointDividend::integer_digits+FixedPointDivisor::fractional_digits),
                    _impl::max(FixedPointQuotient::fractional_digits,
                            FixedPointDividend::fractional_digits+FixedPointDivisor::integer_digits),
                    typename std::conditional<
                            std::is_signed<typename FixedPointQuotient::repr_type>::value
                                    | std::is_signed<typename FixedPointDividend::repr_type>::value,
                            signed,
                            unsigned>::type>;

            return FixedPointQuotient::from_data(
                    _impl::shift_left<
                            (intermediate_type::exponent-FixedPointDivisor::exponent-FixedPointQuotient::exponent),
                            result_repr_type>
                            (static_cast<intermediate_type>(lhs).data()/rhs.data()));
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::add

        template<class Result, class FixedPoint, class Head>
        constexpr Result add(const Head& addend_head)
        {
            static_assert(std::is_same<FixedPoint, Head>::value, "mismatched add parameters");
            return static_cast<Result>(addend_head);
        }

        template<class Result, class FixedPoint, class Head, class ... Tail>
        constexpr Result add(const Head& addend_head, const Tail& ... addend_tail)
        {
            static_assert(std::is_same<FixedPoint, Head>::value, "mismatched add parameters");
            return add<Result, FixedPoint, Tail ...>(addend_tail ...)+static_cast<Result>(addend_head);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // homogeneous (mixed-mode) operator overloads
    //
    // taking one or two identical fixed_point specializations

    template<class ReprType, int Exponent>
    constexpr bool operator==(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs.data()==rhs.data();
    }

    template<class ReprType, int Exponent>
    constexpr bool operator!=(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs.data()!=rhs.data();
    }

    template<class ReprType, int Exponent>
    constexpr bool operator<(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs.data()<rhs.data();
    }

    template<class ReprType, int Exponent>
    constexpr bool operator>(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs.data()>rhs.data();
    }

    template<class ReprType, int Exponent>
    constexpr bool operator>=(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs.data()>=rhs.data();
    }

    template<class ReprType, int Exponent>
    constexpr bool operator<=(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs.data()<=rhs.data();
    }

    // arithmetic
    template<class ReprType, int Exponent>
    constexpr fixed_point<ReprType, Exponent> operator-(
            const fixed_point<ReprType, Exponent>& rhs)
    {
        static_assert(std::is_signed<ReprType>::value, "unary negation of unsigned value");

        return fixed_point<ReprType, Exponent>::from_data(-rhs.data());
    }

    template<class ReprType, int Exponent>
    constexpr fixed_point<ReprType, Exponent> operator+(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return fixed_point<ReprType, Exponent>::from_data(lhs.data()+rhs.data());
    }

    template<class ReprType, int Exponent>
    constexpr fixed_point<ReprType, Exponent> operator-(
            const fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return fixed_point<ReprType, Exponent>::from_data(lhs.data()-rhs.data());
    }

    template<class ReprType, int Exponent>
    fixed_point<ReprType, Exponent>& operator+=(
            fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs = lhs+rhs;
    }

    template<class ReprType, int Exponent>
    fixed_point<ReprType, Exponent>& operator-=(
            fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs = lhs-rhs;
    }

    template<class ReprType, int Exponent>
    fixed_point<ReprType, Exponent>& operator*=(
            fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs = lhs*rhs;
    }

    template<class ReprType, int Exponent>
    fixed_point<ReprType, Exponent>& operator/=(
            fixed_point<ReprType, Exponent>& lhs,
            const fixed_point<ReprType, Exponent>& rhs)
    {
        return lhs = lhs/rhs;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    template<class Lhs, class Rhs>
    constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)==static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)!=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<=static_cast<common_type>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // arithmetic

    template<class Lhs, class Rhs>
    constexpr auto operator+(
            const Lhs& lhs,
            const Rhs& rhs)
    -> _impl::common_type<Lhs, Rhs>
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)+static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator-(
            const Lhs& lhs,
            const Rhs& rhs)
    -> _impl::common_type<Lhs, Rhs>
    {
        using common_type = _impl::common_type<Lhs, Rhs>;
        return static_cast<common_type>(lhs)-static_cast<common_type>(rhs);
    }

    // fixed-point, fixed-point -> fixed-point
    template<class LhsReprType, int LhsExponent, class RhsReprType, int RhsExponent>
    constexpr auto operator*(
            const fixed_point<LhsReprType, LhsExponent>& lhs,
            const fixed_point<RhsReprType, RhsExponent>& rhs)
    -> _impl::common_type<fixed_point<LhsReprType, LhsExponent>, fixed_point<RhsReprType, RhsExponent>>
    {
        using lhs_type = fixed_point<LhsReprType, LhsExponent>;
        using rhs_type = fixed_point<RhsReprType, RhsExponent>;
        using result_type = _impl::common_type<lhs_type, rhs_type>;
        using intermediate_type = _impl::promote_fast_result<result_type>;

        return static_cast<result_type>(_impl::multiply<intermediate_type>(lhs, rhs));
    }

    template<class LhsReprType, int LhsExponent, class RhsReprType, int RhsExponent>
    constexpr auto operator/(
            const fixed_point<LhsReprType, LhsExponent>& lhs,
            const fixed_point<RhsReprType, RhsExponent>& rhs)
    -> _impl::common_type<fixed_point<LhsReprType, LhsExponent>, fixed_point<RhsReprType, RhsExponent>>
    {
        using result_type = _impl::common_type<fixed_point<LhsReprType, LhsExponent>, fixed_point<RhsReprType, RhsExponent>>;
        return _impl::divide<result_type>(lhs, rhs);
    }

    // fixed-point, integer -> fixed-point
    template<class LhsReprType, int LhsExponent, class Integer>
    constexpr auto operator*(
            const fixed_point<LhsReprType, LhsExponent>& lhs,
            const Integer& rhs)
    -> typename std::enable_if<std::is_integral<Integer>::value, fixed_point<LhsReprType, LhsExponent>>::type
    {
        using result_type = fixed_point<LhsReprType, LhsExponent>;
        return _impl::multiply<result_type>(lhs, fixed_point<Integer>(rhs));
    }

    template<class LhsReprType, int LhsExponent, class Integer>
    constexpr auto operator/(
            const fixed_point<LhsReprType, LhsExponent>& lhs,
            const Integer& rhs)
    -> typename std::enable_if<std::is_integral<Integer>::value, fixed_point<LhsReprType, LhsExponent>>::type
    {
        using result_type = fixed_point<LhsReprType, LhsExponent>;
        return _impl::divide<result_type>(lhs, fixed_point<Integer>(rhs));
    }

    // integer. fixed-point -> fixed-point
    template<class Integer, class RhsReprType, int RhsExponent>
    constexpr auto operator*(
            const Integer& lhs,
            const fixed_point<RhsReprType, RhsExponent>& rhs)
    -> typename std::enable_if<std::is_integral<Integer>::value, fixed_point<RhsReprType, RhsExponent>>::type
    {
        using result_type = fixed_point<RhsReprType, RhsExponent>;
        return _impl::multiply<result_type>(fixed_point<Integer>(lhs), rhs);
    }

    template<class Integer, class RhsReprType, int RhsExponent>
    constexpr auto operator/(
            const Integer& lhs,
            const fixed_point<RhsReprType, RhsExponent>& rhs)
    -> typename std::enable_if<std::is_integral<Integer>::value, fixed_point<RhsReprType, RhsExponent>>::type
    {
        using result_type = fixed_point<RhsReprType, RhsExponent>;
        return _impl::divide<result_type>(fixed_point<Integer>(lhs), rhs);
    }

    // fixed-point, floating-point -> floating-point
    template<class LhsReprType, int LhsExponent, class Float>
    constexpr auto operator*(
            const fixed_point<LhsReprType, LhsExponent>& lhs,
            const Float& rhs)
    -> _impl::common_type<
            fixed_point<LhsReprType, LhsExponent>,
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type>
    {
        using result_type = _impl::common_type<fixed_point<LhsReprType, LhsExponent>, Float>;
        return static_cast<result_type>(lhs)*rhs;
    }

    template<class LhsReprType, int LhsExponent, class Float>
    constexpr auto operator/(
            const fixed_point<LhsReprType, LhsExponent>& lhs,
            const Float& rhs)
    -> _impl::common_type<
            fixed_point<LhsReprType, LhsExponent>,
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type>
    {
        using result_type = _impl::common_type<fixed_point<LhsReprType, LhsExponent>, Float>;
        return static_cast<result_type>(lhs)/rhs;
    }

    // floating-point, fixed-point -> floating-point
    template<class Float, class RhsReprType, int RhsExponent>
    constexpr auto operator*(
            const Float& lhs,
            const fixed_point<RhsReprType, RhsExponent>& rhs)
    -> _impl::common_type<
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type,
            fixed_point<RhsReprType, RhsExponent>>
    {
        using result_type = _impl::common_type<fixed_point<RhsReprType, RhsExponent>, Float>;
        return lhs*static_cast<result_type>(rhs);
    }

    template<class Float, class RhsReprType, int RhsExponent>
    constexpr auto operator/(
            const Float& lhs,
            const fixed_point<RhsReprType, RhsExponent>& rhs)
    -> _impl::common_type<
            typename std::enable_if<std::is_floating_point<Float>::value, Float>::type,
            fixed_point<RhsReprType, RhsExponent>>
    {
        using result_type = _impl::common_type<fixed_point<RhsReprType, RhsExponent>, Float>;
        return lhs/
                static_cast<result_type>(rhs);
    }

    template<class LhsReprType, int Exponent, class Rhs>
    fixed_point<LhsReprType, Exponent>& operator+=(fixed_point<LhsReprType, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs += fixed_point<LhsReprType, Exponent>(rhs);
    }

    template<class LhsReprType, int Exponent, class Rhs>
    fixed_point<LhsReprType, Exponent>& operator-=(fixed_point<LhsReprType, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs -= fixed_point<LhsReprType, Exponent>(rhs);
    }

    template<class LhsReprType, int Exponent>
    template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy>
    fixed_point<LhsReprType, Exponent>&
    fixed_point<LhsReprType, Exponent>::operator*=(const Rhs& rhs)
    {
        _repr *= static_cast<repr_type>(rhs);
        return *this;
    }

    template<class LhsReprType, int Exponent>
    template<class Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type Dummy>
    fixed_point<LhsReprType, Exponent>&
    fixed_point<LhsReprType, Exponent>::operator/=(const Rhs& rhs)
    {
        _repr /= static_cast<repr_type>(rhs);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::sqrt

    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    // placeholder implementation; slow when calculated at run-time?
    template<class ReprType, int Exponent>
    constexpr fixed_point<ReprType, Exponent>
    sqrt(const fixed_point<ReprType, Exponent>& x)
    {
        return
#if defined(_SG14_FIXED_POINT_EXCEPTIONS_ENABLED)
                (x<fixed_point<ReprType, Exponent>(0))
                ? throw std::invalid_argument("cannot represent square root of negative value") :
                #endif
                fixed_point<ReprType, Exponent>::from_data(
                        static_cast<ReprType>(_impl::sqrt_solve1(promote(x).data())));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_add_result / trunc_add

    // yields specialization of fixed_point with integral bits necessary to store
    // result of an addition between N values of fixed_point<ReprType, Exponent>
    template<class FixedPoint, unsigned N = 2>
    using trunc_add_result = make_fixed_from_repr<
            typename FixedPoint::repr_type,
            FixedPoint::integer_digits+_impl::capacity<N-1>::value>;

    template<class FixedPoint, class ... Tail>
    trunc_add_result<FixedPoint, sizeof...(Tail)+1>
    constexpr trunc_add(const FixedPoint& addend1, const Tail& ... addend_tail)
    {
        using output_type = trunc_add_result<FixedPoint, sizeof...(Tail)+1>;
        return _impl::add<output_type, FixedPoint>(addend1, addend_tail ...);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_subtract_result / trunc_subtract

    // yields specialization of fixed_point with integral bits necessary to store
    // result of an subtraction between N values of fixed_point<ReprType, Exponent>
    template<class Lhs, class Rhs = Lhs>
    using trunc_subtract_result = make_fixed_from_repr<
            _impl::subtract_result_repr<typename Lhs::repr_type, typename Rhs::repr_type>,
            _impl::max(Lhs::integer_digits, Rhs::integer_digits)+1>;

    template<class Lhs, class Rhs>
    trunc_subtract_result<Lhs, Rhs>
    constexpr trunc_subtract(const Lhs& minuend, const Rhs& subtrahend)
    {
        using output_type = trunc_subtract_result<Lhs, Rhs>;
        return static_cast<output_type>(minuend)-static_cast<output_type>(subtrahend);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_multiply_result / trunc_multiply

    // yields specialization of fixed_point with integral bits necessary to store
    // result of a multiply between values of fixed_point<ReprType, Exponent>
    template<class Lhs, class Rhs = Lhs>
    using trunc_multiply_result = make_fixed_from_repr<
            _impl::arithmetic_result_repr<typename Lhs::repr_type, typename Rhs::repr_type>,
            Lhs::integer_digits+Rhs::integer_digits>;

    // as trunc_multiply_result but converts parameter, factor,
    // ready for safe binary multiply
    template<class Lhs, class Rhs>
    trunc_multiply_result<Lhs, Rhs>
    constexpr trunc_multiply(const Lhs& lhs, const Rhs& rhs)
    {
        using result_type = trunc_multiply_result<Lhs, Rhs>;
        using intermediate_type = _impl::promote_fast_result<result_type>;
        return static_cast<result_type>(_impl::multiply<intermediate_type>(lhs, rhs));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_divide_result / trunc_divide

    // yields specialization of fixed_point with integral bits necessary to store
    // result of a divide between values of types, Lhs and Rhs
    template<class FixedPointDividend, class FixedPointDivisor = FixedPointDividend>
    using trunc_divide_result = make_fixed_from_repr<
            _impl::arithmetic_result_repr<typename FixedPointDividend::repr_type, typename FixedPointDivisor::repr_type>,
            FixedPointDividend::integer_digits+FixedPointDivisor::fractional_digits>;

    // as trunc_divide_result but converts parameter, factor,
    // ready for safe binary divide
    template<class FixedPointDividend, class FixedPointDivisor>
    trunc_divide_result<FixedPointDividend, FixedPointDivisor>
    constexpr trunc_divide(const FixedPointDividend& lhs, const FixedPointDivisor& rhs)
    {
        using result_type = trunc_divide_result<FixedPointDividend, FixedPointDivisor>;
        return _impl::divide<result_type>(lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_reciprocal_result / trunc_reciprocal

    // yields specialization of fixed_point with integral bits necessary to store
    // result of inverse of value of type FixedPoint
    template<class FixedPoint>
    using trunc_reciprocal_result = make_fixed_from_repr<
            typename FixedPoint::repr_type,
            FixedPoint::fractional_digits+1>;

    // returns reciprocal of fixed_point in same-sized fixed-point type
    // that can comfortably store significant digits of result
    template<class FixedPoint>
    trunc_reciprocal_result<FixedPoint>
    constexpr trunc_reciprocal(const FixedPoint& fixed_point)
    {
        using result_type = trunc_reciprocal_result<FixedPoint>;
        using result_repr_type = typename result_type::repr_type;

        using dividend_type = make_fixed_from_repr<result_repr_type, 1>;

        return _impl::divide<result_type>(dividend_type(1), fixed_point);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_square_result / trunc_square

    // yields specialization of fixed_point with integral bits necessary to store
    // result of a multiply between values of fixed_point<ReprType, Exponent>
    // whose sign bit is set to the same value
    template<class FixedPoint>
    using trunc_square_result = make_fixed_from_repr<
            _impl::square_result_repr<typename FixedPoint::repr_type>,
            FixedPoint::integer_digits*2>;

    // as trunc_square_result but converts parameter, factor,
    // ready for safe binary multiply-by-self
    template<class FixedPoint>
    trunc_square_result<FixedPoint>
    constexpr trunc_square(const FixedPoint& root)
    {
        using result_type = trunc_square_result<FixedPoint>;
		using intermediate_type = _impl::promote_fast_result<result_type>;
		return static_cast<result_type>(_impl::multiply<intermediate_type>(root, root));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_sqrt_result / trunc_sqrt

    // yields specialization of fixed_point with integral bits necessary to store
    // the positive result of a square root operation on an object of type,
    // fixed_point<ReprType, Exponent>
    template<class FixedPoint>
    using trunc_sqrt_result = make_fixed_from_repr<
            _impl::sqrt_result_repr<typename FixedPoint::repr_type>,
            (FixedPoint::integer_digits+1)/2>;

    // as trunc_sqrt_result but converts parameter, factor,
    // ready for safe sqrt operation
    template<class FixedPoint>
    trunc_sqrt_result<FixedPoint>
    constexpr trunc_sqrt(const FixedPoint& square)
    {
        using output_type = trunc_sqrt_result<FixedPoint>;
        return output_type(sqrt(square));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_shift_left

    template<int Integer, class ReprType, int Exponent>
    constexpr fixed_point<ReprType, Exponent+Integer>
    trunc_shift_left(const fixed_point<ReprType, Exponent>& fp)
    {
        return fixed_point<ReprType, Exponent+Integer>::from_data(fp.data());
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trunc_shift_right

    template<int Integer, class ReprType, int Exponent>
    constexpr fixed_point<ReprType, Exponent-Integer>
    trunc_shift_right(const fixed_point<ReprType, Exponent>& fp)
    {
        return fixed_point<ReprType, Exponent-Integer>::from_data(fp.data());
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::promote_add_result / promote_add

    // yields specialization of fixed_point with double the capacity necessary to
    // store result of an add between values of fixed_point<ReprType, Exponent>
    template<class FixedPoint, unsigned N = 2>
    using promote_add_result = make_fixed_from_repr<
            _impl::next_size<typename FixedPoint::repr_type>,
            FixedPoint::integer_digits+_impl::capacity<N-1>::value>;

    template<class FixedPoint, class ... Tail>
    promote_add_result<FixedPoint, sizeof...(Tail)+1>
    constexpr promote_add(const FixedPoint& addend1, const Tail& ... addend_tail)
    {
        using output_type = promote_add_result<FixedPoint, sizeof...(Tail)+1>;
        return _impl::add<output_type, FixedPoint>(addend1, addend_tail ...);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::promote_subtract_result / promote_subtract

    // yields specialization of fixed_point with double the capacity necessary to
    // store result of an subtract between values of fixed_point<ReprType, Exponent>
    template<class Lhs, class Rhs = Lhs>
    using promote_subtract_result = make_fixed_from_repr<
            typename _impl::next_size<_impl::subtract_result_repr<typename Lhs::repr_type, typename Rhs::repr_type>>,
            _impl::max(Lhs::integer_digits, Rhs::integer_digits)+1>;

    // as promote_subtract_result but converts parameter, factor,
    // ready for safe binary subtract
    template<class Lhs, class Rhs>
    promote_subtract_result<Lhs, Rhs>
    constexpr promote_subtract(const Lhs& lhs, const Rhs& rhs)
    {
        using result_type = promote_subtract_result<Lhs, Rhs>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::promote_multiply_result / promote_multiply

    // yields specialization of fixed_point with capacity necessary to store
    // result of a multiply between values of fixed_point<ReprType, Exponent>
    template<class Lhs, class Rhs = Lhs>
    using promote_multiply_result = promote_result<_impl::common_type<Lhs, Rhs>>;

    // as promote_multiply_result but converts parameter, factor,
    // ready for safe binary multiply
    template<class Lhs, class Rhs>
    promote_multiply_result<Lhs, Rhs>
    constexpr promote_multiply(const Lhs& lhs, const Rhs& rhs)
    {
        using result_type = promote_multiply_result<Lhs, Rhs>;
        return _impl::multiply<result_type>(lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::promote_divide_result / promote_divide

    // yields specialization of fixed_point with capacity necessary to store
    // result of a divide between values of fixed_point<ReprType, Exponent>
    template<class Lhs, class Rhs = Lhs>
    using promote_divide_result = promote_result<_impl::common_type<Lhs, Rhs>>;

    // as promote_divide_result but converts parameter, factor,
    // ready for safe binary divide
    template<class Lhs, class Rhs>
    promote_divide_result<Lhs, Rhs>
    constexpr promote_divide(const Lhs& lhs, const Rhs& rhs)
    {
        using result_type = promote_divide_result<Lhs, Rhs>;
        return _impl::divide<result_type>(lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::promote_square_result / promote_square

    // yields specialization of fixed_point with integral bits necessary to store
    // result of a multiply between values of fixed_point<ReprType, Exponent>
    // whose sign bit is set to the same value
    template<class FixedPoint>
    using promote_square_result = make_ufixed<
            FixedPoint::integer_digits*2,
            FixedPoint::fractional_digits*2>;

    // as promote_square_result but converts parameter, factor,
    // ready for safe binary multiply-by-self
    template<class FixedPoint>
    promote_square_result<FixedPoint>
    constexpr promote_square(const FixedPoint& root)
    {
        using output_type = promote_square_result<FixedPoint>;
        using output_repr_type = typename output_type::repr_type;
        return output_type::from_data(
                _impl::shift_left<(FixedPoint::exponent*2-output_type::exponent), output_repr_type>(
                        static_cast<output_repr_type>(root.data())*static_cast<output_repr_type>(root.data())));
    }
}

#endif	// defined(_SG14_FIXED_POINT)
