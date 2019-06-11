
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_GCD_H)
#define CNL_IMPL_GCD_H 1

#include "num_traits/is_composite.h"
#include "num_traits/to_rep.h"

#include <numeric>

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(__cpp_lib_gcd)
        template<
                typename Number1, typename Number2,
                typename = enable_if_t<is_composite<Number1>::value&&is_composite<Number2>::value>>
        CNL_NODISCARD constexpr auto gcd(Number1 const& n1, Number2 const& n2) {
            using std::gcd;
            return gcd(to_rep(n1), to_rep(n2));
        }
#endif
    }
}

#endif  // CNL_IMPL_GCD_H
