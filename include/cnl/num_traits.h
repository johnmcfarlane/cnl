
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting generic treatment of numeric types

#if !defined(CNL_NUM_TRAITS)
#define CNL_NUM_TRAITS 1

#include "constant.h"
#include "limits.h"

#include "bits/num_traits/digits.h"
#include "bits/num_traits/for_rep.h"
#include "bits/num_traits/from_rep.h"
#include "bits/num_traits/from_value.h"
#include "bits/num_traits/is_composite.h"
#include "bits/num_traits/scale.h"
#include "bits/num_traits/set_digits.h"
#include "bits/num_traits/shift.h"
#include "bits/num_traits/to_rep.h"
#include "bits/power.h"
#include "bits/type_traits.h"
#include "bits/type_traits/is_integral.h"
#include "bits/type_traits/is_signed.h"
#include "bits/type_traits/make_signed.h"
#include "bits/type_traits/make_unsigned.h"
#include "bits/used_digits.h"

#include <utility>

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::is_integral

    using _impl::is_integral;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::width / set_width

    namespace _impl {
        template<typename T>
        struct width : std::integral_constant<int, digits<T>::value+is_signed<T>::value> {
        };

        template<typename T, int Bits>
        struct set_width : set_digits<T, Bits - is_signed<T>::value> {
        };

        template<typename T, int Bits>
        using set_width_t = typename set_width<T, Bits>::type;
    }
}

#endif  // CNL_NUM_TRAITS
