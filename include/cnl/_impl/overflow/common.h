
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_COMMON_H)
#define CNL_IMPL_OVERFLOW_COMMON_H

#include "../../limits.h"
#include "../../numeric.h"
#include "../native_tag.h"
#include "../num_traits/digits.h"
#include "../operators.h"
#include "../type_traits/is_signed.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // overflow detection

        template<class T>
        struct positive_digits : public std::integral_constant<int, numeric_limits<T>::digits> {
        };

        template<class T>
        struct negative_digits
                : public std::integral_constant<int, cnl::is_signed<T>::value ? digits<T>::value : 0> {
        };

        template<typename Destination, typename Source, typename Enable = void>
        struct convert_test;

        template<typename Destination, typename Source>
        struct convert_test<Destination, Source, enable_if_t<!std::is_floating_point<Source>::value>> {
            static constexpr bool positive(Source const &rhs) {
                return positive_digits<Destination>::value<positive_digits<Source>::value
                                                           && rhs>
                static_cast<Source>(numeric_limits<Destination>::max());
            }

            static constexpr bool negative(Source const &rhs) {
                return negative_digits<Destination>::value<negative_digits<Source>::value
                                                           && rhs <
                                                              static_cast<Source>(numeric_limits<Destination>::lowest());
            }
        };

        template<typename Destination, typename Source>
        struct convert_test<Destination, Source, enable_if_t<std::is_floating_point<Source>::value>> {
            static constexpr bool positive(Source const& rhs)
            {
                return rhs > static_cast<Source>(numeric_limits<Destination>::max());
            }

            static constexpr bool negative(Source const& rhs)
            {
                return rhs < static_cast<Source>(numeric_limits<Destination>::lowest());
            }
        };

        template<class Operator, typename ... Operands>
        struct operator_overflow_traits {
            using result = op_result<Operator, Operands...>;
            using numeric_limits = cnl::numeric_limits<result>;

            static constexpr int positive_digits = _impl::positive_digits<result>::value;
            static constexpr int negative_digits = _impl::negative_digits<result>::value;

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

        // implementation assumes one of three things:
        // 1. type is unsigned
        // 2. type is symetrical around zero (e.g. elastic_integer)
        // 3. type has most negative number
        template<typename Operand, bool IsSigned=is_signed<Operand>::value>
        struct has_most_negative_number : std::false_type {};

        template<typename Operand>
        struct has_most_negative_number<Operand, true> : std::integral_constant<bool,
                numeric_limits<Operand>::lowest()<-numeric_limits<Operand>::max()> {
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
        struct overflow_test<minus_op, Rhs> : overflow_test_base<minus_op, Rhs> {
            using traits = operator_overflow_traits<minus_op, Rhs>;

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
        struct overflow_test<add_op, Lhs, Rhs> : overflow_test_base<add_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<add_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs>Lhs{0}
                        && rhs>Rhs{0}
                        && typename traits::result(lhs)>traits::max()-rhs;
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs<Lhs{0}
                        && rhs<Rhs{0}
                        && typename traits::result(lhs)<traits::lowest()-rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<subtract_op, Lhs, Rhs> : overflow_test_base<subtract_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<subtract_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs>Lhs{0}
                        && rhs<Rhs{0}
                        && lhs>traits::max()+rhs;
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && (rhs>=0)
                        && lhs<traits::lowest()+rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<multiply_op, Lhs, Rhs> : overflow_test_base<multiply_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<multiply_op, Lhs, Rhs>;

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
        struct overflow_test<divide_op, Lhs, Rhs>
                : overflow_test_base<divide_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<divide_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const &lhs, Rhs const &rhs) {
                return (has_most_negative_number<Lhs>::value && has_most_negative_number<Rhs>::value)
                        ? rhs == -1 && lhs == traits::lowest()
                        : false;
            }
        };

        template<typename Lhs, typename Rhs, bool IsSigned>
        struct shift_left_overflow_test;

        template<typename Lhs, typename Rhs>
        struct shift_left_overflow_test<Lhs, Rhs, false>
                : overflow_test_base<Lhs, Rhs> {
            using traits = operator_overflow_traits<shift_left_op, Lhs, Rhs>;

            static constexpr bool negative(Lhs const&, Rhs const&)
            {
                return false;
            }
        };

        template<typename Lhs, typename Rhs>
        struct shift_left_overflow_test<Lhs, Rhs, true>
                : overflow_test_base<Lhs, Rhs> {
            using traits = operator_overflow_traits<shift_left_op, Lhs, Rhs>;

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs<0
                        ? rhs>0
                                ? rhs<traits::positive_digits
                                        ? (lhs >> (traits::positive_digits-rhs))!=-1
                                        : true
                                : false
                        : false;
            }
        };

        // shift-left test that is common to signed and unsigned, i.e. positive overflow
        template<typename Lhs, typename Rhs>
        struct overflow_test<shift_left_op, Lhs, Rhs>
                : shift_left_overflow_test<Lhs, Rhs, is_signed<Lhs>::value> {
            using traits = operator_overflow_traits<shift_left_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs>0
                        ? rhs>0
                                ? rhs<traits::positive_digits
                                        ? (lhs >> (traits::positive_digits-rhs))!=0
                                        : true
                                : false
                        : false;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // result

        template<typename Result, class OverflowTag>
        struct positive_overflow_result;

        template<typename Result, class OverflowTag>
        struct negative_overflow_result;

        ////////////////////////////////////////////////////////////////////////////////
        // overflow_convert

        template<class OverflowTag, typename Result, typename Input>
        struct overflow_convert {
            constexpr Result operator()(Input const& rhs) const
            {
                using test = convert_test<Result, Input>;
                return test::positive(rhs)
                        ? positive_overflow_result<Result, OverflowTag>{}()
                        : test::negative(rhs)
                                ? negative_overflow_result<Result, OverflowTag>{}()
                                : static_cast<Result>(rhs);
            }
        };


        template<class OverflowTag, class Operator>
        struct tagged_binary_overflow_operator {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> op_result<Operator, Lhs, Rhs>
            {
                using overflow_test = overflow_test<Operator, Lhs, Rhs>;
                using result_type = op_result<Operator, Lhs, Rhs>;
                return overflow_test::positive(lhs, rhs)
                        ? positive_overflow_result<result_type, OverflowTag>{}()
                        : overflow_test::negative(lhs, rhs)
                                ? negative_overflow_result<result_type, OverflowTag>{}()
                                : tagged_binary_operator<native_tag, Operator>{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_COMMON_H
