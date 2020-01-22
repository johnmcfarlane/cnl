
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_DEDUCTION_H)
#define CNL_IMPL_ELASTIC_INTEGER_DEDUCTION_H

#include "../num_traits/digits.h"
#include "../num_traits/set_width.h"
#include "../num_traits/width.h"
#include "../operators/homogeneous_deduction_tag_base.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int ArchetypeDigits, typename ArchetypeNarrowest, typename Initializer>
    struct deduction<
            elastic_tag<ArchetypeDigits, ArchetypeNarrowest>,
            Initializer> {
        // tag associated with deduced type
        using tag = elastic_tag<
                digits<Initializer>::value,
                _impl::set_width_t<Initializer, _impl::width<ArchetypeNarrowest>::value>>;

        // deduced type
        using type = Initializer;
    };
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_DEDUCTION_H
