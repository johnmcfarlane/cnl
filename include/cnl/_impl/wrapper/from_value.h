
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_FROM_VALUE_H)
#define CNL_IMPL_WRAPPER_FROM_VALUE_H

#include "../../numeric.h"
#include "../custom_operator/definition.h"
#include "../custom_operator/tag.h"
#include "../num_traits/from_value.h"
#include "../num_traits/from_value_recursive.h"
#include "../type_traits/common_type.h"
#include "definition.h"
#include "make_wrapper.h"

/// compositional numeric library
namespace cnl {
    /// \cond
    template<typename Rep, tag Tag, typename Value>
    requires(!_impl::is_wrapper<Value> && !_impl::is_constant<Value>::value) struct from_value<
            _impl::wrapper<Rep, Tag>, Value> {
        using deduction = cnl::deduction<Tag, Value>;
        using deduced = _impl::wrapper<typename deduction::type, typename deduction::tag>;

        [[nodiscard]] constexpr auto operator()(Value const& value) const -> deduced
        {
            return deduced{value};
        }
    };

    template<typename ArchetypeRep, tag ArchetypeTag, typename Value>
    requires(!_impl::can_convert_tag_family<ArchetypeTag, _impl::tag_of_t<Value>>::value) struct from_value<
            _impl::wrapper<ArchetypeRep, ArchetypeTag>, Value> {
        using result_type = _impl::wrapper<from_value_t<ArchetypeRep, Value>, ArchetypeTag>;

        [[nodiscard]] constexpr auto operator()(Value const& value) const -> result_type
        {
            return result_type{value};
        }
    };

    template<class ArchetypeRep, tag ArchetypeTag, class ValueRep, class ValueTag>
    requires(_impl::can_convert_tag_family<ArchetypeTag, ValueTag>::value) struct from_value<
            _impl::wrapper<ArchetypeRep, ArchetypeTag>, _impl::wrapper<ValueRep, ValueTag>> {
        [[nodiscard]] constexpr auto operator()(auto const& value) const
        {
            return _impl::from_value_simple<
                  _impl::wrapper<from_value_t<ArchetypeRep, ValueRep>, ArchetypeTag>,
                  _impl::wrapper<ValueRep, ValueTag>>()(value);
        }
    };
    /// \endcond

    template<typename Rep, tag Tag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<_impl::wrapper<Rep, Tag>, constant<Value>> {
        [[nodiscard]] constexpr auto operator()(constant<Value>) const
        {
            return _impl::make_wrapper<Tag>(_impl::from_value<Rep>(constant<Value>{}));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_FROM_VALUE_H
