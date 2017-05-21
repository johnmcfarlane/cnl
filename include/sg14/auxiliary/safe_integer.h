
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::safe_integer` type

#if !defined(SG14_SAFE_INTEGER_H)
#define SG14_SAFE_INTEGER_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include <sg14/bits/number_base.h>
#include <sg14/fixed_point>
#include <sg14/numeric_traits>
#include "overflow.h"
#endif

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // macros

#define SG14_INTEGER_BIT_SHIFT_DEFINE(OP) \
    template <class LhsRep, class LhsOverflowTag, class RhsRep, class RhsOverflowTag> \
    constexpr auto operator OP (const safe_integer<LhsRep, LhsOverflowTag>& lhs, const safe_integer<RhsRep, RhsOverflowTag>& rhs) \
    -> safe_integer<LhsRep, LhsOverflowTag> { \
        return lhs.data() OP rhs.data(); } \
    \
    template <class Lhs, class RhsRep, class RhsOverflowTag, _impl::enable_if_t<std::is_fundamental<Lhs>::value, int> dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const safe_integer<RhsRep, RhsOverflowTag>& rhs) \
    -> Lhs { \
        return lhs OP rhs.data(); } \
    \
    template <class LhsRep, class LhsOverflowTag, class Rhs, _impl::enable_if_t<std::is_fundamental<Rhs>::value, int> dummy = 0> \
    constexpr auto operator OP (const safe_integer<LhsRep, LhsOverflowTag>& lhs, const Rhs& rhs) \
    -> safe_integer<LhsRep, LhsOverflowTag> { \
        return safe_integer<LhsRep, LhsOverflowTag>(lhs.data() OP rhs); }

    ////////////////////////////////////////////////////////////////////////////////
    // forward-declarations

    template<class Rep, class OverflowTag>
    class safe_integer;

    namespace _integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::is_safe_integer - trait to identify sg14::safe_integer<>

        template<class T>
        struct is_safe_integer
                : std::false_type {
        };

        template<class Rep, class OverflowTag>
        struct is_safe_integer<safe_integer<Rep, OverflowTag>>
                : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::are_integer_class_operands - basically identifies
        // operands that should go into a function defined here; filters out fixed-point

        template<class Lhs, class Rhs>
        struct are_integer_class_operands {
            static constexpr int integer_class = is_safe_integer<Lhs>::value + is_safe_integer<Rhs>::value;
            static constexpr int integer_or_float = _impl::is_integer_or_float<Lhs>::value + _impl::is_integer_or_float<Rhs>::value;
            static constexpr bool value = (integer_class >= 1) && (integer_or_float == 2);
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::common_type

        template<class, class, class = void>
        struct common_type;

        // given two safe_integer<>, produces the type that is best suited to both of them
        template<class LhsRep, class RhsRep, class OverflowTag>
        struct common_type<
                safe_integer<LhsRep, OverflowTag>,
                safe_integer<RhsRep, OverflowTag>> {
            using type = safe_integer<
                    typename std::common_type<LhsRep, RhsRep>::type,
                    OverflowTag>;
        };

        // given a safe_integer<> and a built-in integer type,
        // generates a safe_integer<> type that is as big as both of them (or as close as possible)
        template<class LhsRep, class LhsOverflowTag, class RhsInteger>
        struct common_type<
                safe_integer<LhsRep, LhsOverflowTag>, RhsInteger,
                _impl::enable_if_t<
                        !_integer_impl::is_safe_integer<RhsInteger>::value && std::is_integral<RhsInteger>::value>> {
            using type = typename sg14::safe_integer<typename std::common_type<LhsRep, RhsInteger>::type, LhsOverflowTag>;
        };

        // given a safe_integer<> and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, class LhsOverflowTag, class Float>
        struct common_type<
                safe_integer<LhsRep, LhsOverflowTag>, Float,
                _impl::enable_if_t<std::is_floating_point<Float>::value>> {
            using type = typename std::common_type<LhsRep, Float>::type;
        };

        // when first type is not safe_integer<> and second type is, reverse the order
        template<class Lhs, class RhsRep, class RhsOverflowTag>
        struct common_type<Lhs, safe_integer<RhsRep, RhsOverflowTag>>
                : common_type<safe_integer<RhsRep, RhsOverflowTag>, Lhs> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::safe_integer<>

    // an integer which can be customized to react in different ways to overflow;
    // currently doesn't correctly detect overflow from operators
    template<class Rep = int, class OverflowTag = throwing_overflow_tag>
    class safe_integer : public _impl::number_base<safe_integer<Rep, OverflowTag>, Rep> {
        using _base = _impl::number_base<safe_integer<Rep, OverflowTag>, Rep>;
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        using rep = Rep;
        using overflow_tag = OverflowTag;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

        constexpr safe_integer() = delete;

        constexpr safe_integer(const safe_integer& rhs)
                :_base(rhs.data())
        {
        }

        template<class Rhs, _impl::enable_if_t<!_integer_impl::is_safe_integer<Rhs>::value, int> dummy = 0>
        constexpr safe_integer(const Rhs& rhs)
                :_base(convert<rep>(overflow_tag{}, rhs))
        {
        }

        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(_base::data());
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::numeric_traits<safe_integer<>>
    template<class Rep, class OverflowTag>
    struct numeric_traits<safe_integer<Rep, OverflowTag>>
    : numeric_traits<_impl::number_base<safe_integer<Rep, OverflowTag>, Rep>> {
        using value_type = safe_integer<Rep, OverflowTag>;
        
        template<typename Input>
        static constexpr auto make(const Input& input) 
        -> safe_integer<Input, OverflowTag> {
            return input;
        }

    private:
        // TODO: dupe of code from specialization in <numeric_traits>
        using _base = numeric_traits<_impl::number_base<safe_integer<Rep, OverflowTag>, Rep>>;
        using _rep = typename _base::_rep;
        using result_type = decltype(make(sg14::scale(std::declval<_rep>(), 0, 0)));

        static constexpr result_type pown(int base, int exp)
        {
            return exp
                   ? pown(base, exp-1)*static_cast<result_type>(base)
                   : static_cast<result_type>(1);
        }

        static constexpr result_type pow2(int exp)
        {
            return result_type{1} << exp;
        }

        static constexpr result_type pow(int base, int exp)
        {
            return (base==2) ? pow2(exp) : pown(base, exp);
        }

    public:
        static constexpr result_type scale(const value_type& i, int base, int exp)
        {
            return (exp<0)
                    ? to_rep(i)/pow(base, -exp)
                    : to_rep(i)*pow(base, exp);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_safe_integer

    template<class OverflowTag, class Rep>
    constexpr auto make_safe_integer(Rep const& value)
    -> safe_integer<Rep, OverflowTag>
    {
        return value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary arithmetic

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // arithmetc

        // for arithmetic operands with a common overflow policy
        template<class OverflowTag, class OperatorTag, class LhsRep, class RhsRep, class = enable_if_t<OperatorTag::is_arithmetic>>
        constexpr auto operate_common_policy(
                OverflowTag,
                OperatorTag,
                const safe_integer<LhsRep, OverflowTag>& lhs,
                const safe_integer<RhsRep, OverflowTag>& rhs)
        -> decltype(make_safe_integer<OverflowTag>(_overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data())))
        {
            return make_safe_integer<OverflowTag>(_overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data()));
        }

        // for comparison operands with a common overflow policy
        template<class OverflowTag, class OperatorTag, class LhsRep, class RhsRep, class = enable_if_t<OperatorTag::is_comparison>>
        constexpr auto operate_common_policy(
                OverflowTag,
                OperatorTag,
                const safe_integer<LhsRep, OverflowTag>& lhs,
                const safe_integer<RhsRep, OverflowTag>& rhs)
        -> decltype(_overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data()))
        {
            return _overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data());
        }
    
        // for arithmetic operands with different policies
        template<class OperatorTag, class LhsRep, class LhsPolicy, class RhsRep, class RhsPolicy>
        constexpr auto operate(
                const safe_integer<LhsRep, LhsPolicy>& lhs,
                const safe_integer<RhsRep, RhsPolicy>& rhs,
                OperatorTag operator_tag)
        -> decltype(operate_common_policy(common_type_t<LhsPolicy, RhsPolicy>{}, operator_tag, lhs, rhs))
        {
            return operate_common_policy(common_type_t<LhsPolicy, RhsPolicy>{}, operator_tag, lhs, rhs);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary bit-wise

    SG14_INTEGER_BIT_SHIFT_DEFINE(>>);

    SG14_INTEGER_BIT_SHIFT_DEFINE(<<);

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::set_width<safe_integer<>, > partial specialization

    template<class Rep, class OverflowTag, _width_type MinNumBits>
    struct set_width<safe_integer<Rep, OverflowTag>, MinNumBits> {
        using type = safe_integer<set_width_t<Rep, MinNumBits>, OverflowTag>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width<safe_integer<>> partial specialization

    template<class Rep, class OverflowTag>
    struct width<safe_integer<Rep, OverflowTag>> : width<Rep> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::safe_integer-specific specializations to std-like templates

    // sg14::make_unsigned<sg14::safe_integer<>>
    template<class Rep, class OverflowTag>
    struct make_unsigned<safe_integer<Rep, OverflowTag>> {
        using type = safe_integer<typename make_unsigned<Rep>::type, OverflowTag>;
    };

    // sg14::make_signed<sg14::safe_integer<>>
    template<class Rep, class OverflowTag>
    struct make_signed<safe_integer<Rep, OverflowTag>> {
        using type = safe_integer<typename make_signed<Rep>::type, OverflowTag>;
    };
}

namespace std {
    // std::common_type<T, sg14::safe_integer>
    template<
            class Lhs,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            Lhs,
            sg14::safe_integer<RhsRep, RhsOverflowTag>>
            : sg14::_integer_impl::common_type<
                    Lhs,
                    sg14::safe_integer<RhsRep, RhsOverflowTag>> {
    };

    // std::common_type<sg14::safe_integer, T>
    template<
            class LhsRep, class LhsOverflowTag,
            class Rhs>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowTag>,
            Rhs>
            : sg14::_integer_impl::common_type<
                    sg14::safe_integer<LhsRep, LhsOverflowTag>,
                    Rhs> {
    };

    // std::common_type<sg14::safe_integer, sg14::fixed_point>
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, int RhsExponent>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowTag>,
            sg14::fixed_point<RhsRep, RhsExponent>>
            : std::common_type<
                    sg14::fixed_point<sg14::safe_integer<LhsRep, LhsOverflowTag>, 0>,
                    sg14::fixed_point<RhsRep, RhsExponent>> {
    };

    // std::common_type<sg14::fixed_point, sg14::safe_integer>
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            sg14::fixed_point<LhsRep, LhsExponent>,
            sg14::safe_integer<RhsRep, RhsOverflowTag>>
            : std::common_type<
                    sg14::fixed_point<LhsRep, LhsExponent>,
                    sg14::fixed_point<sg14::safe_integer<RhsRep, RhsOverflowTag>, 0>> {
    };
    
    // std::common_type<sg14::safe_integer, sg14::safe_integer>
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowTag>,
            sg14::safe_integer<RhsRep, RhsOverflowTag>>
            : sg14::_integer_impl::common_type<
                    sg14::safe_integer<LhsRep, LhsOverflowTag>,
                    sg14::safe_integer<RhsRep, RhsOverflowTag>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for safe_integer

    template<class Rep, class OverflowTag>
    struct numeric_limits<sg14::safe_integer<Rep, OverflowTag>>
    : numeric_limits<sg14::_impl::number_base<sg14::safe_integer<Rep, OverflowTag>, Rep>> {};
}

#endif	// SG14_SAFE_INTEGER_H
