
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_CAN_CONVERT_TAG_FAMILY_H)
#define CNL_IMPL_NUMBER_CAN_CONVERT_TAG_FAMILY_H

#include "../operators/is_same_tag_family.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class ToTag, class FromTag>
        struct can_convert_tag_family
            : std::integral_constant<
                      bool,
                      is_same_tag_family<ToTag, FromTag>::value ||
                              std::is_base_of<ToTag, FromTag>::value ||
                              std::is_base_of<FromTag, ToTag>::value> {
        };
    }
}

#endif // CNL_IMPL_NUMBER_CAN_CONVERT_TAG_FAMILY_H
