
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the overflow detection and handling

#if !defined(CNL_OVERFLOW_H)
#define CNL_OVERFLOW_H

#include <cnl/num_traits.h>
#include <cnl/bits/common.h>

#if defined(CNL_EXCEPTIONS_ENABLED)
#include <stdexcept>
#else
#include <cstdio>
#include <exception>
#endif

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // mode tags and objects

    // match the behavior of fundamental arithmetic types
    static constexpr struct native_overflow_tag {
    } native_overflow{};

    // throw an exception when overflow is detected
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};

    // confine range of results
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::convert

    // implementation details
    namespace _overflow_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // throw exception (or crash horribly)

#if defined(CNL_EXCEPTIONS_ENABLED)
        template<class Result>
        constexpr Result return_if(bool condition, Result const& value, char const* )
        {
            return condition ? value : throw std::overflow_error("");
        }
#else
#if (__cplusplus>=201402L)
        template<class Result>
        [[noreturn]] constexpr Result terminate(char const* message) noexcept {
            std::fputs(message, stderr);
            std::terminate();
            return Result{0};
        }
#else
        template<class Result>
        [[noreturn]] Result terminate(char const* message) noexcept {
            std::fputs(message, stderr);
            std::terminate();
            return Result{0};
        }
#endif

        template<class Result>
        constexpr Result return_if(bool condition, Result const& value, char const* message) {
            return condition ? value : terminate<Result>(message);
        }
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // overflow detection

        // positive_digits
        template<class T>
        struct positive_digits : public std::integral_constant<int, std::numeric_limits<T>::digits> {
        };

        template<class T>
        struct negative_digits
                : public std::integral_constant<int, std::is_signed<T>::value ? std::numeric_limits<T>::digits : 0> {
        };

        // is_positive_overflow
        template<
                class Destination, class Source,
                _impl::enable_if_t<_impl::greater_than_or_equal_tag(positive_digits<Destination>::value, positive_digits<Source>::value), int> dummy = 0>
        constexpr bool is_positive_overflow(Source const&)
        {
            // If positive capacity of Destination is equal to or exceeds that of Source,
            // positive overflow cannot occur.
            return false;
        }

        template<
                class Destination, class Source,
                _impl::enable_if_t<_impl::less_than_tag(positive_digits<Destination>::value, positive_digits<Source>::value), int> dummy = 0>
        constexpr bool is_positive_overflow(Source const& source)
        {
            return _impl::greater_than_tag(source, static_cast<Source>(std::numeric_limits<Destination>::max()));
        }

        // is_negative_overflow
        template<
                class Destination, class Source,
                _impl::enable_if_t<_impl::greater_than_or_equal_tag(negative_digits<Destination>::value, negative_digits<Source>::value), int> dummy = 0>
        constexpr bool is_negative_overflow(Source const&)
        {
            // If positive capacity of Destination is equal to or exceeds that of Source,
            // positive overflow cannot occur.
            return false;
        }

        template<
                class Destination, class Source,
                _impl::enable_if_t<_impl::less_than_tag(negative_digits<Destination>::value, negative_digits<Source>::value), int> dummy = 0>
        constexpr bool is_negative_overflow(Source const& source)
        {
            return static_cast<Source>(std::numeric_limits<Destination>::lowest())>source;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // operate

        template<class OverflowTag, class Operator, class Enable = void>
        struct operate;
    }

    template<class Result, class Input>
    constexpr Result convert(native_overflow_tag, Input const& rhs)
    {
        return static_cast<Result>(rhs);
    }

    template<class Result, class Input>
    constexpr Result convert(throwing_overflow_tag, Input const& rhs)
    {
        return _impl::encompasses<Result, Input>::value
               ? static_cast<Result>(rhs)
               : _overflow_impl::return_if(
                        !_overflow_impl::is_positive_overflow<Result>(rhs),
                        _overflow_impl::return_if(
                                !_overflow_impl::is_negative_overflow<Result>(rhs),
                                static_cast<Result>(rhs),
                                "negative overflow in conversion"),
                        "positive overflow in conversion");
    }

    template<class Result, class Input>
    constexpr Result convert(saturated_overflow_tag, Input const& rhs)
    {
        using numeric_limits = std::numeric_limits<Result>;
        return !_impl::encompasses<Result, Input>::value
               ? _overflow_impl::is_positive_overflow<Result>(rhs)
                 ? numeric_limits::max()
                 : _overflow_impl::is_negative_overflow<Result>(rhs)
                   ? numeric_limits::lowest()
                   : static_cast<Result>(rhs)
               : static_cast<Result>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::add

    // implementation details
    namespace _overflow_impl {
        template<>
        struct operate<native_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };

        template<>
        struct operate<throwing_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = std::numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        !((rhs>=_impl::from_rep<Rhs>(0))
                          ? (lhs>numeric_limits::max()-rhs)
                          : (lhs<numeric_limits::lowest()-rhs)),
                        lhs+rhs,
                        "overflow in addition");
            }
        };

        template<>
        struct operate<saturated_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::add_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = std::numeric_limits<result_type>;
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
        return for_rep<decltype(lhs+rhs)>(_overflow_impl::operate<OverflowTag, _impl::add_op>(), lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::subtract

    // implementation details
    namespace _overflow_impl {
        template<>
        struct operate<native_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };

        template<>
        struct operate<throwing_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = std::numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        (rhs<_impl::from_rep<Rhs>(0))
                        ? (lhs<=numeric_limits::max()+rhs)
                        : (lhs>=numeric_limits::lowest()+rhs),
                        lhs-rhs,
                        "positive overflow in subtraction");
            }
        };

        template<>
        struct operate<saturated_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::subtract_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = std::numeric_limits<result_type>;
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
        return for_rep<decltype(lhs-rhs)>(_overflow_impl::operate<OverflowTag, _impl::subtract_op>(), lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::multiply

    // implementation details
    namespace _overflow_impl {
        template<>
        struct operate<native_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };

        template<class Lhs, class Rhs>
        constexpr bool is_multiply_overflow(Lhs const& lhs, Rhs const& rhs)
        {
            using result_nl = std::numeric_limits<decltype(lhs*rhs)>;
            return lhs && rhs && ((lhs>Lhs{})
                                  ? ((rhs>Rhs{}) ? (result_nl::max()/rhs) : (result_nl::lowest()/rhs))<lhs
                                  : ((rhs>Rhs{}) ? (result_nl::lowest()/rhs) : (result_nl::max()/rhs))>lhs);
        }

        template<>
        struct operate<throwing_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return _overflow_impl::return_if(
                        !is_multiply_overflow(lhs, rhs),
                        lhs*rhs, "overflow in multiplication");
            }
        };

        template<>
        struct operate<saturated_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::multiply_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs*rhs);
                return is_multiply_overflow(lhs, rhs)
                       ? ((lhs>0) ^ (rhs>0))
                         ? std::numeric_limits<result_type>::lowest()
                         : std::numeric_limits<result_type>::max()
                       : lhs*rhs;
            }
        };
    }

    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto multiply(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs*rhs)
    {
        return for_rep<decltype(lhs*rhs)>(_overflow_impl::operate<OverflowTag, _impl::multiply_op>(), lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::divide

    // implementation details
    namespace _overflow_impl {
        template<class OverflowTag>
        struct operate<OverflowTag, _impl::divide_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // comparison

    // implementation details
    namespace _overflow_impl {
        template<class Operator>
        struct operate<native_overflow_tag, Operator,
                _impl::enable_if_t<Operator::is_comparison>> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, rhs))
        {
            return Operator()(lhs, rhs);
        }
        };

        template<class Operator>
        struct operate<throwing_overflow_tag, Operator,
                _impl::enable_if_t<Operator::is_comparison>> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                return Operator()(lhs, rhs);
            }
        };

        template<class Operator>
        struct operate<saturated_overflow_tag, Operator,
                _impl::enable_if_t<Operator::is_comparison>> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                // assumes all arithmetic-induced implicit convertion goes the same
                // or at least that `|` is a less "promotion-inducing" operation
                using converted = decltype(lhs | rhs);
                return Operator()(
                        convert<converted>(saturated_overflow, lhs),
                        convert<converted>(saturated_overflow, rhs));
            }
        };
    }
}

#endif  // CNL_OVERFLOW_H
