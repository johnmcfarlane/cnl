
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_THROWING_H)
#define CNL_IMPL_OVERFLOW_THROWING_H

#include "common.h"
#include "overflow_operator.h"
#include "../throw_exception.h"

#include <stdexcept>

/// compositional numeric library
namespace cnl {
    // terminate program with diagnostic when overflow is detected
    // throw an exception when overflow is don overflowetected
    struct throwing_overflow_tag {
    };

    namespace _impl {
        template<typename Operator>
        struct overflow_operator<Operator, throwing_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            constexpr Destination operator()(Source const&) const
            {
                return throw_exception<Destination, std::overflow_error>("positive overflow");
            }

            template<class ... Operands>
            constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return throw_exception<op_result<Operator, Operands...>, std::overflow_error>("positive overflow");
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, throwing_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            constexpr Destination operator()(Source const&) const
            {
                return throw_exception<Destination, std::overflow_error>("negative overflow");
            }

            template<class ... Operands>
            constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return throw_exception<op_result<Operator, Operands...>, std::overflow_error>("negative overflow");
            }
        };

        template<typename Result>
        struct negative_overflow_result<Result, throwing_overflow_tag> {
            constexpr Result operator()() const
            {
                return throw_exception<Result, std::overflow_error>("negative overflow");
            }
        };

        template<typename Destination, typename Source>
        struct tagged_convert_operator<throwing_overflow_tag, Destination, Source>
                : tagged_convert_overflow_operator<throwing_overflow_tag, Destination, Source> {
        };

        template<class Operator>
        struct tagged_unary_operator<throwing_overflow_tag, Operator>
                : tagged_unary_overflow_operator<throwing_overflow_tag, Operator> {
        };

        template<class Operator>
        struct tagged_binary_operator<throwing_overflow_tag, Operator>
                : tagged_binary_overflow_operator<throwing_overflow_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_THROWING_H
