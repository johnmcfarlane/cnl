
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUMBER_BASE_TO_STRING_H
#define CNL_IMPL_NUMBER_BASE_TO_STRING_H

#include "definition.h"
#include "is_derived_from_number_base.h"
#include "../num_traits/to_rep.h"
#include "../type_traits/enable_if.h"

#include <string>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Derived>
        auto to_string(Derived const& n)
        -> enable_if_t<is_class_derived_from_number_base<Derived>::value, std::string>
        {
            using std::to_string;
            return to_string(to_rep(n));
        }
    }
}

#endif  // CNL_IMPL_NUMBER_BASE_TO_STRING_H
