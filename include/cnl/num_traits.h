
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
    // cnl::from_rep

    /// \brief generic function object that returns the number encapsulating a given value
    ///
    /// \tparam Number archetype for the encapsulating type
    ///
    /// \note Rather than returning Number, invocation may return an alternative
    /// template instantiation based on input parameter.
    /// \sa to_rep, from_value
    template<class Number, class Enable = void>
    struct from_rep;

    /// \brief Specialization of \ref from_rep for integer types
    ///
    /// \tparam Number fundamental integer type to return
    ///
    /// \note This specialization *does* return integers of type, \c Number
    /// \sa to_rep, from_value
    template<class Number>
    struct from_rep<Number, _impl::enable_if_t<cnl::is_integral<Number>::value>> {
        template<class Rep>
        constexpr Number operator()(Rep const& rep) const {
            // by default, a number type's rep type is the number type itself
            return static_cast<Number>(rep);
        }
    };

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
