//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(_SG14_INTEGER)
#define _SG14_INTEGER 1

#include <sg14/type_traits.h>

#include <limits>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
// _SG14_INTEGER_EXCEPTIONS_ENABLED macro definition 

#if defined(_SG14_INTEGER_EXCEPTIONS_ENABLED)
#error _SG14_INTEGER_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define _SG14_INTEGER_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define _SG14_INTEGER_EXCEPTIONS_ENABLED
#endif
#else
#define _SG14_INTEGER_EXCEPTIONS_ENABLED
#endif

#if defined(_SG14_INTEGER_EXCEPTIONS_ENABLED)

#include <stdexcept>

#endif

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // macros

    // basic definitions
#define _SG14_INTEGER_COMPARISON_DEFINE(OP) \
    template <class Lhs, class Rhs> \
    constexpr auto operator OP (const Lhs& lhs, const Rhs& rhs) \
    -> typename std::enable_if<sg14::_integer_impl::is_integer_class<Lhs>::value || sg14::_integer_impl::is_integer_class<Rhs>::value, bool>::type { \
        using common_type = typename common_type<Lhs, Rhs>::type; \
        return static_cast<common_type>(lhs).data() OP static_cast<common_type>(rhs).data(); }

#define _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(OP) \
    template <class LhsRepr, class RhsRepr, class OverflowPolicy> \
    constexpr auto operator OP (const integer<LhsRepr, OverflowPolicy>& lhs, const integer<RhsRepr, OverflowPolicy>& rhs) \
    -> integer<decltype(std::declval<LhsRepr>() OP std::declval<RhsRepr>()), OverflowPolicy> { \
        using Result = integer<decltype(std::declval<LhsRepr>() OP std::declval<RhsRepr>()), OverflowPolicy>; \
        return static_cast<Result>(lhs.data() OP rhs.data()); } \
    \
    template <class Lhs, class RhsRepr, class RhsOverflowPolicy, typename std::enable_if<std::is_fundamental<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> integer<decltype(std::declval<Lhs>() OP std::declval<RhsRepr>()), RhsOverflowPolicy> { \
        using Result = integer<decltype(std::declval<Lhs>() OP std::declval<RhsRepr>()), RhsOverflowPolicy>; \
        return static_cast<Result>(lhs OP rhs.data()); } \
    \
    template <class LhsRepr, class LhsOverflowPolicy, class Rhs, typename std::enable_if<std::is_fundamental<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> integer<decltype(std::declval<LhsRepr>() OP std::declval<Rhs>()), LhsOverflowPolicy> { \
        using Result = integer<decltype(std::declval<LhsRepr>() OP std::declval<Rhs>()), LhsOverflowPolicy>; \
        return static_cast<Result>(lhs.data() OP rhs); }

#define _SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(OP, BIN_OP) \
    template <class Rhs> \
    auto operator OP (const Rhs& rhs) \
    -> integer& { \
        repr = static_cast<repr_type>(repr BIN_OP rhs); \
        return *this; }

#define _SG14_INTEGER_BIT_SHIFT_DEFINE(OP) \
    template <class LhsRepr, class LhsOverflowPolicy, class RhsRepr, class RhsOverflowPolicy> \
    constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> integer<LhsRepr, LhsOverflowPolicy> { \
        return lhs.data() OP rhs.data(); } \
    \
    template <class Lhs, class RhsRepr, class RhsOverflowPolicy, typename std::enable_if<std::is_fundamental<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> Lhs { \
        return lhs OP rhs.data(); } \
    \
    template <class LhsRepr, class LhsOverflowPolicy, class Rhs, typename std::enable_if<std::is_fundamental<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> integer<LhsRepr, LhsOverflowPolicy> { \
        return integer<LhsRepr, LhsOverflowPolicy>(lhs.data() OP rhs); }

    ////////////////////////////////////////////////////////////////////////////////
    // forward-declarations

    template<typename Repr, typename OverflowPolicy>
    class integer;

    namespace _integer_impl {
        template<class, class, class = void>
        struct common_type;
    };

    namespace _integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_integer_impl::is_integer_class - trait to identify sg14::integer<>

        template<typename T>
        struct is_integer_class;

        template<typename T>
        struct is_integer_class
                : std::false_type {
        };

        template<typename Repr, typename OverflowPolicy>
        struct is_integer_class<integer<Repr, OverflowPolicy>>
                : std::true_type {
        };

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
                    "this function helps convert values *to* sg14::integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

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
                    "this function helps convert values *to* sg14::integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

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
                    "this function helps convert values *to* sg14::integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

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
                    "this function helps convert values *to* sg14::integer");
            static_assert(!is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

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

#if defined(_SG14_INTEGER_EXCEPTIONS_ENABLED)

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
                    "this function helps convert values *to* sg14::integer");
            static_assert(!_integer_impl::is_integer_class<Rhs>::value,
                    "this function helps convert values *to* sg14::integer");

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

        // given two integer<>, produces the type that is best suited to both of them
        template<class LhsRepr, class RhsRepr, class OverflowPolicy>
        struct common_type<
                integer<LhsRepr, OverflowPolicy>,
                integer<RhsRepr, OverflowPolicy>> {
            using type = integer<
                    typename sg14::common_type<LhsRepr, RhsRepr>::type,
                    OverflowPolicy>;
        };

        // given a integer<> and a built-in integer type,
        // generates a integer<> type that is as big as both of them (or as close as possible)
        template<class LhsRepr, class LhsOverflowPolicy, class RhsInteger>
        struct common_type<
                integer<LhsRepr, LhsOverflowPolicy>,
                RhsInteger,
                typename std::enable_if<
                        !_integer_impl::is_integer_class<RhsInteger>::value
                                && std::is_integral<RhsInteger>::value
                >::type> {
            using type = typename sg14::integer<typename sg14::common_type<LhsRepr, RhsInteger>::type, LhsOverflowPolicy>;
        };

        // given a integer<> and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRepr, class LhsOverflowPolicy, class Float>
        struct common_type<
                integer<LhsRepr, LhsOverflowPolicy>,
                Float,
                typename std::enable_if<std::is_floating_point<Float>::value>::type> {
            using type = typename sg14::common_type<LhsRepr, Float>::type;
        };

        // when first type is not integer<> and second type is, reverse the order
        template<class Lhs, class RhsRepr, class RhsOverflowPolicy>
        struct common_type<Lhs, integer<RhsRepr, RhsOverflowPolicy>>
                : common_type<integer<RhsRepr, RhsOverflowPolicy>, Lhs> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::integer<>

    // an integer which can be customized to react in different ways to overflow
    template<typename Repr = int, typename OverflowPolicy = native_overflow_policy>
    class integer {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        using repr_type = Repr;
        using overflow = OverflowPolicy;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

        template<class RhsRepr, typename std::enable_if<!_integer_impl::is_integer_class<RhsRepr>::value, int>::type dummy = 0>
        constexpr explicit integer(const RhsRepr& rhs)
                :repr(OverflowPolicy{}.template convert<repr_type>(rhs))
        {
        }

        template<class Rhs, typename std::enable_if<_integer_impl::is_integer_class<Rhs>::value, int>::type dummy = 0>
        constexpr explicit integer(const Rhs& rhs)
                :repr(OverflowPolicy{}.template convert<repr_type>(rhs.data()))
        {
        }

        template<typename LhsRepr>
        constexpr explicit operator LhsRepr() const
        {
            return static_cast<LhsRepr>(repr);
        }

        constexpr friend integer operator-(const integer& rhs)
        {
            return integer(-rhs.repr);
        }

        _SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(+=, +);

        _SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(-=, -);

        _SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(*=, *);

        _SG14_INTEGER_COMPOUND_ASSIGN_DEFINE(/=, /);

        constexpr repr_type const& data() const
        {
            return repr;
        }

    private:
        ////////////////////////////////////////////////////////////////////////////////
        // variables

        repr_type repr;
    };

    _SG14_INTEGER_COMPARISON_DEFINE(==);

    _SG14_INTEGER_COMPARISON_DEFINE(!=);

    _SG14_INTEGER_COMPARISON_DEFINE(<);

    _SG14_INTEGER_COMPARISON_DEFINE(>);

    _SG14_INTEGER_COMPARISON_DEFINE(<=);

    _SG14_INTEGER_COMPARISON_DEFINE(>=);

    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(+);

    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(-);

    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(*);

    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(/);

    _SG14_INTEGER_BIT_SHIFT_DEFINE(>>);

    _SG14_INTEGER_BIT_SHIFT_DEFINE(<<);

    ////////////////////////////////////////////////////////////////////////////////
    // integer<> partial specializations

    template<typename Repr = int>
    using native_integer = integer<Repr, native_overflow_policy>;

    template<typename Repr = int>
    using throwing_integer = integer<Repr, throwing_overflow_policy>;

    template<typename Repr = int>
    using saturated_integer = integer<Repr, saturated_overflow_policy>;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::set_width<integer<>, > partial specialization

    template<class Repr, class OverflowPolicy, _width_type MinNumBits>
    struct set_width<integer<Repr, OverflowPolicy>, MinNumBits> {
        using type = integer<set_width_t<Repr, MinNumBits>, OverflowPolicy>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width<integer<>> partial specialization

    template<class Repr, class OverflowPolicy>
    struct width<integer<Repr, OverflowPolicy>> : width<Repr> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::integer-specific specializations to std-like templates

    // sg14::is_integral<sg14::integer<>>
    template<typename Repr, typename OverflowPolicy>
    struct is_integral<integer<Repr, OverflowPolicy>>
            : std::integral_constant<bool, is_integral<Repr>::value> {
    };

    // sg14::is_unsigned<sg14::integer<>>
    template<typename Repr, typename OverflowPolicy>
    struct is_unsigned<integer<Repr, OverflowPolicy>>
            : std::integral_constant<bool, is_unsigned<Repr>::value> {
    };

    // sg14::is_signed<sg14::integer<>>
    template<typename Repr, typename OverflowPolicy>
    struct is_signed<integer<Repr, OverflowPolicy>>
            : std::integral_constant<bool, is_signed<Repr>::value> {
    };

    // sg14::make_unsigned<sg14::integer<>>
    template<typename Repr, typename OverflowPolicy>
    struct make_unsigned<integer<Repr, OverflowPolicy>> {
        using type = integer<typename make_unsigned<Repr>::type, OverflowPolicy>;
    };

    // sg14::make_signed<sg14::integer<>>
    template<typename Repr, typename OverflowPolicy>
    struct make_signed<integer<Repr, OverflowPolicy>> {
        using type = integer<typename make_signed<Repr>::type, OverflowPolicy>;
    };

    // std::common_type<T, sg14::integer>
    template<
            class Lhs,
            class RhsRepr, class RhsOverflowPolicy>
    struct common_type<
            Lhs,
            integer<RhsRepr, RhsOverflowPolicy>>
            : _integer_impl::common_type<
                    Lhs,
                    integer<RhsRepr, RhsOverflowPolicy>> {
    };

    // std::common_type<sg14::integer, T>
    template<
            class LhsRepr, class LhsOverflowPolicy,
            class Rhs>
    struct common_type<
            integer<LhsRepr, LhsOverflowPolicy>,
            Rhs>
            : _integer_impl::common_type<
                    integer<LhsRepr, LhsOverflowPolicy>,
                    Rhs> {
    };

    // std::common_type<sg14::integer, sg14::integer>
    template<
            class LhsRepr, class LhsOverflowPolicy,
            class RhsRepr, class RhsOverflowPolicy>
    struct common_type<
            integer<LhsRepr, LhsOverflowPolicy>,
            integer<RhsRepr, RhsOverflowPolicy>>
            : _integer_impl::common_type<
                    integer<LhsRepr, LhsOverflowPolicy>,
                    integer<RhsRepr, RhsOverflowPolicy>> {
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for integer

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<class Repr, class OverflowPolicy>
    struct numeric_limits<sg14::integer<Repr, OverflowPolicy>> {
        // integer-specific helpers
        using _value_type = sg14::integer<Repr, OverflowPolicy>;
        using _repr_type = typename _value_type::repr_type;
        using _repr_numeric_limits = numeric_limits<_repr_type>;

        // standard members

        static constexpr bool is_specialized = true;

        static constexpr _value_type min() noexcept
        {
            return _value_type(_repr_numeric_limits::min());
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type(_repr_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _value_type(_repr_numeric_limits::lowest());
        }

        static constexpr int digits = _repr_numeric_limits::digits;

        static constexpr int digits10 = _repr_numeric_limits::digits10;
        static constexpr int max_digits10 = _repr_numeric_limits::max_digits10;

        static constexpr bool is_signed = _repr_numeric_limits::is_signed;

        static constexpr bool is_integer = true;
        static_assert(is_integer, "integer must be represented using binary type");

        static constexpr bool is_exact = _repr_numeric_limits::is_exact;

        static constexpr int radix = _repr_numeric_limits::radix;
        static_assert(radix==2, "integer must be represented using binary type");

        static constexpr _value_type epsilon() noexcept
        {
            return _repr_numeric_limits::epsilon();
        }

        static constexpr _value_type round_error() noexcept
        {
            return _repr_numeric_limits::round_error();
        }

        // TODO: verify
        static constexpr int min_exponent = _repr_numeric_limits::min_exponent;
        static constexpr int max_exponent = _repr_numeric_limits::max_exponent;

        static constexpr int min_exponent10 = _repr_numeric_limits::min_exponent10;
        static constexpr int max_exponent10 = _repr_numeric_limits::max_exponent10;

        static constexpr bool has_infinity = _repr_numeric_limits::has_infinity;
        static constexpr bool has_quiet_NaN = _repr_numeric_limits::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = _repr_numeric_limits::has_signaling_NaN;
        static constexpr float_denorm_style has_denorm = _repr_numeric_limits::has_denorm;
        static constexpr bool has_denorm_loss = _repr_numeric_limits::has_denorm_loss;

        static constexpr _value_type infinity() noexcept
        {
            return _repr_numeric_limits::infinity();
        }

        static constexpr _value_type quiet_NaN() noexcept
        {
            return _repr_numeric_limits::quiet_NaN();
        }

        static constexpr _value_type signaling_NaN() noexcept
        {
            return _repr_numeric_limits::signaling_NaN();
        }

        static constexpr _value_type denorm_min() noexcept
        {
            return _repr_numeric_limits::denorm_min();
        }

        static constexpr bool is_iec559 = _repr_numeric_limits::is_iec559;
        static constexpr bool is_bounded = _repr_numeric_limits::is_bounded;
        static constexpr bool is_modulo = _repr_numeric_limits::is_modulo;

        static constexpr bool traps = _repr_numeric_limits::traps;
        static constexpr bool tinyness_before = _repr_numeric_limits::tinyness_before;
        static constexpr float_round_style round_style = _repr_numeric_limits::round_style;
    };
}

#endif	// defined(_SG14_INTEGER)
