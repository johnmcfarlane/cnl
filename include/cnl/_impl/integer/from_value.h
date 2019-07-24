
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_FROM_VALUE_H)
#define CNL_IMPL_INTEGER_FROM_VALUE_H

#include "type.h"
#include "../num_traits/from_value.h"
#include "../num_traits/from_value_recursive.h"
#include "../type_traits/common_type.h"
#include "../../numeric.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag, typename Value>
    struct from_value<
            _impl::integer<Rep, Tag>, Value,
            _impl::enable_if_t<!_impl::is_integer<Value>::value && !_impl::is_constant<Value>::value>>
            : _impl::from_value_simple<_impl::integer<Value, Tag>, Value> {
    };

    template<class ArchetypeRep, class ArchetypeTag, class ValueRep, class ValueTag>
    struct from_value<
            _impl::integer<ArchetypeRep, ArchetypeTag>,
            _impl::integer<ValueRep, ValueTag>,
            _impl::enable_if_t<!_impl::can_convert_tag_family<ArchetypeTag, ValueTag>::value>> {
        using result_type = _impl::integer<
                from_value_t<ArchetypeRep, ValueRep>,
                ArchetypeTag>;
        CNL_NODISCARD constexpr auto operator()(_impl::integer<ValueRep, ValueTag> const& value) const
        -> result_type
        {
            return result_type{value};
        }
    };

    template<class ArchetypeRep, class ArchetypeTag, class ValueRep, class ValueTag>
    struct from_value<
            _impl::integer<ArchetypeRep, ArchetypeTag>,
            _impl::integer<ValueRep, ValueTag>,
            _impl::enable_if_t<_impl::can_convert_tag_family<ArchetypeTag, ValueTag>::value>>
            : _impl::from_value_simple<
                    _impl::integer<
                            from_value_t<ArchetypeRep, ValueRep>,
                            ArchetypeTag>,
                    _impl::integer<ValueRep, ValueTag>> {
    };

    template<typename Rep, class Tag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<_impl::integer<Rep, Tag>, constant<Value>> {
        CNL_NODISCARD constexpr auto operator()(constant<Value>) const
        -> decltype(_impl::make_integer<Tag>(_impl::from_value<Rep>(constant<Value>{})))
        {
            return _impl::make_integer<Tag>(_impl::from_value<Rep>(constant<Value>{}));
        }
    };
}

#endif  // CNL_IMPL_INTEGER_FROM_VALUE_H
