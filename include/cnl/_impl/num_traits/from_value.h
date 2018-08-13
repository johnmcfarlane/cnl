
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_FROM_VALUE
#define CNL_IMPL_NUM_TRAITS_FROM_VALUE

#include "../../constant.h"

#include "digits.h"
#include "set_digits.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/make_signed.h"
#include "../used_digits.h"

namespace cnl {
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
            Number, Value, _impl::enable_if_t<_impl::is_integral<Number>::value && _impl::is_integral<Value>::value>>
            : _impl::from_value_simple<Value, Value> {
    };

    template<class Number, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<Number, constant<Value>, _impl::enable_if_t<_impl::is_integral<Number>::value>> {
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
}

#endif  // CNL_IMPL_NUM_TRAITS_FROM_VALUE
