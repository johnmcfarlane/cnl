
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_BASE_IS_DERIVED_FROM_NUMBER_BASE_H)
#define CNL_IMPL_NUMBER_BASE_IS_DERIVED_FROM_NUMBER_BASE_H

#include "../type_traits/enable_if.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_class_derived_from_number_base

        // true iff T's base class is cnl::_impl::number_base;
        // T must be a class;
        // used by cnl::_impl::is_derived_from_number_base
        template<class Derived, class Enable = void>
        struct is_class_derived_from_number_base : std::false_type {
        };

        template<class Derived>
        struct is_class_derived_from_number_base<const Derived> : is_class_derived_from_number_base<Derived> {
        };

        template<class Derived>
        struct is_class_derived_from_number_base<
                Derived,
                enable_if_t<std::is_base_of<number_base<Derived, typename Derived::rep>, Derived>::value>>
            : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_derived_from_number_base

        // true if T is the Derived parameter of a number_base type
        template<class T, class Enable = void>
        struct is_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_derived_from_number_base<Derived, enable_if_t<std::is_class<Derived>::value>>
            : is_class_derived_from_number_base<Derived> { };
    }
}

#endif  // CNL_IMPL_NUMBER_BASE_IS_DERIVED_FROM_NUMBER_BASE_H
