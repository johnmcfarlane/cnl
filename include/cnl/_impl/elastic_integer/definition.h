
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_DEFINITION_H)
#define CNL_IMPL_ELASTIC_INTEGER_DEFINITION_H

#include "../elastic_tag.h"
#include "../num_traits/set_digits.h"
#include "../number.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<int Digits, class Narrowest>
        using elastic_rep_t = typename set_digits<
                Narrowest,
                _impl::max(digits<Narrowest>::value, Digits)>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_elastic_integer

        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };

        template<int Digits, class Narrowest>
        struct is_elastic_integer<elastic_integer<Digits, Narrowest>> : std::true_type {
        };

        template<typename Rep, int Digits, typename Narrowest>
        struct is_elastic_integer<number<Rep, elastic_tag<Digits, Narrowest>>>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::elastic_integer_base_t

        template<int Digits, class Narrowest>
        using elastic_integer_base_t = _impl::number<
                _impl::rep_t<elastic_integer<Digits, Narrowest>>,
                _impl::tag_t<elastic_integer<Digits, Narrowest>>>;
    }

    template<int Digits, class Narrowest>
    class elastic_integer : public _impl::elastic_integer_base_t<Digits, Narrowest> {
    public:
        using _base = _impl::elastic_integer_base_t<Digits, Narrowest>;
        static_assert(!_impl::is_elastic_integer<_impl::rep_t<_base>>::value,
                "elastic_integer of elastic_integer is not a supported");

        /// default constructor
        elastic_integer() = default;

        /// constructor not taking cnl::fraction
        template<typename Number>
        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        constexpr elastic_integer(Number const& n)
                : _base(n) { }
    };

    namespace _impl {
        template<bool Signed>
        struct machine_digits {
            static constexpr int value =
                    digits<typename std::conditional<Signed, signed, unsigned>::type>::value;
        };

        template<typename S>
        using narrowest = set_digits_t<S, machine_digits<is_signed<S>::value>::value>;
    }

    template<class S>
    elastic_integer(S const& s)
    -> elastic_integer<digits_v<S>, _impl::narrowest<S>>;

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    elastic_integer(constant<Value>)
    -> elastic_integer<digits_v<constant<Value>>>;
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_DEFINITION_H
