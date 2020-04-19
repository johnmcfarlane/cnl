
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_DECLARATION_H)
#define CNL_IMPL_ELASTIC_INTEGER_DECLARATION_H

#include "../num_traits/digits.h"

/// compositional numeric library
namespace cnl {

    /// \brief An integer type with auto-widening operators
    ///
    /// \tparam Digits a count of the number of digits needed to express the number
    /// \tparam Narrowest the most narrow integer type to use for storage
    ///
    /// \note The value is stored in an object of \c Narrowest or
    /// \ref cnl::set_gidits_t<Narrowest, Digits>.
    /// \note Arithmetic operations result in types with an adjusted Digits parameter accordingly.
    /// For instance, when two \ref elastic_integer values are multiplied together,
    /// the resultant type has Digits set to the sum of the operands.
    ///
    /// \sa cnl::elastic_scaled_integer
    template<int Digits = digits<int>::value, class Narrowest = int>
    class elastic_integer;
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_DECLARATION_H
