
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_UNDEFINED_H)
#define CNL_IMPL_OVERFLOW_UNDEFINED_H

#include "is_overflow_tag.h"
#include "overflow_operator.h"
#include "../polarity.h"
#include "../unreachable.h"

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify undefined-behavior-on-overflow behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag invoke [undefined behavior](https://en.cppreference.com/w/cpp/language/ub)
    /// when the result exceeds the range of the result type.
    ///
    /// \note Invocation of UB allows optimizing compilers to generate faster code and for undefined behavior
    /// sanitizers to trap on arithmetic errors.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag
    struct undefined_overflow_tag {
    };

    namespace _impl {
        template<>
        struct is_overflow_tag<undefined_overflow_tag> : std::true_type {
        };

        template<typename Operator>
        struct overflow_operator<Operator, undefined_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr Destination operator()(Source const&) const
            {
                return unreachable<Destination>("positive overflow");
            }

            template<class ... Operands>
            CNL_NODISCARD constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return unreachable<op_result<Operator, Operands...>>("positive overflow");
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, undefined_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr Destination operator()(Source const&) const
            {
                return unreachable<Destination>("negative overflow");
            }

            template<class ... Operands>
            CNL_NODISCARD constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return unreachable<op_result<Operator, Operands...>>("negative overflow");
            }
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_UNDEFINED_H
