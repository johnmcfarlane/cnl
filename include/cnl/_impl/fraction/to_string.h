
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_TO_STRING_H)
#define CNL_IMPL_FRACTION_TO_STRING_H 1

#include "gcd.h"
#include "make_fraction.h"
#include "type.h"

#include <string>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::fraction free functions
        using std::to_string;

        template<typename N, typename D>
        std::string to_string(fraction<N, D> const& f)
        {
            auto const numerator_string = to_string(f.numerator);
            auto const denominator_string = to_string(f.denominator);

            auto const total_length = numerator_string.length()+1+denominator_string.length();
            std::string fraction_string;
            fraction_string.reserve(total_length);

            fraction_string = numerator_string;
            fraction_string.push_back('/');
            fraction_string += denominator_string;
            return fraction_string;
        }
    }
}

#endif  // CNL_IMPL_FRACTION_TO_STRING_H
