
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_NUMBERS_H)
#define CNL_IMPL_FRACTION_NUMBERS_H

#include "../../numbers.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl::numbers {
    template<typename Numerator, typename Denominator>
    struct signedness<fraction<Numerator, Denominator>>
        : std::conjunction<signedness<Numerator>, signedness<Denominator>> {
    };
}

#endif  // CNL_IMPL_FRACTION_NUMBERS_H
