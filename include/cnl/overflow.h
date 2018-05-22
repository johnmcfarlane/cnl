
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to overflow detection and handling

#if !defined(CNL_OVERFLOW_H)
#define CNL_OVERFLOW_H

#include "numeric.h"
#include "cnl/bits/operators.h"
#include "bits/encompasses.h"

#if defined(CNL_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif
#include <cstdio>
#include <exception>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // mode tags and objects

    // match the behavior of fundamental arithmetic types
    static constexpr struct native_overflow_tag {
    } native_overflow{};

    // terminate program with diagnostic when overflow is detected
    static constexpr struct trapping_overflow_tag {
    } trapping_overflow{};

    // throw an exception when overflow is don overflowetected
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};

    // confine range of results
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};

    namespace _overflow_impl {
        // wrapper for an overflow tag which doesn't change the value of the operation
        template<class WrappedOverflowTag>
        struct passive_overflow_tag {
        };
    }

    // implementation details
    namespace _overflow_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // crash horribly

        template<class Result>
        [[noreturn]] CNL_RELAXED_CONSTEXPR Result terminate(char const* message) noexcept
        {
            std::fprintf(stderr, "%s\n", message);
            std::terminate();
        }

        template<class Result>
        constexpr Result return_if(trapping_overflow_tag, bool condition, Result const& value, char const* message)
        {
            return condition ? value : terminate<Result>(message);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // throw exception (or crash horribly)

#if defined(CNL_EXCEPTIONS_ENABLED)

        template<class Result>
        constexpr Result return_if(throwing_overflow_tag, bool condition, Result const& value, char const* message)
        {
            return condition ? value : throw std::overflow_error(message);
        }

#else
        template<class Result>
        constexpr Result return_if(throwing_overflow_tag, bool condition, Result const& value, char const* message) {
            return return_if(trapping_overflow, condition, value, message);
        }
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // operators

        template<class OverflowTag, class Operator, class Enable = void>
        struct binary_operator;

        template<class OverflowTag, class Operator, class Enable = void>
        struct comparison_operator;

        ////////////////////////////////////////////////////////////////////////////////
        // passive operators

        template<class Operator, class Enable>
        struct binary_operator<trapping_overflow_tag, Operator, Enable>
                : binary_operator<_overflow_impl::passive_overflow_tag<trapping_overflow_tag>, Operator> {
        };

        template<class Operator, class Enable>
        struct binary_operator<throwing_overflow_tag, Operator, Enable>
                : binary_operator<_overflow_impl::passive_overflow_tag<throwing_overflow_tag>, Operator> {
        };

        template<class Operator, class Enable>
        struct comparison_operator<trapping_overflow_tag, Operator, Enable>
                : binary_operator<_overflow_impl::passive_overflow_tag<trapping_overflow_tag>, Operator> {
        };

        template<class Operator, class Enable>
        struct comparison_operator<throwing_overflow_tag, Operator, Enable>
                : binary_operator<_overflow_impl::passive_overflow_tag<throwing_overflow_tag>, Operator> {
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

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::convert

        template<typename Destination, typename Source>
        struct convert_test {
            static constexpr bool positive(Source const& rhs)
            {
                return _overflow_impl::positive_digits<Destination>::value
                        <_overflow_impl::positive_digits<Source>::value
                        && rhs>static_cast<Source>(numeric_limits<Destination>::max());
            }

            static constexpr bool negative(Source const& rhs)
            {
                return _overflow_impl::negative_digits<Destination>::value
                        <_overflow_impl::negative_digits<Source>::value
                        && rhs<static_cast<Source>(numeric_limits<Destination>::lowest());
            }
        };

        template<class OverflowTag, class Result, class Input>
        struct convert;

        template<class Result, class Input>
        struct convert<native_overflow_tag, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                return static_cast<Result>(rhs);
            }
        };

        template<class OverflowTag, class Result, class Input>
        struct convert<_overflow_impl::passive_overflow_tag<OverflowTag>, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                using test = convert_test<Result, Input>;
                return _overflow_impl::return_if(
                        OverflowTag{},
                        !test::positive(rhs),
                        _overflow_impl::return_if(
                                OverflowTag{},
                                !test::negative(rhs),
                                static_cast<Result>(rhs),
                                "negative overflow in conversion"),
                        "positive overflow in conversion");
            }
        };

        template<class Result, class Input>
        struct convert<trapping_overflow_tag, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                return convert<_overflow_impl::passive_overflow_tag<trapping_overflow_tag>, Result, Input>{}(rhs);
            }
        };

        template<class Result, class Input>
        struct convert<throwing_overflow_tag, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                return convert<_overflow_impl::passive_overflow_tag<throwing_overflow_tag>, Result, Input>{}(rhs);
            }
        };

        template<class Result, class Input>
        struct convert<saturated_overflow_tag, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                using test = convert_test<Result, Input>;
                using numeric_limits = numeric_limits<Result>;
                return test::positive(rhs)
                       ? numeric_limits::max()
                       : test::negative(rhs)
                         ? numeric_limits::lowest()
                         : static_cast<Result>(rhs);
            }
        };
    }

    using _impl::convert;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::add

    // implementation details
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };

        template<class OverflowTag>
        struct binary_operator<_overflow_impl::passive_overflow_tag<OverflowTag>, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        OverflowTag{},
                        !((rhs>=from_rep<Rhs>{}(0))
                          ? (lhs>numeric_limits::max()-rhs)
                          : (lhs<numeric_limits::lowest()-rhs)),
                        lhs+rhs,
                        "overflow in addition");
            }
        };

        template<>
        struct binary_operator<saturated_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::add_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = numeric_limits<result_type>;
                return (rhs>0)
                       ? (lhs>numeric_limits::max()-rhs) ? numeric_limits::max() : lhs+rhs
                       : (lhs<numeric_limits::lowest()-rhs) ? numeric_limits::lowest() : lhs+rhs;
            }
        };
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto add(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs+rhs)
    {
        return _impl::for_rep<decltype(lhs+rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::add_op>(), lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::subtract

    // implementation details
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };

        template<class OverflowTag>
        struct binary_operator<_overflow_impl::passive_overflow_tag<OverflowTag>, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        OverflowTag{},
                        (rhs<from_rep<Rhs>{}(0))
                        ? (lhs<=numeric_limits::max()+rhs)
                        : (lhs>=numeric_limits::lowest()+rhs),
                        lhs-rhs,
                        "positive overflow in subtraction");
            }
        };

        template<>
        struct binary_operator<saturated_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::subtract_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = numeric_limits<result_type>;
                return (rhs<0)
                       ? (lhs>numeric_limits::max()+rhs) ? numeric_limits::max() : lhs-rhs
                       : (lhs<numeric_limits::lowest()+rhs) ? numeric_limits::lowest() : lhs-rhs;
            }
        };
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto subtract(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs-rhs)
    {
        return _impl::for_rep<decltype(lhs-rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::subtract_op>(), lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::multiply

    // implementation details
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };

        template<class Operand>
        constexpr bool is_multiply_overflow(Operand const& lhs, Operand const& rhs)
        {
            using result_nl = numeric_limits<decltype(lhs*rhs)>;
            return lhs && rhs && ((lhs>Operand{0})
                                  ? ((rhs>Operand{0}) ? (result_nl::max()/rhs) : (result_nl::lowest()/rhs))<lhs
                                  : ((rhs>Operand{0}) ? (result_nl::lowest()/rhs) : (result_nl::max()/rhs))>lhs);
        }

        template<class OverflowTag>
        struct binary_operator<_overflow_impl::passive_overflow_tag<OverflowTag>, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return _overflow_impl::return_if(
                        OverflowTag{},
                        !is_multiply_overflow(lhs, rhs),
                        lhs*rhs, "overflow in multiplication");
            }
        };

        template<>
        struct binary_operator<saturated_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::multiply_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs*rhs);
                return is_multiply_overflow(lhs, rhs)
                       ? ((lhs>0) ^ (rhs>0))
                         ? numeric_limits<result_type>::lowest()
                         : numeric_limits<result_type>::max()
                       : lhs*rhs;
            }
        };
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto multiply(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs*rhs)
    {
        return _impl::for_rep<decltype(lhs*rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::multiply_op>(), lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::divide

    // implementation details
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::divide_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };

        template<class OverflowTag>
        struct binary_operator<_overflow_impl::passive_overflow_tag<OverflowTag>, _impl::divide_op>
                : binary_operator<native_overflow_tag, _impl::divide_op> {
        };

        template<>
        struct binary_operator<saturated_overflow_tag, _impl::divide_op>
                : binary_operator<native_overflow_tag, _impl::divide_op> {
        };
    }

    // cnl::divide not actually needed

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::shift_right

    // implementation details
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::shift_right_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs>>rhs)
            {
                return lhs>>rhs;
            }
        };

        template<class OverflowTag>
        struct binary_operator<_overflow_impl::passive_overflow_tag<OverflowTag>, _impl::shift_right_op>
                : binary_operator<native_overflow_tag, _impl::shift_right_op> {
        };

        template<>
        struct binary_operator<saturated_overflow_tag, _impl::shift_right_op>
                : binary_operator<native_overflow_tag, _impl::shift_right_op> {
        };
    }

    // cnl::shift_right not actually needed

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::shift_left

    // implementation details
    namespace _overflow_impl {
        template<class Lhs, class Rhs>
        constexpr bool is_shift_left_overflow(Lhs const& lhs, Rhs const& rhs)
        {
            using result_type = decltype(lhs<<rhs);
            return rhs>0 && unsigned(cnl::leading_bits(static_cast<result_type>(lhs)))<unsigned(rhs);
        }

        template<>
        struct binary_operator<native_overflow_tag, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                using result_type = decltype(lhs<<rhs);
                return static_cast<result_type>(static_cast<cnl::make_unsigned_t<result_type>>(lhs)<<rhs);
            }
        };

        template<class OverflowTag>
        struct binary_operator<_overflow_impl::passive_overflow_tag<OverflowTag>, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                return _overflow_impl::return_if(
                        OverflowTag{},
                        !is_shift_left_overflow(lhs, rhs),
                        binary_operator<native_overflow_tag, _impl::shift_left_op>{}(lhs, rhs),
                        "overflow in shift left");
            }
        };

        template<>
        struct binary_operator<saturated_overflow_tag, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                using result_type = decltype(lhs<<rhs);
                return is_shift_left_overflow(lhs, rhs)
                       ? ((lhs>0) ^ (rhs>0))
                         ? numeric_limits<result_type>::lowest()
                         : numeric_limits<result_type>::max()
                       : binary_operator<native_overflow_tag, _impl::shift_left_op>{}(lhs, rhs);
            }
        };
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto shift_left(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs<<rhs)
    {
        return _impl::for_rep<decltype(lhs<<rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::shift_left_op>(), lhs, rhs);
    }
}

#endif  // CNL_OVERFLOW_H
