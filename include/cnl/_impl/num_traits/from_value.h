
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_FROM_VALUE)
#define CNL_IMPL_NUM_TRAITS_FROM_VALUE

#include "../../constant.h"

#include "../type_traits/add_signedness.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/remove_cvref.h"
#include "../used_digits.h"
#include "digits.h"
#include "set_digits.h"

namespace cnl {
    namespace _impl {
        template<typename Result, typename Value>
        struct from_value_simple {
            CNL_NODISCARD constexpr Result operator()(Value const& value) const {
                return value;
            }
        };
    }

    /// \brief returns a scaled value of the same type
    /// \headerfile cnl/scaled_integer.h
    template<typename Number, typename Value, class Enable = void>
    struct from_value {
        void operator()(Value const &) const;
    };

    template<class Number, class Value>
    struct from_value<
            Number, Value, _impl::enable_if_t<_impl::is_integral<Number>::value>>
            : _impl::from_value_simple<Value, Value> {
    };

    template<class Number, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<Number, constant<Value>, _impl::enable_if_t<_impl::is_integral<Number>::value>> {
    private:
        using _result_type = set_digits_t<
                add_signedness_t<Number>,
                _impl::max(digits<int>::value, _impl::used_digits(Value))>;
    public:
        CNL_NODISCARD constexpr _result_type operator()(constant<Value> const &value) const {
            return _result_type(value);
        }
    };

    namespace _impl {
        template<typename Number, typename Value>
        CNL_NODISCARD constexpr auto from_value(Value const& value)
        -> decltype(cnl::from_value<Number, remove_cvref_t<Value>>{}(value))
        {
            return cnl::from_value<Number, remove_cvref_t<Value>>{}(value);
        }
    }

    template<typename Number, typename Value>
    using from_value_t = decltype(_impl::from_value<Number>(std::declval<Value>()));
}

#endif  // CNL_IMPL_NUM_TRAITS_FROM_VALUE
