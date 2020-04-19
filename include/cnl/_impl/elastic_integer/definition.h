
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_DEFINITION_H)
#define CNL_IMPL_ELASTIC_INTEGER_DEFINITION_H

#include "../../integer.h"
#include "../elastic_tag.h"
#include "../num_traits/digits.h"
#include "../wrapper.h"

/// compositional numeric library
namespace cnl {
    /// \brief An integer type with auto-widening operators.
    ///
    /// \tparam Digits a count of the number of digits needed to express the number
    /// \tparam Narrowest the most narrow integer type to use for storage
    ///
    /// Arithmetic operations return numbers with the appropriate number of digits
    /// necessary to store the correct value, without risk of overflow.
    /// For instance, when two \ref elastic_integer values are multiplied together,
    /// the resultant type has Digits set to the sum of the operands' Digits.
    ///
    /// \note The lowest value of a signed `elastic_integer` type is the negative of
    /// its maximum value. E.g. the range of `elastic_integer<7>` is [-127..127],
    /// and not [-128..127]. This prevents range errors or wasteful stretching or
    /// Digits that would otherwise occur in relation to `-` and `/` operators.
    ///
    /// \note The value is stored in an object of \c Narrowest or
    /// \ref set_digits_t<Narrowest, Digits>.
    ///
    /// \sa elastic_scaled_integer
    template<int Digits = digits_v<int>,integer Narrowest = int>
    using elastic_integer = _impl::wrapper<
            typename elastic_tag<Digits, Narrowest>::rep,
            elastic_tag<Digits, Narrowest>>;
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_DEFINITION_H
