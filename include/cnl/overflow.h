
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to overflow detection and handling

#if !defined(CNL_OVERFLOW_H)
#define CNL_OVERFLOW_H

#include "cnl/bits/terminate.h"

#include "numeric.h"
#include "bits/native_tag.h"

#if defined(CNL_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif

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
        // wrapper for an overflow tag which doesn't change the value of the operation
        template<class WrappedOverflowTag>
        struct passive_overflow_tag {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // test condition and return value if true else terminate

        template<class Result>
        constexpr Result return_if(trapping_overflow_tag, bool condition, Result const& value, char const* message)
        {
            return condition ? value : terminate<Result>(message);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // test condition and return value if true else throw

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
        // passive operators

        template<class Operator, class Enable>
        struct tagged_binary_operator<trapping_overflow_tag, Operator, Enable>
                : tagged_binary_operator<_impl::passive_overflow_tag<trapping_overflow_tag>, Operator> {
        };

        template<class Operator, class Enable>
        struct tagged_binary_operator<throwing_overflow_tag, Operator, Enable>
                : tagged_binary_operator<_impl::passive_overflow_tag<throwing_overflow_tag>, Operator> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<class OverflowTag, class Operator, class Enable = void>
        struct comparison_operator : public CNL_ERROR___cannot_use<OverflowTag>::as_a_tag {
        };

        template<class Operator, class Enable>
        struct comparison_operator<trapping_overflow_tag, Operator, Enable>
                : tagged_binary_operator<_impl::passive_overflow_tag<trapping_overflow_tag>, Operator> {
        };

        template<class Operator, class Enable>
        struct comparison_operator<throwing_overflow_tag, Operator, Enable>
                : tagged_binary_operator<_impl::passive_overflow_tag<throwing_overflow_tag>, Operator> {
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

        template<class Result, class Input>
        struct convert<native_overflow_tag, Result, Input>
                : convert<_impl::native_tag, Result, Input> {
        };

        template<class OverflowTag, class Result, class Input>
        struct convert<_impl::passive_overflow_tag<OverflowTag>, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                using test = convert_test<Result, Input>;
                return _impl::return_if(
                        OverflowTag{},
                        !test::positive(rhs),
                        _impl::return_if(
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
                return convert<_impl::passive_overflow_tag<trapping_overflow_tag>, Result, Input>{}(rhs);
            }
        };

        template<class Result, class Input>
        struct convert<throwing_overflow_tag, Result, Input> {
            constexpr Result operator()(Input const& rhs) const
            {
                return convert<_impl::passive_overflow_tag<throwing_overflow_tag>, Result, Input>{}(rhs);
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
    // arithmetic overflow

    namespace _impl {
        // overflow tests
        template<class Operator, typename Lhs, typename Rhs>
        struct overflow_test {
            using result = _impl::op_result<Operator, Lhs, Rhs>;

            static constexpr bool positive(Lhs const&, Rhs const&)
            {
                return false;
            }

            static constexpr bool negative(Lhs const&, Rhs const&)
            {
                return false;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::add_op, Lhs, Rhs> {
            using result = _impl::op_result<_impl::add_op, Lhs, Rhs>;
            using result_nl = numeric_limits<result>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >positive_digits<result>::value)
                        && lhs>Lhs{0}
                        && rhs>Rhs{0}
                        && lhs>result_nl::max()-rhs;
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >positive_digits<result>::value)
                        && lhs<Lhs{0}
                        && rhs<Rhs{0}
                        && lhs<result_nl::lowest()-rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::subtract_op, Lhs, Rhs> {
            using result = _impl::op_result<_impl::subtract_op, Lhs, Rhs>;
            using result_nl = numeric_limits<result>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >positive_digits<result>::value)
                        && lhs>Lhs{0}
                        && rhs<Rhs{0}
                        && lhs>result_nl::max()+rhs;
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (_impl::max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >positive_digits<result>::value)
                        && (rhs>=0)
                        && lhs<result_nl::lowest()+rhs;
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::multiply_op, Lhs, Rhs> {
            using result = _impl::op_result<_impl::multiply_op, Lhs, Rhs>;
            using result_nl = numeric_limits<result>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (positive_digits<Lhs>::value+positive_digits<Rhs>::value>positive_digits<result>::value)
                        && ((lhs>Lhs{0})
                            ? (rhs>Rhs{0}) && (result_nl::max()/rhs)<lhs
                            : (rhs<Rhs{0}) && (result_nl::max()/rhs)>lhs);
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (positive_digits<Lhs>::value+positive_digits<Rhs>::value>positive_digits<result>::value)
                        && ((lhs<Lhs{0})
                            ? (rhs>Rhs{0}) && (result_nl::lowest()/rhs)>lhs
                            : (rhs<Rhs{0}) && (result_nl::lowest()/rhs)<lhs);
            }
        };

        template<typename Lhs, typename Rhs>
        struct overflow_test<_impl::shift_left_op, Lhs, Rhs> {
            using result = _impl::op_result<_impl::shift_left_op, Lhs, Rhs>;

            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs>0 && rhs>0 && unsigned(cnl::leading_bits(static_cast<result>(lhs)))<unsigned(rhs);
            }

            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs<0 && rhs>0 && unsigned(cnl::leading_bits(static_cast<result>(lhs)))<unsigned(rhs);
            }
        };

        template<class Operator>
        struct tagged_binary_operator<native_overflow_tag, Operator>
                : tagged_binary_operator<_impl::native_tag, Operator> {
        };

        template<class OverflowTag, class Operator>
        struct tagged_binary_operator<_impl::passive_overflow_tag<OverflowTag>, Operator> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                using overflow_test = overflow_test<Operator, Lhs, Rhs>;
                return return_if(
                        OverflowTag{},
                        !overflow_test::positive(lhs, rhs),
                        return_if(
                                OverflowTag{},
                                !overflow_test::negative(lhs, rhs),
                                tagged_binary_operator<native_tag, Operator>{}(lhs, rhs),
                                "negative overflow"),
                        "positive overflow");
            }
        };

        template<class Operator>
        struct tagged_binary_operator<saturated_overflow_tag, Operator> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                using overflow_test = overflow_test<Operator, Lhs, Rhs>;
                using result_type = typename overflow_test::result;
                return overflow_test::positive(lhs, rhs)
                       ? numeric_limits<result_type>::max()
                       : overflow_test::negative(lhs, rhs)
                         ? numeric_limits<result_type>::lowest()
                         : tagged_binary_operator<native_tag, Operator>{}(lhs, rhs);
            }
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
