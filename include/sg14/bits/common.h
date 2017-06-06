
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(SG14_COMMON_H)
#define SG14_COMMON_H 1

#include <utility>

namespace sg14 {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::min

        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // operator helpers

        ////////////////////////////////////////////////////////////////////////////////
        // operation tags

        struct arithmetic_op {
            static constexpr bool is_arithmetic = true;
        };

        struct comparison_op {
            static constexpr bool is_comparison = true;
        };

        struct minus_op : arithmetic_op {
            template<class Rhs>
            constexpr auto operator()(const Rhs& rhs) const -> decltype(-rhs)
            {
                return -rhs;
            }
        };

        struct plus_op : arithmetic_op {
            template<class Rhs>
            constexpr auto operator()(const Rhs& rhs) const -> decltype(+rhs)
            {
                return +rhs;
            }
        };

        struct add_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };

        struct subtract_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };

        struct multiply_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };

        struct divide_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };

        struct bitwise_or_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs|rhs)
            {
                return lhs|rhs;
            }
        };

        struct bitwise_and_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs&rhs)
            {
                return lhs&rhs;
            }
        };

        struct bitwise_xor_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs^rhs)
            {
                return lhs^rhs;
            }
        };

        struct equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs==rhs)
            {
                return lhs==rhs;
            }
        };

        struct not_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs!=rhs)
            {
                return lhs!=rhs;
            }
        };

        struct less_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs<rhs)
            {
                return lhs<rhs;
            }
        };

        struct greater_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs>rhs)
            {
                return lhs>rhs;
            }
        };

        struct less_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs<=rhs)
            {
                return lhs<=rhs;
            }
        };

        struct greater_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs>=rhs)
            {
                return lhs>=rhs;
            }
        };

        static constexpr plus_op plus_tag {};
        static constexpr minus_op minus_tag {};

        static constexpr add_op add_tag {};
        static constexpr subtract_op subtract_tag {};
        static constexpr multiply_op multiply_tag {};
        static constexpr divide_op divide_tag {};

        static constexpr bitwise_or_op bitwise_or_tag {};
        static constexpr bitwise_and_op bitwise_and_tag {};
        static constexpr bitwise_xor_op bitwise_xor_tag {};

        static constexpr equal_op equal_tag {};
        static constexpr not_equal_op not_equal_tag {};
        static constexpr less_than_op less_than_tag {};
        static constexpr greater_than_op greater_than_tag {};
        static constexpr less_than_or_equal_op less_than_or_equal_tag {};
        static constexpr greater_than_or_equal_op greater_than_or_equal_tag {};

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::rep_op_result

        template<class Operator, class Lhs, class Rhs>
        using op_result = decltype(Operator()(std::declval<Lhs>(), std::declval<Rhs>()));
    }
}

#endif  // SG14_COMMON_H
