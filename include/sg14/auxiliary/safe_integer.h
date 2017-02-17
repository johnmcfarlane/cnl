
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::safe_integer` type

#if !defined(SG14_SAFE_INTEGER_H)
#define SG14_SAFE_INTEGER_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include <sg14/fixed_point>
#endif

#include <stdexcept>

#if defined(SG14_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // macros

    // basic definitions
#define SG14_SAFE_INTEGER_COMPARISON_DEFINE(OP) \
    template <class Lhs, class Rhs> \
    constexpr auto operator OP (const Lhs& lhs, const Rhs& rhs) \
    -> typename std::enable_if<sg14::_integer_impl::are_integer_class_operands<Lhs, Rhs>::value, bool>::type { \
        using common_type = typename std::common_type<Lhs, Rhs>::type; \
        return static_cast<common_type>(lhs).data() OP static_cast<common_type>(rhs).data(); }

#define SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(OP) \
    template <class LhsRep, class RhsRep, class OverflowPolicy> \
    constexpr auto operator OP (const safe_integer<LhsRep, OverflowPolicy>& lhs, const safe_integer<RhsRep, OverflowPolicy>& rhs) \
    -> safe_integer<decltype(std::declval<LhsRep>() OP std::declval<RhsRep>()), OverflowPolicy> { \
        using Result = safe_integer<decltype(std::declval<LhsRep>() OP std::declval<RhsRep>()), OverflowPolicy>; \
        return static_cast<Result>(lhs.data() OP rhs.data()); } \
    \
    template <class Lhs, class RhsRep, class RhsOverflowPolicy, typename std::enable_if<std::is_integral<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const safe_integer<RhsRep, RhsOverflowPolicy>& rhs) \
    -> safe_integer<decltype(std::declval<Lhs>() OP std::declval<RhsRep>()), RhsOverflowPolicy> { \
        using Result = safe_integer<decltype(std::declval<Lhs>() OP std::declval<RhsRep>()), RhsOverflowPolicy>; \
        return static_cast<Result>(lhs OP rhs.data()); } \
    \
    template <class LhsRep, class LhsOverflowPolicy, class Rhs, typename std::enable_if<std::is_integral<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const safe_integer<LhsRep, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> safe_integer<decltype(std::declval<LhsRep>() OP std::declval<Rhs>()), LhsOverflowPolicy> { \
        using Result = safe_integer<decltype(std::declval<LhsRep>() OP std::declval<Rhs>()), LhsOverflowPolicy>; \
        return static_cast<Result>(lhs.data() OP rhs); } \
    \
    template <class Lhs, class RhsRep, class RhsOverflowPolicy, typename std::enable_if<std::is_floating_point<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const safe_integer<RhsRep, RhsOverflowPolicy>& rhs) \
    -> decltype(std::declval<Lhs>() OP std::declval<RhsRep>()) { \
        return lhs OP rhs.data(); } \
    \
    template <class LhsRep, class LhsOverflowPolicy, class Rhs, typename std::enable_if<std::is_floating_point<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const safe_integer<LhsRep, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> decltype(std::declval<LhsRep>() OP std::declval<Rhs>()) { \
        return lhs.data() OP rhs; }

#define SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(OP, BIN_OP) \
    template <class Rhs> \
    auto operator OP (const Rhs& rhs) \
    -> safe_integer& { \
        _r = static_cast<rep>(_r BIN_OP rhs); \
        return *this; }

#define SG14_INTEGER_BIT_SHIFT_DEFINE(OP) \
    template <class LhsRep, class LhsOverflowPolicy, class RhsRep, class RhsOverflowPolicy> \
    constexpr auto operator OP (const safe_integer<LhsRep, LhsOverflowPolicy>& lhs, const safe_integer<RhsRep, RhsOverflowPolicy>& rhs) \
    -> safe_integer<LhsRep, LhsOverflowPolicy> { \
        return lhs.data() OP rhs.data(); } \
    \
    template <class Lhs, class RhsRep, class RhsOverflowPolicy, typename std::enable_if<std::is_fundamental<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const safe_integer<RhsRep, RhsOverflowPolicy>& rhs) \
    -> Lhs { \
        return lhs OP rhs.data(); } \
    \
    template <class LhsRep, class LhsOverflowPolicy, class Rhs, typename std::enable_if<std::is_fundamental<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const safe_integer<LhsRep, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> safe_integer<LhsRep, LhsOverflowPolicy> { \
        return safe_integer<LhsRep, LhsOverflowPolicy>(lhs.data() OP rhs); }

    ////////////////////////////////////////////////////////////////////////////////
    // forward-declarations

    template<typename Rep, typename OverflowPolicy>
    class safe_integer;

    namespace _integer_impl {
        template<class, class, class = void>
        struct common_type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::is_integer_class - trait to identify sg14::safe_integer<>

        template<typename T>
        struct is_integer_class;

        template<typename T>
        struct is_integer_class
                : std::false_type {
        };

        template<typename Rep, typename OverflowPolicy>
        struct is_integer_class<safe_integer<Rep, OverflowPolicy>>
                : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::are_integer_class_operands - basically identifies
        // operands that should go into a function defined here; filters out fixed-point

        template<class Lhs, class Rhs>
        struct are_integer_class_operands {
            static constexpr int integer_class = is_integer_class<Lhs>::value + is_integer_class<Rhs>::value;
            static constexpr int integer_or_float = _impl::is_integer_or_float<Lhs>::value + _impl::is_integer_or_float<Rhs>::value;
            static constexpr bool value = (integer_class >= 1) && (integer_or_float == 2);
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::arithmetic_result - should op return safe_integer<>
        // or floating-point?

        template<class IntegerClass, class Operand, class RepResult>
        struct arithmetic_result;

        template<class IntegerClass, class Operand, class RepResult>
        struct arithmetic_result;

        ////////////////////////////////////////////////////////////////////////////////
        // overflow detection

        // positive_digits
        template<typename T>
        struct positive_digits : public std::integral_constant<int, std::numeric_limits<T>::digits> {
        };

        template<typename T>
        struct negative_digits : public std::integral_constant<int,
                std::is_signed<T>::value ? std::numeric_limits<T>::digits
                                         : 0> {
        };

        // is_positive_overflow
        template<
                typename Destination, typename Source,
                typename std::enable_if<
                        !(positive_digits<Destination>::value<positive_digits<Source>::value),
                        int>::type dummy = 0>
        constexpr bool is_positive_overflow(Source const&)
        {
            static_assert(!is_integer_class<Destination>::value,
                    "this function helps convert values *to* sg14::safe_integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::safe_integer");

            // If positive capacity of Destination is equal to or exceeds that of Source,
            // positive overflow cannot occur.
            return false;
        }

        template<
                typename Destination, typename Source,
                typename std::enable_if<
                        (positive_digits<Destination>::value<positive_digits<Source>::value),
                        int>::type dummy = 0>
        constexpr bool is_positive_overflow(Source const& source)
        {
            static_assert(!is_integer_class<Destination>::value,
                    "this function helps convert values *to* sg14::safe_integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::safe_integer");

            return source>static_cast<Source>(std::numeric_limits<Destination>::max());
        }

        // is_negative_overflow
        template<
                typename Destination, typename Source,
                typename std::enable_if<
                        !(negative_digits<Destination>::value<negative_digits<Source>::value),
                        int>::type dummy = 0>
        constexpr bool is_negative_overflow(Source const&)
        {
            static_assert(!is_integer_class<Destination>::value,
                    "this function helps convert values *to* sg14::safe_integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::safe_integer");

            // If positive capacity of Destination is equal to or exceeds that of Source,
            // positive overflow cannot occur.
            return false;
        }

        template<
                typename Destination, typename Source,
                typename std::enable_if<
                        (negative_digits<Destination>::value<negative_digits<Source>::value),
                        int>::type dummy = 0>
        constexpr bool is_negative_overflow(Source const& source)
        {
            static_assert(!is_integer_class<Destination>::value,
                    "this function helps convert values *to* sg14::safe_integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::safe_integer");

            return source<static_cast<Source>(std::numeric_limits<Destination>::lowest());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // policies

    struct native_overflow_policy {
        template<typename Lhs, typename Rhs>
        constexpr Lhs convert(const Rhs& rhs) const
        {
            return static_cast<Lhs>(rhs);
        }
    };

#if defined(SG14_EXCEPTIONS_ENABLED)

    struct throwing_overflow_policy {
        template<typename Lhs, typename Rhs>
        constexpr Lhs convert(const Rhs& rhs) const
        {
            return _integer_impl::is_positive_overflow<Lhs>(rhs)
                   ? throw std::overflow_error("positive overflow in coversion")
                   : _integer_impl::is_negative_overflow<Lhs>(rhs)
                     ? throw std::overflow_error("negative overflow in coversion")
                     : static_cast<Lhs>(rhs);
        }
    };

#else
    using throwing_overflow_policy = native_overflow_policy;
#endif

    struct saturated_overflow_policy {
        template<typename Lhs, typename Rhs>
        constexpr Lhs convert(const Rhs& rhs) const
        {
            static_assert(!_integer_impl::is_integer_class<Lhs>::value,
                    "this function helps convert values *to* sg14::safe_integer");
            static_assert(!_integer_impl::is_integer_class<Rhs>::value,
                    "this function helps convert values *to* sg14::safe_integer");

            using LhsNumericLimits = std::numeric_limits<Lhs>;
            return
                    _integer_impl::is_positive_overflow<Lhs>(rhs)
                    ? LhsNumericLimits::max()
                    : _integer_impl::is_negative_overflow<Lhs>(rhs)
                      ? LhsNumericLimits::lowest()
                      : static_cast<Lhs>(rhs);
        }
    };

    namespace _integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::common_type

        // given two safe_integer<>, produces the type that is best suited to both of them
        template<class LhsRep, class RhsRep, class OverflowPolicy>
        struct common_type<
                safe_integer<LhsRep, OverflowPolicy>,
                safe_integer<RhsRep, OverflowPolicy>> {
            using type = safe_integer<
                    typename std::common_type<LhsRep, RhsRep>::type,
                    OverflowPolicy>;
        };

        // given a safe_integer<> and a built-in integer type,
        // generates a safe_integer<> type that is as big as both of them (or as close as possible)
        template<class LhsRep, class LhsOverflowPolicy, class RhsInteger>
        struct common_type<
                safe_integer<LhsRep, LhsOverflowPolicy>,
                RhsInteger,
                typename std::enable_if<
                        !_integer_impl::is_integer_class<RhsInteger>::value
                                && std::is_integral<RhsInteger>::value
                >::type> {
            using type = typename sg14::safe_integer<typename std::common_type<LhsRep, RhsInteger>::type, LhsOverflowPolicy>;
        };

        // given a safe_integer<> and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, class LhsOverflowPolicy, class Float>
        struct common_type<
                safe_integer<LhsRep, LhsOverflowPolicy>,
                Float,
                typename std::enable_if<std::is_floating_point<Float>::value>::type> {
            using type = typename std::common_type<LhsRep, Float>::type;
        };

        // when first type is not safe_integer<> and second type is, reverse the order
        template<class Lhs, class RhsRep, class RhsOverflowPolicy>
        struct common_type<Lhs, safe_integer<RhsRep, RhsOverflowPolicy>>
                : common_type<safe_integer<RhsRep, RhsOverflowPolicy>, Lhs> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::safe_integer<>

    // an integer which can be customized to react in different ways to overflow;
    // currently doesn't correctly detect overflow from operators
    template<typename Rep = int, typename OverflowPolicy = throwing_overflow_policy>
    class safe_integer {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        using rep = Rep;
        using overflow = OverflowPolicy;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

        constexpr safe_integer(const safe_integer& rhs)
            :_r(rhs._r)
        {
        }

        constexpr safe_integer(const rep& rhs)
            :_r(rhs)
        {
        }

        template<class RhsRep, typename std::enable_if<!_integer_impl::is_integer_class<RhsRep>::value, int>::type dummy = 0>
        constexpr explicit safe_integer(const RhsRep& rhs)
                :_r(OverflowPolicy{}.template convert<rep>(rhs))
        {
        }

        template<class Rhs, typename std::enable_if<_integer_impl::is_integer_class<Rhs>::value, int>::type dummy = 0>
        constexpr explicit safe_integer(const Rhs& rhs)
                :_r(OverflowPolicy{}.template convert<rep>(rhs.data()))
        {
        }

        template<typename LhsRep>
        constexpr explicit operator LhsRep() const
        {
            return static_cast<LhsRep>(_r);
        }

        constexpr friend safe_integer operator-(const safe_integer& rhs)
        {
            return safe_integer(-rhs._r);
        }

        SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(+=, +);

        SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(-=, -);

        SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(*=, *);

        SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(/=, /);

        constexpr rep const& data() const
        {
            return _r;
        }

    private:
        ////////////////////////////////////////////////////////////////////////////////
        // variables

        rep _r;
    };

    SG14_SAFE_INTEGER_COMPARISON_DEFINE(==);

    SG14_SAFE_INTEGER_COMPARISON_DEFINE(!=);

    SG14_SAFE_INTEGER_COMPARISON_DEFINE(<);

    SG14_SAFE_INTEGER_COMPARISON_DEFINE(>);

    SG14_SAFE_INTEGER_COMPARISON_DEFINE(<=);

    SG14_SAFE_INTEGER_COMPARISON_DEFINE(>=);

    SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(+);

    SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(-);

    SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(*);

    SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(/);

    SG14_INTEGER_BIT_SHIFT_DEFINE(>>);

    SG14_INTEGER_BIT_SHIFT_DEFINE(<<);

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::set_width<safe_integer<>, > partial specialization

    template<class Rep, class OverflowPolicy, _width_type MinNumBits>
    struct set_width<safe_integer<Rep, OverflowPolicy>, MinNumBits> {
        using type = safe_integer<set_width_t<Rep, MinNumBits>, OverflowPolicy>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width<safe_integer<>> partial specialization

    template<class Rep, class OverflowPolicy>
    struct width<safe_integer<Rep, OverflowPolicy>> : width<Rep> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::safe_integer-specific specializations to std-like templates

    // sg14::make_unsigned<sg14::safe_integer<>>
    template<typename Rep, typename OverflowPolicy>
    struct make_unsigned<safe_integer<Rep, OverflowPolicy>> {
        using type = safe_integer<typename make_unsigned<Rep>::type, OverflowPolicy>;
    };

    // sg14::make_signed<sg14::safe_integer<>>
    template<typename Rep, typename OverflowPolicy>
    struct make_signed<safe_integer<Rep, OverflowPolicy>> {
        using type = safe_integer<typename make_signed<Rep>::type, OverflowPolicy>;
    };
}

namespace std {
    // std::common_type<T, sg14::safe_integer>
    template<
            class Lhs,
            class RhsRep, class RhsOverflowPolicy>
    struct common_type<
            Lhs,
            sg14::safe_integer<RhsRep, RhsOverflowPolicy>>
            : sg14::_integer_impl::common_type<
                    Lhs,
                    sg14::safe_integer<RhsRep, RhsOverflowPolicy>> {
    };

    // std::common_type<sg14::safe_integer, T>
    template<
            class LhsRep, class LhsOverflowPolicy,
            class Rhs>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowPolicy>,
            Rhs>
            : sg14::_integer_impl::common_type<
                    sg14::safe_integer<LhsRep, LhsOverflowPolicy>,
                    Rhs> {
    };

    // std::common_type<sg14::safe_integer, sg14::fixed_point>
    template<
            class LhsRep, class LhsOverflowPolicy,
            class RhsRep, int RhsExponent>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowPolicy>,
            sg14::fixed_point<RhsRep, RhsExponent>>
            : std::common_type<
                    sg14::fixed_point<sg14::safe_integer<LhsRep, LhsOverflowPolicy>, 0>,
                    sg14::fixed_point<RhsRep, RhsExponent>> {
    };

    // std::common_type<sg14::fixed_point, sg14::safe_integer>
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, class RhsOverflowPolicy>
    struct common_type<
            sg14::fixed_point<LhsRep, LhsExponent>,
            sg14::safe_integer<RhsRep, RhsOverflowPolicy>>
            : std::common_type<
                    sg14::fixed_point<LhsRep, LhsExponent>,
                    sg14::fixed_point<sg14::safe_integer<RhsRep, RhsOverflowPolicy>, 0>> {
    };
    
    // std::common_type<sg14::safe_integer, sg14::safe_integer>
    template<
            class LhsRep, class LhsOverflowPolicy,
            class RhsRep, class RhsOverflowPolicy>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowPolicy>,
            sg14::safe_integer<RhsRep, RhsOverflowPolicy>>
            : sg14::_integer_impl::common_type<
                    sg14::safe_integer<LhsRep, LhsOverflowPolicy>,
                    sg14::safe_integer<RhsRep, RhsOverflowPolicy>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for safe_integer

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<class Rep, class OverflowPolicy>
    struct numeric_limits<sg14::safe_integer<Rep, OverflowPolicy>> {
        // safe_integer-specific helpers
        using _value_type = sg14::safe_integer<Rep, OverflowPolicy>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members

        static constexpr bool is_specialized = true;

        static constexpr _value_type min() noexcept
        {
            return _value_type(_rep_numeric_limits::min());
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _value_type(_rep_numeric_limits::lowest());
        }

        static constexpr int digits = _rep_numeric_limits::digits;

        static constexpr int digits10 = _rep_numeric_limits::digits10;
        static constexpr int max_digits10 = _rep_numeric_limits::max_digits10;

        static constexpr bool is_signed = _rep_numeric_limits::is_signed;

        static constexpr bool is_integer = true;
        static_assert(is_integer, "safe_integer must be represented using binary type");

        static constexpr bool is_exact = _rep_numeric_limits::is_exact;

        static constexpr int radix = _rep_numeric_limits::radix;
        static_assert(radix==2, "safe_integer must be represented using binary type");

        static constexpr _value_type epsilon() noexcept
        {
            return _rep_numeric_limits::epsilon();
        }

        static constexpr _value_type round_error() noexcept
        {
            return _rep_numeric_limits::round_error();
        }

        // TODO: verify
        static constexpr int min_exponent = _rep_numeric_limits::min_exponent;
        static constexpr int max_exponent = _rep_numeric_limits::max_exponent;

        static constexpr int min_exponent10 = _rep_numeric_limits::min_exponent10;
        static constexpr int max_exponent10 = _rep_numeric_limits::max_exponent10;

        static constexpr bool has_infinity = _rep_numeric_limits::has_infinity;
        static constexpr bool has_quiet_NaN = _rep_numeric_limits::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = _rep_numeric_limits::has_signaling_NaN;
        static constexpr float_denorm_style has_denorm = _rep_numeric_limits::has_denorm;
        static constexpr bool has_denorm_loss = _rep_numeric_limits::has_denorm_loss;

        static constexpr _value_type infinity() noexcept
        {
            return _rep_numeric_limits::infinity();
        }

        static constexpr _value_type quiet_NaN() noexcept
        {
            return _rep_numeric_limits::quiet_NaN();
        }

        static constexpr _value_type signaling_NaN() noexcept
        {
            return _rep_numeric_limits::signaling_NaN();
        }

        static constexpr _value_type denorm_min() noexcept
        {
            return _rep_numeric_limits::denorm_min();
        }

        static constexpr bool is_iec559 = _rep_numeric_limits::is_iec559;
        static constexpr bool is_bounded = _rep_numeric_limits::is_bounded;
        static constexpr bool is_modulo = _rep_numeric_limits::is_modulo;

        static constexpr bool traps = _rep_numeric_limits::traps;
        static constexpr bool tinyness_before = _rep_numeric_limits::tinyness_before;
        static constexpr float_round_style round_style = _rep_numeric_limits::round_style;
    };
}

#endif	// SG14_SAFE_INTEGER_H
