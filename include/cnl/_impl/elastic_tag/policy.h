
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_POLICY_H)
#define CNL_IMPL_ELASTIC_TAG_POLICY_H

#include "../common.h"
#include "../operators/operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Operation, int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy;

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::add_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = _impl::max(LhsDigits, RhsDigits)+1;
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::subtract_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = _impl::max(LhsDigits, RhsDigits) + (LhsIsSigned | RhsIsSigned);
            static constexpr bool is_signed = true;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::multiply_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int contribution(int operand_digits) { return operand_digits == 1 ? 0 : operand_digits; }
            static constexpr int digits = max(1, contribution(LhsDigits)+contribution(RhsDigits));
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::divide_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = LhsDigits;
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::modulo_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = LhsDigits;
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::bitwise_or_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = _impl::max(LhsDigits, RhsDigits);
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::bitwise_and_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = _impl::min(LhsDigits, RhsDigits);
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };

        template<int LhsDigits, bool LhsIsSigned, int RhsDigits, bool RhsIsSigned>
        struct policy<_impl::bitwise_xor_op, LhsDigits, LhsIsSigned, RhsDigits, RhsIsSigned> {
            static constexpr int digits = _impl::max(LhsDigits, RhsDigits);
            static constexpr bool is_signed = LhsIsSigned || RhsIsSigned;
        };
    }
}

#endif  // CNL_IMPL_ELASTIC_TAG_POLICY_H
