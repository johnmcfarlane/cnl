
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_FROM_VALUE_H)
#define CNL_IMPL_NUM_TRAITS_FROM_VALUE_H

#include "../../constant.h"
#include "../narrow_cast.h"
#include "../numbers/set_signedness.h"
#include "../type_traits/remove_cvref.h"
#include "../used_digits.h"
#include "digits.h"
#include "set_digits.h"

#include <algorithm>
#include <concepts>

namespace cnl {
    namespace _impl {
        template<typename Result, typename Value>
        struct from_value_simple {
            [[nodiscard]] constexpr auto operator()(Value const& value) const
            {
                return Result{value};
            }
        };
    }

    template<typename Number, typename Value>
    struct from_value {
        void operator()(Value const&) const;
    };

    template<std::integral Number, class Value>
    struct from_value<Number, Value>
        : _impl::from_value_simple<Value, Value> {
    };

    template<std::integral Number, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<Number, constant<Value>> {
    private:
        using result_type = set_digits_t<
                numbers::set_signedness_t<Number, true>,
                std::max(digits_v<int>, _impl::used_digits(Value))>;

    public:
        [[nodiscard]] constexpr auto operator()(constant<Value> const& value) const
        {
            return _impl::narrow_cast<result_type>(value);
        }
    };

    namespace _impl {
        template<typename Number, typename Value>
        [[nodiscard]] constexpr auto from_value(Value const& value)
                -> decltype(cnl::from_value<Number, remove_cvref_t<Value>>{}(value))
        {
            return cnl::from_value<Number, remove_cvref_t<Value>>{}(value);
        }
    }

    template<typename Number, typename Value>
    using from_value_t = decltype(_impl::from_value<Number>(std::declval<Value>()));
}

#endif  // CNL_IMPL_NUM_TRAITS_FROM_VALUE_H
