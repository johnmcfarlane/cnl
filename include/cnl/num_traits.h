
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting generic treatment of numeric types

#if !defined(CNL_NUM_TRAITS)
#define CNL_NUM_TRAITS 1

#include "constant.h"
#include "limits.h"

#include "bits/num_traits/set_digits.h"
#include "bits/power.h"
#include "bits/type_traits.h"
#include "bits/type_traits/is_integral.h"
#include "bits/used_digits.h"

#include <utility>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::is_composite (default specialization)

    template<class T, class Enable = void>
    struct is_composite : std::false_type {
        static_assert(!std::is_reference<T>::value, "T is a reference");
        static_assert(!std::is_const<T>::value, "T is const");
        static_assert(!std::is_volatile<T>::value, "T is volatile");
    };

#if (__cplusplus > 201402L)
    template<class T>
    constexpr auto is_composite_v = is_composite<T>::value;
#endif

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::are_composite

        template<class ... Args>
        struct are_composite;

        template<>
        struct are_composite<> : std::false_type {
        };

        template<class ArgHead, class ... ArgTail>
        struct are_composite<ArgHead, ArgTail...>
                : std::integral_constant<bool, is_composite<typename std::decay<ArgHead>::type>::value || are_composite<ArgTail...>::value> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // digits

    template<typename T>
    struct digits : std::integral_constant<int, numeric_limits<T>::digits> {
        static_assert(numeric_limits<T>::is_specialized, "cnl::digits is not correctly specialized for T");
    };

#if (__cplusplus > 201402L)
    template<class T>
    constexpr int digits_v = digits<T>::value;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::digits<cnl::constant<>>

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct digits<constant<Value>> : std::integral_constant<
            int,
            _impl::used_digits((Value<0) ? -Value : Value)> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::is_integral

    using _impl::is_integral;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::is_signed

    template<class T>
    struct is_signed : std::integral_constant<bool, numeric_limits<T>::is_signed> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_signed

    template<class, class = void>
    struct make_signed;

    template<class T>
    struct make_signed<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {
    };

    template<class T>
    using make_signed_t = typename make_signed<T>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_unsigned

    template<class, class = void>
    struct make_unsigned;

    template<class T>
    struct make_unsigned<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_unsigned<T> {
    };

#if defined(CNL_INT128_ENABLED)
    // TODO: cnl::is_integral
    template<>
    struct make_unsigned<int128> {
        using type = uint128;
    };
    template<>
    struct make_unsigned<uint128> {
        using type = uint128;
    };

    template<>
    struct make_signed<int128> {
        using type = int128;
    };
    template<>
    struct make_signed<uint128> {
        using type = int128;
    };
#endif

    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::to_rep

    /// \brief Returns the value encapsulated in \c number
    /// \param number the 'outer' object
    /// \return the 'inner' value
    /// \sa from_rep, from_value
    template<class Number>
    constexpr Number to_rep(Number const& number) {
        return number;
    }

    namespace _impl {
        using cnl::to_rep;

        template<class Number>
        using to_rep_t = decltype(to_rep(std::declval<Number>()));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::from_rep

    /// \brief generic function object that returns the number encapsulating a given value
    ///
    /// \tparam Number archetype for the encapsulating type
    ///
    /// \note Rather than returning Number, invocation may return an alternative
    /// template instantiation based on input parameter.
    /// \sa to_rep, from_value
    template<class Number, class Enable = void>
    struct from_rep;

    /// \brief Specialization of \ref from_rep for integer types
    ///
    /// \tparam Number fundamental integer type to return
    ///
    /// \note This specialization *does* return integers of type, \c Number
    /// \sa to_rep, from_value
    template<class Number>
    struct from_rep<Number, _impl::enable_if_t<cnl::is_integral<Number>::value>> {
        template<class Rep>
        constexpr Number operator()(Rep const& rep) const {
            // by default, a number type's rep type is the number type itself
            return static_cast<Number>(rep);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::for_rep

    namespace _impl {
        // invoke a given generic lambda on given parameters
        template<class Result, class F, class ... Args,
                _impl::enable_if_t<!_impl::are_composite<Args ...>::value, int> dummy = 0>
        constexpr Result for_rep(F f, Args &&...args) {
            return f(std::forward<Args>(args)...);
        }

        template<class Result, class F, class ... Args,
                _impl::enable_if_t<_impl::are_composite<Args ...>::value, int> dummy = 0>
        constexpr Result for_rep(F f, Args &&...args) {
            return for_rep<Result>(f, to_rep(std::forward<Args>(args))...);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::from_value

    namespace _impl {
        template<typename Result, typename Value>
        struct from_value_simple {
            constexpr Result operator()(Value const& value) const {
                return value;
            }
        };
    }

    template<typename Number, typename Value, class Enable = void>
    struct from_value : _impl::from_value_simple<void, Value> {
        void operator()(Value const &) const;
    };

    template<class Number, class Value>
    struct from_value<
            Number, Value, _impl::enable_if_t<cnl::is_integral<Number>::value && cnl::is_integral<Value>::value>>
            : _impl::from_value_simple<Value, Value> {
    };

    template<class Number, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<Number, constant<Value>, _impl::enable_if_t<is_integral<Number>::value>> {
    private:
        using _result_type = set_digits_t<
                make_signed_t<Number>,
                _impl::max(digits<int>::value, _impl::used_digits(Value))>;
    public:
        constexpr _result_type operator()(constant<Value> const &value) const {
            return _result_type(value);
        }
    };

    namespace _impl {
        template<typename Number, typename Value>
        constexpr auto make_number(Value const& value)
        -> decltype(cnl::from_value<Number, Value>{}(value))
        {
            return cnl::from_value<Number, Value>{}(value);
        }
    }

    template<typename Number, typename Value>
    using from_value_t = decltype(_impl::make_number<Number>(std::declval<Value>()));

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::shift

    template<int Digits, int Radix, class S, class Enable = void>
    struct shift;

    namespace _impl {
        // fundamental integer-friendly cnl::shift algorithm
        template<int Digits, int Radix, typename S, class Enable = void>
        struct default_shift;

        template<int Digits, int Radix, typename S>
        struct default_shift<Digits, Radix, S, _impl::enable_if_t<0<=Digits>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s*power<S, Digits, Radix>())
            {
                return s*power<S, Digits, Radix>();
            }
        };

        // cnl::default_shift<-ve, cnl::constant<>>
        template<int Digits, int Radix, typename S>
        struct default_shift<Digits, Radix, S, _impl::enable_if_t<Digits<0>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s/power<S, -Digits, Radix>())
            {
                return s/power<S, -Digits, Radix>();
            }
        };
    }

    // cnl::shift<..., fundamental-integer>
    template<int Digits, int Radix, class S>
    struct shift<Digits, Radix, S, _impl::enable_if_t<cnl::is_integral<S>::value>>
            : _impl::default_shift<Digits, Radix, S> {
    };

    namespace _impl {
        // cnl::_impl::shift - convenience wrapper for cnl::shift
        template<int Digits, int Radix=2, class S>
        constexpr auto shift(S const& s)
        -> decltype(cnl::shift<Digits, Radix, S>{}(s))
        {
            return cnl::shift<Digits, Radix, S>{}(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::scale

    // returns a scaled value of the same type
    template<int Digits, int Radix, class S, class Enable=void>
    struct scale {
        constexpr S operator()(S const& s) const
        {
            return static_cast<S>(shift<Digits, Radix, S>()(s));
        }
    };

    namespace _impl {
        template<int Digits, class S=void>
        constexpr S scale(S const& s)
        {
            return cnl::scale<Digits, numeric_limits<S>::radix, S>()(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::width / set_width

    namespace _impl {
        template<typename T>
        struct width : std::integral_constant<int, digits<T>::value+is_signed<T>::value> {
        };

        template<typename T, int Bits>
        struct set_width : set_digits<T, Bits - is_signed<T>::value> {
        };

        template<typename T, int Bits>
        using set_width_t = typename set_width<T, Bits>::type;
    }
}

#endif  // CNL_NUM_TRAITS
