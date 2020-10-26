
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief operators represented as types

#if !defined(CNL_IMPL_OPERATORS_OPERATORS_H)
#define CNL_IMPL_OPERATORS_OPERATORS_H

#include "../config.h"
#include "../type_traits/type_identity.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // operation tags

        struct convert_op {
            template<class Destination, class Source>
            CNL_NODISCARD constexpr auto operator()(Source const& source) const
            {
                return static_cast<Destination>(source);
            }
        };

        struct minus_op {
            template<class Rhs>
            CNL_NODISCARD constexpr auto operator()(Rhs const& rhs) const
            {
                return -rhs;
            }
        };

        struct plus_op {
            template<class Rhs>
            CNL_NODISCARD constexpr auto operator()(Rhs const& rhs) const
            {
                return +rhs;
            }
        };

        struct bitwise_not_op {
            template<class Rhs>
            CNL_NODISCARD constexpr auto operator()(Rhs const& rhs) const
            {
                return ~rhs;
            }
        };

        struct add_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs+rhs;
            }
        };

        struct subtract_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs-rhs;
            }
        };

        struct multiply_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs*rhs;
            }
        };

        struct divide_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs/rhs;
            }
        };

        struct modulo_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs%rhs;
            }
        };

        struct bitwise_or_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs | rhs;
            }
        };

        struct bitwise_and_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs & rhs;
            }
        };

        struct bitwise_xor_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs ^ rhs;
            }
        };

        struct shift_left_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                // TODO: Not reproduced locally. Investigate.
                // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
                return lhs << rhs;
            }
        };

        struct shift_right_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs >> rhs;
            }
        };

        struct equal_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs==rhs;
            }
        };

        struct not_equal_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs!=rhs;
            }
        };

        struct less_than_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs<rhs;
            }
        };

        struct greater_than_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs>rhs;
            }
        };

        struct less_than_or_equal_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs<=rhs;
            }
        };

        struct greater_than_or_equal_op {
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return lhs>=rhs;
            }
        };

        struct pre_increment_op {
            template<class Rhs>
            constexpr auto operator()(Rhs& rhs) const
            {
                return ++rhs;
            }
        };

        struct pre_decrement_op {
            template<class Rhs>
            constexpr auto operator()(Rhs& rhs) const
            {
                return --rhs;
            }
        };

        struct post_increment_op {
            template<class Lhs>
            constexpr auto operator()(Lhs& lhs) const
            {
                return lhs++;
            }
        };

        struct post_decrement_op {
            template<class Lhs>
            constexpr auto operator()(Lhs& lhs) const
            {
                return lhs--;
            }
        };

#if defined(__GNUC__)
// GCC warns against short+=short and short-=short
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4244)
#endif
        struct assign_add_op {
            using binary = add_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs += rhs;
            }
        };

        struct assign_subtract_op {
            using binary = subtract_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs -= rhs;
            }
        };
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

        struct assign_multiply_op {
            using binary = multiply_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs *= rhs;
            }
        };

        struct assign_divide_op {
            using binary = divide_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs /= rhs;
            }
        };

        struct assign_modulo_op {
            using binary = modulo_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs %= rhs;
            }
        };

        struct assign_bitwise_or_op {
            using binary = bitwise_or_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs |= rhs;
            }
        };

        struct assign_bitwise_and_op {
            using binary = bitwise_and_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs &= rhs;
            }
        };

        struct assign_bitwise_xor_op {
            using binary = bitwise_xor_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs ^= rhs;
            }
        };

        struct assign_shift_left_op {
            using binary = shift_left_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs <<= rhs;
            }
        };

        struct assign_shift_right_op {
            using binary = shift_right_op;

            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const
            {
                return lhs >>= rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::op_result

        template<class Operator, class ... Operands>
        using op_result = decltype(Operator()(std::declval<Operands>() ...));

        ////////////////////////////////////////////////////////////////////////////////
        // type transformations from increment/decrement to compound add/subtract

        template<class Operator>
        struct pre_to_assign;

        template<>
        struct pre_to_assign<pre_increment_op> : type_identity<assign_add_op> {
        };

        template<>
        struct pre_to_assign<pre_decrement_op> : type_identity<assign_subtract_op> {
        };

        template<class Operator>
        struct post_to_assign;

        template<>
        struct post_to_assign<post_increment_op> : type_identity<assign_add_op> {
        };

        template<>
        struct post_to_assign<post_decrement_op> : type_identity<assign_subtract_op> {
        };
    }
}

#endif //CNL_IMPL_OPERATORS_OPERATORS_H
