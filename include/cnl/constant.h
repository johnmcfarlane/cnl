
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to `std::constant` type and its UDL, ""_c

#if !defined(CNL_CONSTANT_H)
#define CNL_CONSTANT_H

#include "_impl/config.h"
#include "_impl/cstdint/types.h"
#include "_impl/num_traits/digits.h"
#include "_impl/numbers/signedness.h"
#include "_impl/parse.h"
#include "integer.h"
#include "limits.h"  // NOLINT(modernize-deprecated-headers,  hicpp-deprecated-headers)

#include <type_traits>

// CNL_IMPL_CONSTANT_VALUE_TYPE - determines cnl::constant<>::value_type
#if defined(CNL_TEMPLATE_AUTO)
// If template<auto> feature is available, cnl::constant's value can be any type.
#define CNL_IMPL_CONSTANT_VALUE_TYPE auto  // NOLINT(cppcoreguidelines-macro-usage)
#else
// Otherwise it is defaulted to the widest quantity type that can be used as a template argument.
#define CNL_IMPL_CONSTANT_VALUE_TYPE ::cnl::intmax  // NOLINT(cppcoreguidelines-macro-usage)
#endif

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::constant - expresses a value as a type

    /// \brief type whose value is also its non-type template parameter
    /// \headerfile cnl/constant.h
    ///
    /// \tparam Value the value of the object

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct constant {
#if defined(CNL_TEMPLATE_AUTO)
        using value_type = decltype(Value);
#else
        using value_type = CNL_IMPL_CONSTANT_VALUE_TYPE;
#endif

        static constexpr value_type value = Value;

        constexpr constant() = default;

        template<typename S>
        constexpr explicit constant(S const& init)
        {
            static_assert(
                    value <= cnl::numeric_limits<S>::max()
                            && value >= cnl::numeric_limits<S>::lowest(),
                    "initial value couldn't possibly represent value");
            CNL_ASSERT(value == init);
        }

#if defined(_MSC_VER) && _MSC_VER < 1924
        // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
        [[nodiscard]] constexpr operator auto() const -> value_type
        {
            return value;
        }
#else
        // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
        [[nodiscard]] constexpr operator value_type() const
        {
            return value;
        }
#endif
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::constant operator overloads

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_IMPL_CONSTANT_UNARY(OPERATOR) \
    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value> \
    [[nodiscard]] constexpr auto operator OPERATOR( \
            constant<Value>) noexcept->constant<OPERATOR Value> \
    { \
        return constant<OPERATOR Value>{}; \
    }

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_IMPL_CONSTANT_BINARY(OPERATOR) \
    template<CNL_IMPL_CONSTANT_VALUE_TYPE LhsValue, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue> \
    [[nodiscard]] constexpr auto operator OPERATOR( \
            constant<LhsValue>, \
            constant<RhsValue>) noexcept->constant<(LhsValue OPERATOR RhsValue)> \
    { \
        return constant<(LhsValue OPERATOR RhsValue)>{}; \
    }

    // arithmetic
    CNL_IMPL_CONSTANT_UNARY(+)
    CNL_IMPL_CONSTANT_UNARY(-)
    CNL_IMPL_CONSTANT_BINARY(+)
    CNL_IMPL_CONSTANT_BINARY(-)
    CNL_IMPL_CONSTANT_BINARY(*)
    CNL_IMPL_CONSTANT_BINARY(/)
    CNL_IMPL_CONSTANT_BINARY(%)
    CNL_IMPL_CONSTANT_UNARY(~)
    CNL_IMPL_CONSTANT_BINARY(&)
    CNL_IMPL_CONSTANT_BINARY(|)
    CNL_IMPL_CONSTANT_BINARY(^)
    CNL_IMPL_CONSTANT_BINARY(<<)
    CNL_IMPL_CONSTANT_BINARY(>>)

    // logical
    CNL_IMPL_CONSTANT_UNARY(!)
    CNL_IMPL_CONSTANT_BINARY(&&)
    CNL_IMPL_CONSTANT_BINARY(||)

    // comparison
    CNL_IMPL_CONSTANT_BINARY(==)
    CNL_IMPL_CONSTANT_BINARY(!=)
    CNL_IMPL_CONSTANT_BINARY(<)
    CNL_IMPL_CONSTANT_BINARY(>)
    CNL_IMPL_CONSTANT_BINARY(<=)
    CNL_IMPL_CONSTANT_BINARY(>=)

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_constant type traits

        template<class T>
        struct is_constant : std::false_type {
        };

        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        struct is_constant<::cnl::constant<Value>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::literals - literal wrapper for constant
    //
    // http://codereview.stackexchange.com/a/51576/26421

    namespace literals {
        template<char... Chars>
        [[nodiscard]] constexpr auto operator"" _c()
                -> constant<_impl::parse<intmax, Chars...>()>
        {
            return {};
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    template<auto Value>
    inline constexpr int digits<constant<Value>> = _impl::used_digits((Value < 0) ? -Value : Value);

    ////////////////////////////////////////////////////////////////////////////////
    // numbers traits

    namespace numbers {
        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        struct signedness<constant<Value>> : signedness<decltype(Value)> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // concepts

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct is_integer<constant<Value>> : is_integer<decltype(Value)> {
    };

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct is_integer<constant<Value> const> : is_integer<decltype(Value)> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits<cnl::constant>

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct numeric_limits<constant<Value>>
        : cnl::numeric_limits<typename constant<Value>::value_type> {
        using _value_type = typename constant<Value>::value_type;

        [[nodiscard]] static constexpr auto min()
        {
            return _value_type{};
        }

        [[nodiscard]] static constexpr auto max()
        {
            return _value_type{};
        }

        [[nodiscard]] static constexpr auto lowest()
        {
            return _value_type{};
        }
    };
}

#endif  // CNL_CONSTANT_H
