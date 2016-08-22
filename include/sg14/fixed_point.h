
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_H)
#define SG14_FIXED_POINT_H 1

#include <limits>

#include "type_traits.h"

#include "bits/common.h"

////////////////////////////////////////////////////////////////////////////////
// SG14_FIXED_POINT_EXCEPTIONS_ENABLED macro definition 

#if defined(SG14_FIXED_POINT_EXCEPTIONS_ENABLED)
#error SG14_FIXED_POINT_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define SG14_FIXED_POINT_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define SG14_FIXED_POINT_EXCEPTIONS_ENABLED
#endif
#else
#define SG14_FIXED_POINT_EXCEPTIONS_ENABLED
#endif

#if defined(SG14_FIXED_POINT_EXCEPTIONS_ENABLED)

#include <stdexcept>

#endif

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // general-purpose _fixed_point_impl definitions

    namespace _fixed_point_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::digits

        template<class T>
        using digits = std::integral_constant<int, width<T>::value-is_signed<T>::value>;

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
        // sg14::_fixed_point_impl::pow2

        // returns given power of 2
        template<class S, int Exponent, typename std::enable_if<Exponent==0, int>::type Dummy = 0>
        constexpr S pow2()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return 1;
        }

        template<class S, int Exponent, typename std::enable_if<!(Exponent<=0) && (Exponent<8), int>::type Dummy = 0>
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

        template<class S, int Exponent, typename std::enable_if<!(Exponent>=0) && (Exponent>-8), int>::type Dummy = 0>
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
        using rep = Rep;

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// number of binary digits this type can represent;
        /// equivalent to [std::numeric_limits::digits](http://en.cppreference.com/w/cpp/types/numeric_limits/digits)
        constexpr static int digits = _fixed_point_impl::digits<Rep>::value;

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

        /// constructor taking an integer type
        template<class S, typename std::enable_if<std::numeric_limits<S>::is_integer, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_r(integral_to_rep(s))
        {
        }

        /// constructor taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        explicit constexpr fixed_point(S s)
                :_r(floating_point_to_rep(s))
        {
        }

        /// constructor taking a fixed-point type
        template<class FromRep, int FromExponent>
        explicit constexpr fixed_point(const fixed_point<FromRep, FromExponent>& rhs)
                :_r(fixed_point_to_rep(rhs))
        {
        }

        /// copy assignment operator taking an integer type
        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _r = integral_to_rep(s);
            return *this;
        }

        /// copy assignment operator taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        fixed_point& operator=(S s)
        {
            _r = floating_point_to_rep(s);
            return *this;
        }

        /// copy assignement operator taking a fixed-point type
        template<class FromRep, int FromExponent>
        fixed_point& operator=(const fixed_point<FromRep, FromExponent>& rhs)
        {
            _r = fixed_point_to_rep(rhs);
            return *this;
        }

        /// returns value represented as integral
        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        explicit constexpr operator S() const
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
        static constexpr S one()
        {
            return _fixed_point_impl::pow2<S, -exponent>();
        }

        template<class S, typename std::enable_if<is_integral<S>::value, int>::type Dummy = 0>
        static constexpr S one()
        {
            return integral_to_rep<S>(1);
        }

        template<class S>
        static constexpr S inverse_one()
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return _fixed_point_impl::pow2<S, exponent>();
        }

        template<class S>
        static constexpr rep integral_to_rep(S s)
        {
            static_assert(std::numeric_limits<S>::is_integer, "S must be unsigned integral type");

            return _fixed_point_impl::shift_right<exponent, rep>(s);
        }

        template<class S>
        static constexpr S rep_to_integral(rep r)
        {
            static_assert(is_integral<S>::value, "S must be unsigned integral type");

            return _fixed_point_impl::shift_left<exponent, S>(r);
        }

        template<class S>
        static constexpr rep floating_point_to_rep(S s)
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return static_cast<rep>(s*one<S>());
        }

        template<class S>
        static constexpr S rep_to_floating_point(rep r)
        {
            static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
            return S(r)*inverse_one<S>();
        }

        template<class FromRep, int FromExponent>
        static constexpr rep fixed_point_to_rep(const fixed_point<FromRep, FromExponent>& rhs)
        {
            return _fixed_point_impl::shift_right<(exponent-FromExponent), rep>(rhs.data());
        }

        ////////////////////////////////////////////////////////////////////////////////
        // variables

        rep _r;
    };

    /// \brief Produce a fixed-point type with the given number of integer and fractional digits.
    ///
    /// \tparam IntegerDigits specifies minimum value of @ref fixed_point::integer_digits
    /// \tparam FractionalDigits specifies the exact value of @ref fixed_point::fractional_digits
    /// \tparam Archetype hints at the type of @ref fixed_point::rep
    ///
    /// \remarks The signage of \a Archetype specifies signage of the resultant fixed-point type.
    /// \remarks Typical choices for \a Archetype, `signed` and `unsigned`,
    /// result in a type that uses built-in integers for \a fixed_point::rep.
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
        // sg14::_fixed_point_impl::common_type_mixed

        template<class Lhs, class Rhs, class _Enable = void>
        struct common_type_mixed;

        // given a fixed-point and an integer type,
        // generates a fixed-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, int LhsExponent, class RhsInteger>
        struct common_type_mixed<
                fixed_point<LhsRep, LhsExponent>,
                RhsInteger,
                typename std::enable_if<is_integral<RhsInteger>::value>::type>
                : std::common_type<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsInteger, 0>> {
        };

        // given a fixed-point and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, int LhsExponent, class Float>
        struct common_type_mixed<
                fixed_point<LhsRep, LhsExponent>,
                Float,
                typename std::enable_if<std::is_floating_point<Float>::value>::type>
                : std::common_type<float_of_same_size<LhsRep>, Float> {
        };
    }
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::common_type<> specializations related to sg14::sg14::fixed_point<>

    // std::common_type<fixed_point<>>
    template<class Rep, int Exponent>
    struct common_type<sg14::fixed_point<Rep, Exponent>> {
        using type = sg14::fixed_point<
                typename std::common_type<Rep>::type,
                Exponent>;
    };

    // std::common_type<fixed_point<>, not-fixed-point>
    template<class LhsRep, int LhsExponent, class Rhs>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, Rhs> {
        static_assert(!sg14::_fixed_point_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");
        using type = typename sg14::_fixed_point_impl::common_type_mixed<sg14::fixed_point<LhsRep, LhsExponent>, Rhs>::type;
    };

    // std::common_type<not-fixed-point, fixed_point<>>
    template<class Lhs, class RhsRep, int RhsExponent>
    struct common_type<Lhs, sg14::fixed_point<RhsRep, RhsExponent>> {
        static_assert(!sg14::_fixed_point_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");
        using type = typename sg14::_fixed_point_impl::common_type_mixed<sg14::fixed_point<RhsRep, RhsExponent>, Lhs>::type;
    };

    // std::common_type<fixed_point<>, fixed_point<>>
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, sg14::fixed_point<RhsRep, RhsExponent>> {
        using _result_rep = typename std::common_type<LhsRep, RhsRep>::type;

        // exponent is the lower of the two operands' unless that could cause overflow in which case it is adjusted downward
        static constexpr int _capacity = sg14::_fixed_point_impl::digits<_result_rep>::value;
        static constexpr int _ideal_max_top = sg14::_impl::max(
                sg14::fixed_point<LhsRep, LhsExponent>::integer_digits,
                sg14::fixed_point<RhsRep, RhsExponent>::integer_digits);
        static constexpr int _ideal_exponent = sg14::_impl::min(LhsExponent, RhsExponent);
        static constexpr int _exponent = ((_ideal_max_top-_ideal_exponent)<=_capacity) ? _ideal_exponent :
                                         _ideal_max_top-_capacity;

        using type = sg14::fixed_point<_result_rep, _exponent>;
    };
}

namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // named fixed-point arithmetic traits

    namespace _fixed_point_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // binary_pair

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_pair_base {
            using lhs_type = fixed_point<LhsRep, LhsExponent>;
            using rhs_type = fixed_point<RhsRep, RhsExponent>;
        };

        template<class Lhs, class Rhs>
        struct binary_pair;

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_pair<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>
                : binary_pair_base<LhsRep, LhsExponent, RhsRep, RhsExponent> {
        };

        template<class LhsRep, int LhsExponent, class Rhs>
        struct binary_pair<fixed_point<LhsRep, LhsExponent>, Rhs> : binary_pair_base<LhsRep, LhsExponent, Rhs, 0> {
            static_assert(sg14::is_integral<Rhs>::value,
                    "named arithmetic functions take only fixed_point and integral types");
        };

        template<class Lhs, class RhsRep, int RhsExponent>
        struct binary_pair<Lhs, fixed_point<RhsRep, RhsExponent>> : binary_pair_base<Lhs, 0, RhsRep, RhsExponent> {
            static_assert(sg14::is_integral<Lhs>::value,
                    "named arithmetic functions take only fixed_point and integral types");
        };

        ////////////////////////////////////////////////////////////////////////////////
        // tags

        // strategy
        struct lean_tag;    // like-for-like interger arithmetic
        struct wide_tag;    // effort is made to widen to accommodate results of multiplication and division

        // strategy aliases - for ease of flip-flopping
        using named_function_tag = lean_tag;
        using arithmetic_operator_tag = wide_tag;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::rep_op_exponent

        template<class OperationTag, class ... Operands>
        struct rep_op_exponent;

        template<class Rhs>
        struct rep_op_exponent<_impl::negate_tag, Rhs> : public std::integral_constant<int, Rhs::exponent> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::add_tag, Lhs, Rhs> : public std::integral_constant<int, _impl::min<int>(
                Lhs::exponent,
                Rhs::exponent)> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::subtract_tag, Lhs, Rhs> : public std::integral_constant<int, _impl::min<int>(
                Lhs::exponent,
                Rhs::exponent)> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::multiply_tag, Lhs, Rhs> : public std::integral_constant<int,
                Lhs::exponent+Rhs::exponent> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::divide_tag, Lhs, Rhs> : public std::integral_constant<int,
                Lhs::exponent-Rhs::exponent> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::result

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct result;

        // result<lean_tag>
        template<class OperationTag, class Lhs, class Rhs>
        struct result<lean_tag, OperationTag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            static constexpr int exponent = rep_op_exponent<OperationTag, Lhs, Rhs>::value;

            using type = fixed_point<rep_op_result, exponent>;
        };

        // result<wide_tag>
        template<class OperationTag, class Lhs, class Rhs>
        struct result<wide_tag, OperationTag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            // 'Wide' doesn't guarantee avoiding overflow. Adding a single bit to add/subtract results would often lead to double the width being used.
            static constexpr int sufficient_sign_bits = std::is_signed<rep_op_result>::value;
            static constexpr int sufficient_integer_digits = _impl::max(Lhs::integer_digits,
                    Rhs::integer_digits);
            static constexpr int sufficient_fractional_digits = _impl::max(Lhs::fractional_digits,
                    Rhs::fractional_digits);
            static constexpr _width_type sufficient_width =
                    sufficient_sign_bits+sufficient_integer_digits+sufficient_fractional_digits;
            static constexpr int result_width = _impl::max(sufficient_width, width<rep_op_result>::value);

            using rep_type = set_width_t<rep_op_result, result_width>;
            using type = fixed_point<rep_type, -sufficient_fractional_digits>;
        };
        template<class Lhs, class Rhs>
        struct result<wide_tag, _impl::multiply_tag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<_impl::multiply_tag, lhs_rep, rhs_rep>;

            static constexpr int digits = Lhs::digits+Rhs::digits;
            static constexpr bool is_signed =
                    std::numeric_limits<lhs_rep>::is_signed || std::numeric_limits<rhs_rep>::is_signed;
            static constexpr int width = digits+is_signed;

            using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
            using rep_type = set_width_t<prewidened_result_rep, width>;

            static constexpr int rep_exponent = rep_op_exponent<_impl::multiply_tag, Lhs, Rhs>::value;

            using type = fixed_point<rep_type, rep_exponent>;
        };
        template<class Lhs, class Rhs>
        struct result<wide_tag, _impl::divide_tag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<_impl::multiply_tag, lhs_rep, rhs_rep>;

            static constexpr int integer_digits = Lhs::integer_digits+Rhs::fractional_digits;
            static constexpr int fractional_digits = Lhs::fractional_digits+Rhs::integer_digits;
            static constexpr int digits = integer_digits+fractional_digits;
            static constexpr bool is_signed =
                    std::numeric_limits<lhs_rep>::is_signed || std::numeric_limits<rhs_rep>::is_signed;

            static constexpr int necessary_width = digits+is_signed;
            static constexpr int promotion_width = width<rep_op_result>::value;
            static constexpr int width = _impl::max(necessary_width, promotion_width);

            using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
            using rep_type = set_width_t<prewidened_result_rep, width>;

            static constexpr int rep_exponent = -fractional_digits;

            using type = fixed_point<rep_type, rep_exponent>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::intermediate

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct intermediate;

        // bare add / subtract

        template<class OperationTag, class Lhs, class Rhs>
        struct intermediate<lean_tag, OperationTag, Lhs, Rhs> {
            using _result = result<lean_tag, OperationTag, Lhs, Rhs>;
            using lhs_type = typename _result::type;
            using rhs_type = lhs_type;
        };

        template<class Lhs, class Rhs>
        struct intermediate<lean_tag, _impl::multiply_tag, Lhs, Rhs> {
            using lhs_type = Lhs;
            using rhs_type = Rhs;
        };

        template<class Lhs, class Rhs>
        struct intermediate<lean_tag, _impl::divide_tag, Lhs, Rhs> {
            using lhs_type = Lhs;
            using rhs_type = Rhs;
        };

        // intermediate - wide

        // wide - add/subtract
        template<class OperationTag, class Lhs, class Rhs>
        struct intermediate<wide_tag, OperationTag, Lhs, Rhs> {
            using _result = result<wide_tag, OperationTag, Lhs, Rhs>;
            using lhs_type = typename _result::type;
            using rhs_type = lhs_type;
        };

        template<class Lhs, class Rhs>
        struct intermediate<wide_tag, _impl::multiply_tag, Lhs, Rhs> {
            using _result = result<wide_tag, _impl::multiply_tag, Lhs, Rhs>;
            using result_rep = typename _result::rep_type;
            using prewidened_result_rep = typename _result::prewidened_result_rep;

            // If the 'natural' result of the rep op is wide enough, stick with it.
            // This ensures that auto-widening rep types (e.g. elastic_integer) don't get widened twice
            // but types that need a little help (e.g. built-ins) get widened going into the op.
            using rep_type = typename std::conditional<
                    width<prewidened_result_rep>::value>=_result::width,
                    typename Lhs::rep, result_rep>::type;

            using lhs_type = fixed_point<rep_type, Lhs::exponent>;
            using rhs_type = Rhs;
        };

        template<class Lhs, class Rhs>
        struct intermediate<wide_tag, _impl::divide_tag, Lhs, Rhs> {
            using wide_result = result<wide_tag, _impl::divide_tag, Lhs, Rhs>;
            using rep_type = typename wide_result::rep_type;

            static constexpr int exponent = Lhs::exponent-Rhs::digits;

            using lhs_type = fixed_point<rep_type, exponent>;
            using rhs_type = Rhs;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::operate_params

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct operate_params;

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct operate_params {
            using _binary_pair = binary_pair<Lhs, Rhs>;
            using lhs_type = typename _binary_pair::lhs_type;
            using rhs_type = typename _binary_pair::rhs_type;

            using _intermediate = intermediate<PolicyTag, OperationTag, lhs_type, rhs_type>;
            using intermediate_lhs = typename _intermediate::lhs_type;
            using intermediate_rhs = typename _intermediate::rhs_type;

            using _result = result<PolicyTag, OperationTag, lhs_type, rhs_type>;
            using result_type = typename _result::type;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::operate

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        constexpr auto operate(const Lhs& lhs, const Rhs& rhs)
        -> typename operate_params<PolicyTag, OperationTag, Lhs, Rhs>::result_type
        {
            using params = operate_params<PolicyTag, OperationTag, Lhs, Rhs>;
            using intermediate_lhs = typename params::intermediate_lhs;
            using intermediate_rhs = typename params::intermediate_rhs;
            using result_type = typename params::result_type;
            using result_rep = typename result_type::rep;

            return result_type::from_data(
                    static_cast<result_rep>(
                            _impl::op_fn<OperationTag>(
                                    static_cast<intermediate_lhs>(lhs).data(),
                                    static_cast<intermediate_rhs>(rhs).data())));
        };
    }

    /// \brief calculates the negative of a \ref fixed_point value
    ///
    /// \param rhs input value
    ///
    /// \return negative: - rhs
    ///
    /// \note This function negates the value
    /// without performing any additional scaling or conversion.
    ///
    /// \sa add, subtract, multiply, divide

    template<class RhsRep, int RhsExponent>
    constexpr auto negate(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }

    /// \brief calculates the sum of two \ref fixed_point values
    ///
    /// \param lhs, rhs augend and addend
    ///
    /// \return sum: lhs + rhs
    ///
    /// \note This function add the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, subtract, multiply, divide

    template<class Lhs, class Rhs>
    constexpr auto add(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::add_tag>(lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::add_tag>(lhs, rhs);
    }

    /// \brief calculates the difference of two \ref fixed_point values
    ///
    /// \param lhs, rhs minuend and subtrahend
    ///
    /// \return difference: lhs - rhs
    ///
    /// \note This function subtracts the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, multiply, divide

    template<class Lhs, class Rhs>
    constexpr auto subtract(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::subtract_tag>(lhs,
            rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::subtract_tag>(lhs,
                rhs);
    }

    /// \brief calculates the product of two \ref fixed_point factors
    ///
    /// \param lhs, rhs the factors
    ///
    /// \return product: lhs * rhs
    ///
    /// \note This function multiplies the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, divide

    template<class Lhs, class Rhs>
    constexpr auto multiply(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::multiply_tag>(lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::multiply_tag>(lhs, rhs);
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    ///
    /// \param lhs, rhs dividend and divisor
    ///
    /// \return quotient: lhs / rhs
    ///
    /// \note This function divides the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, multiply

    template<class Lhs, class Rhs>
    constexpr auto divide(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::divide_tag>(lhs,
            rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::divide_tag>(lhs,
                rhs);
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
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }

    // add
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator+(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::add_tag>(lhs,
            rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::add_tag>(lhs,
                rhs);
    }

    // subtract
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator-(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::subtract_tag>(
            lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::subtract_tag>(
                lhs, rhs);
    }

    // multiply
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::multiply_tag>(
            lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::multiply_tag>(
                lhs, rhs);
    }

    // divide
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::divide_tag>(
            lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::divide_tag>(
                lhs, rhs);
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
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)==static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)!=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<=static_cast<common_type>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ non-fixed_point) arithmetic operators

    // fixed-point, integer -> fixed-point
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs + fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs + fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs - fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs - fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator*(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs*fixed_point<RhsInteger>(rhs))
    {
        return lhs*fixed_point<RhsInteger>(rhs);
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator/(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs/fixed_point<RhsInteger>{rhs})
    {
        return lhs/fixed_point<RhsInteger>{rhs};
    }

    // integer. fixed-point -> fixed-point
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator+(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger, 0>{lhs} + rhs)
    {
        return fixed_point<LhsInteger, 0>{lhs} + rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator-(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}-rhs)
    {
        return fixed_point<LhsInteger>{lhs}-rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator*(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}*rhs)
    {
        return fixed_point<LhsInteger>{lhs}*rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator/(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}/rhs)
    {
        return fixed_point<LhsInteger>{lhs}/rhs;
    }

    // fixed-point, floating-point -> floating-point
    template<class LhsRep, int LhsExponent, class RhsFloat, typename = typename std::enable_if<std::is_floating_point<RhsFloat>::value>::type>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = typename std::enable_if<std::is_floating_point<RhsFloat>::value>::type>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<RhsFloat>::value, RhsFloat>::type>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)*rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<RhsFloat>::value, RhsFloat>::type>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)/rhs;
    }

    // floating-point, fixed-point -> floating-point
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = typename std::enable_if<std::is_floating_point<LhsFloat>::value>::type>
    constexpr auto operator+(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = typename std::enable_if<std::is_floating_point<LhsFloat>::value>::type>
    constexpr auto operator-(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t<
            typename std::enable_if<std::is_floating_point<LhsFloat>::value, LhsFloat>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs*static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t<
            typename std::enable_if<std::is_floating_point<LhsFloat>::value, LhsFloat>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
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
    template<class Rhs>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator*=(const Rhs& rhs)
    {
        _r *= static_cast<rep>(rhs);
        return *this;
    }

    template<class LhsRep, int Exponent>
    template<class Rhs>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator/=(const Rhs& rhs)
    {
        _r /= static_cast<rep>(rhs);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr fixed_point<LhsRep, LhsExponent>
    operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    {
        return fixed_point<LhsRep, LhsExponent>::from_data(lhs.data() << rhs);
    };

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr fixed_point<LhsRep, LhsExponent>
    operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    {
        return fixed_point<LhsRep, LhsExponent>::from_data(lhs.data() >> rhs);
    };
}

#include "bits/fixed_point_extras.h"

#endif	// SG14_FIXED_POINT_H
