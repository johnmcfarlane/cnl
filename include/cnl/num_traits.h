
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

#include "bits/num_traits/digits.h"
#include "bits/num_traits/from_value.h"
#include "bits/num_traits/scale.h"
#include "bits/num_traits/set_digits.h"
#include "bits/num_traits/shift.h"
#include "bits/power.h"
#include "bits/type_traits.h"
#include "bits/type_traits/is_integral.h"
#include "bits/type_traits/make_signed.h"
#include "bits/type_traits/make_unsigned.h"
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
    // cnl::is_integral

    using _impl::is_integral;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::is_signed

    template<class T>
    struct is_signed : std::integral_constant<bool, numeric_limits<T>::is_signed> {
    };

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
