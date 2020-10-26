
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_FROM_VALUE_H)
#define CNL_IMPL_NUMBER_FROM_VALUE_H

#include "../../numeric.h"
#include "../num_traits/from_value.h"
#include "../num_traits/from_value_recursive.h"
#include "../operators/generic.h"
#include "../type_traits/common_type.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag, typename Value>
    struct from_value<
            _impl::number<Rep, Tag>, Value,
            _impl::enable_if_t<!_impl::is_number<Value>::value && !_impl::is_constant<Value>::value>> {
        using deduction = cnl::deduction<Tag, Value>;
        using deduced = _impl::number<typename deduction::type, typename deduction::tag>;

        CNL_NODISCARD constexpr auto operator()(Value const& value) const -> deduced
        {
            return deduced{value};
        }
    };

    template<typename ArchetypeRep, class ArchetypeTag, typename Value>
    struct from_value<
            _impl::number<ArchetypeRep, ArchetypeTag>,
            Value,
            _impl::enable_if_t<!_impl::can_convert_tag_family<ArchetypeTag, _impl::tag_t<Value>>::value>> {
        using result_type = _impl::number<
                from_value_t<ArchetypeRep, Value>,
                ArchetypeTag>;

        CNL_NODISCARD constexpr auto operator()(Value const& value) const
        -> result_type
        {
            return result_type{value};
        }
    };

    template<class ArchetypeRep, class ArchetypeTag, class ValueRep, class ValueTag>
    struct from_value<
            _impl::number<ArchetypeRep, ArchetypeTag>,
            _impl::number<ValueRep, ValueTag>,
            _impl::enable_if_t<_impl::can_convert_tag_family<ArchetypeTag, ValueTag>::value>>
            : _impl::from_value_simple<
                    _impl::number<
                            from_value_t<ArchetypeRep, ValueRep>,
                            ArchetypeTag>,
                    _impl::number<ValueRep, ValueTag>> {
    };

    template<typename Rep, class Tag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<_impl::number<Rep, Tag>, constant<Value>> {
        CNL_NODISCARD constexpr auto operator()(constant<Value>) const
        {
            return _impl::make_number<Tag>(_impl::from_value<Rep>(constant<Value>{}));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_FROM_VALUE_H
