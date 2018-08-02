
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to overflow detection and handling

#if !defined(CNL_OVERFLOW_H)
#define CNL_OVERFLOW_H

#include "_impl/terminate.h"
#include "_impl/throw_exception.h"

#include "numeric.h"
#include "_impl/native_tag.h"

#include <stdexcept>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // mode tags and objects

    // match the behavior of fundamental arithmetic types
    struct native_overflow_tag : _impl::native_tag {
    };

    static constexpr native_overflow_tag native_overflow{};

    // terminate program with diagnostic when overflow is detected
    static constexpr struct trapping_overflow_tag {
    } trapping_overflow{};

    // throw an exception when overflow is don overflowetected
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};

    // confine range of results
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // things to return when things go south (or north)

        template<class Result>
        constexpr Result positive_overflow_result(trapping_overflow_tag)
        {
            return terminate<Result>("positive overflow");
        }

        template<class Result>
        constexpr Result positive_overflow_result(throwing_overflow_tag)
        {
            return throw_exception<Result, std::overflow_error>("positive overflow");
        }

        template<class Result>
        constexpr Result positive_overflow_result(saturated_overflow_tag)
        {
            return numeric_limits<Result>::max();
        }

        template<class Result>
        constexpr Result negative_overflow_result(trapping_overflow_tag)
        {
            return terminate<Result>("negative overflow");
        }

        template<class Result>
        constexpr Result negative_overflow_result(throwing_overflow_tag)
        {
            return throw_exception<Result, std::overflow_error>("negative overflow");
        }

        template<class Result>
        constexpr Result negative_overflow_result(saturated_overflow_tag)
        {
            return numeric_limits<Result>::lowest();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<class OverflowTag, class Operator>
        struct comparison_operator : public CNL_ERROR___cannot_use<OverflowTag>::as_a_tag {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // overflow detection

        template<class T>
        struct positive_digits : public std::integral_constant<int, numeric_limits<T>::digits> {
        };

        template<class T>
        struct negative_digits
                : public std::integral_constant<int, cnl::is_signed<T>::value ? digits<T>::value : 0> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // conversion overflow

    namespace _impl {
        template<typename Destination, typename Source>
        struct convert_test {
            static constexpr bool positive(Source const& rhs)
            {
                return _impl::positive_digits<Destination>::value
                        <_impl::positive_digits<Source>::value
                        && rhs>static_cast<Source>(numeric_limits<Destination>::max());
            }

            static constexpr bool negative(Source const& rhs)
            {
                return _impl::negative_digits<Destination>::value
                        <_impl::negative_digits<Source>::value
                        && rhs<static_cast<Source>(numeric_limits<Destination>::lowest());
            }
        };

        template<class OverflowTag, typename Result>
        struct overflow_convert {
            template<typename Input>
            constexpr Result operator()(Input const& rhs) const
            {
                using test = convert_test<Result, Input>;
                return test::positive(rhs)
                        ? positive_overflow_result<Result>(OverflowTag{})
                        : test::negative(rhs)
                                ? negative_overflow_result<Result>(OverflowTag{})
                                : static_cast<Result>(rhs);
            }
        };

        template<typename Result>
        struct convert<native_overflow_tag, Result>
                : convert<_impl::native_tag, Result> {
        };

        template<typename Result>
        struct convert<trapping_overflow_tag, Result> : overflow_convert<trapping_overflow_tag, Result> {
        };

        template<typename Result>
        struct convert<throwing_overflow_tag, Result> : overflow_convert<throwing_overflow_tag, Result> {
        };

        template<typename Result>
        struct convert<saturated_overflow_tag, Result> : overflow_convert<saturated_overflow_tag, Result> {
        };
    }

    using _impl::convert;

    ////////////////////////////////////////////////////////////////////////////////
    // arithmetic overflow

    namespace _impl {
        // implementation assumes one of three things:
        // 1. type is unsigned
        // 2. type is symetrical around zero (e.g. elastic_integer)
        // 3. type has most negative number
        template<typename Operand, bool IsSigned=is_signed<Operand>::value>
        struct has_most_negative_number : std::false_type {};

        template<typename Operand>
        struct has_most_negative_number<Operand, true> : std::integral_constant<bool,
                -(numeric_limits<Operand>::lowest() + 1) == numeric_limits<Operand>::max()> {
        };

        template<class Operator, typename ... Operands>
        class operator_overflow_traits {
            using result = _impl::op_result<Operator, Operands...>;
            using numeric_limits = cnl::numeric_limits<result>;
        public:
            static constexpr int positive_digits = cnl::_impl::positive_digits<result>::value;
            static constexpr int negative_digits = cnl::_impl::negative_digits<result>::value;
            
            static constexpr result lowest()
            {
                return numeric_limits::lowest();
            }
            static constexpr result max()
            {
                return numeric_limits::max();
            }

            template<typename Operand>
            static constexpr int leading_bits(Operand const& operand)
            {
                return cnl::leading_bits(static_cast<result>(operand));
            }
        };

        // overflow tests
        template<class Operator, typename ... Operands>
        struct overflow_test_base {
            // by default, an operation is safe
            static constexpr bool positive(Operands const &...)
            {
                return false;
            }

            static constexpr bool negative(Operands const &...)
            {
                return false;
            }
        };

        template<class Operator, typename ... Operands>
        struct overflow_test : overflow_test_base<Operator, Operands...> {
        };

        template<typename Rhs>
        struct overflow_test<_impl::minus_op, Rhs> : overflow_test_base<_impl::minus_op, Rhs> {
            using traits = operator_overflow_traits<_impl::minus_op, Rhs>;

            static constexpr bool positive(Rhs const &rhs) {
                return has_most_negative_number<Rhs>::value
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4146)
#endif
                       // Causes a warning about unsigned unary minus despite the fact that it should not be being
                       // evaluated when Rhs is unsigned due to an is_signed test in has_most_negative_number.
                       ? (-traits::max()) > rhs
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
                       : 0;
            }

            static constexpr bool negative(Rhs const &rhs) {
                return !is_signed<Rhs>::value && rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::add_op, Lhs, Rhs> : overflow_test_base<_impl::add_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<_impl::add_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs>Lhs{0}
                        && rhs>Rhs{0}
                        && lhs>traits::max()-rhs;
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs<Lhs{0}
                        && rhs<Rhs{0}
                        && lhs<traits::lowest()-rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::subtract_op, Lhs, Rhs> : overflow_test_base<_impl::subtract_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<_impl::subtract_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs>Lhs{0}
                        && rhs<Rhs{0}
                        && lhs>traits::max()+rhs;
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && (rhs>=0)
                        && lhs<traits::lowest()+rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::multiply_op, Lhs, Rhs> : overflow_test_base<_impl::multiply_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<_impl::multiply_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (positive_digits<Lhs>::value+positive_digits<Rhs>::value>traits::positive_digits)
                        && ((lhs>Lhs{0})
                            ? (rhs>Rhs{0}) && (traits::max()/rhs)<lhs
                            : (rhs<Rhs{0}) && (traits::max()/rhs)>lhs);
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (positive_digits<Lhs>::value+positive_digits<Rhs>::value>traits::positive_digits)
                        && ((lhs<Lhs{0})
                            ? (rhs>Rhs{0}) && (traits::lowest()/rhs)>lhs
                            : (rhs<Rhs{0}) && (traits::lowest()/rhs)<lhs);
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::divide_op, Lhs, Rhs>
                : overflow_test_base<_impl::divide_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<_impl::divide_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const &lhs, Rhs const &rhs) {
                return (has_most_negative_number<Lhs>::value && has_most_negative_number<Rhs>::value)
                       ? rhs == -1 && lhs == traits::lowest()
                       : false;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::shift_left_op, Lhs, Rhs> : overflow_test_base<_impl::shift_left_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<_impl::shift_left_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs>0 && rhs>0 && traits::leading_bits(lhs)<int(rhs);
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs<0 && rhs>0 && traits::leading_bits(lhs)<int(rhs);
            }
        };

        template<class OverflowTag, class Operator>
        struct tagged_binary_overflow_operator {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                using overflow_test = overflow_test<Operator, Lhs, Rhs>;
                using result_type = _impl::op_result<Operator, Lhs, Rhs>;
                return overflow_test::positive(lhs, rhs)
                        ? positive_overflow_result<result_type>(OverflowTag{})
                        : overflow_test::negative(lhs, rhs)
                                ? negative_overflow_result<result_type>(OverflowTag{})
                                : tagged_binary_operator<native_tag, Operator>{}(lhs, rhs);
            }
        };

        template<class Operator>
        struct tagged_binary_operator<native_overflow_tag, Operator>
                : tagged_binary_operator<_impl::native_tag, Operator> {
        };

        template<class Operator>
        struct tagged_binary_operator<trapping_overflow_tag, Operator>
                : tagged_binary_overflow_operator<trapping_overflow_tag, Operator> {
        };

        template<class Operator>
        struct tagged_binary_operator<throwing_overflow_tag, Operator>
                : tagged_binary_overflow_operator<throwing_overflow_tag, Operator> {
        };

        template<class Operator>
        struct tagged_binary_operator<saturated_overflow_tag, Operator>
                : tagged_binary_overflow_operator<saturated_overflow_tag, Operator> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // free overflow functions

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto add(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs+rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::add_op>{}(lhs, rhs);
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto subtract(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs-rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::subtract_op>{}(lhs, rhs);
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto multiply(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs*rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::multiply_op>{}(lhs, rhs);
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto shift_left(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs<<rhs)
    {
        return _impl::for_rep<decltype(lhs << rhs)>(
                _impl::tagged_binary_operator<OverflowTag, _impl::shift_left_op>(), lhs, rhs);
    }
}

#endif  // CNL_OVERFLOW_H
