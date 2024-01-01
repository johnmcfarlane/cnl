
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding.h>
#include <test.h>

namespace {
    namespace test_nearest_rounding {

        namespace convert {
            static_assert(identical(
                    std::uint8_t{201},
                    cnl::convert<cnl::nearest_rounding_tag, std::uint8_t>{}(200.5)));
            static_assert(identical(
                    std::int16_t{-1001},
                    cnl::convert<cnl::nearest_rounding_tag, std::int16_t>{}(-1000.5L)));
            static_assert(identical(
                    55,
                    cnl::convert<cnl::nearest_rounding_tag, std::int32_t>{}(55.2F)));

            static_assert(identical(
                    -1,
                    cnl::convert<cnl::nearest_rounding_tag, int>{}(-0.50)));
            static_assert(identical(
                    -0,
                    cnl::convert<cnl::nearest_rounding_tag, int>{}(-0.49)));
            static_assert(identical(
                    +0,
                    cnl::convert<cnl::nearest_rounding_tag, int>{}(0.49)));
            static_assert(identical(
                    +1,
                    cnl::convert<cnl::nearest_rounding_tag, int>{}(0.50)));

            static_assert(identical(
                    std::uint8_t{200},
                    cnl::convert<cnl::native_rounding_tag, std::uint8_t>{}(200.5)));
            static_assert(identical(
                    std::int16_t{-1000},
                    cnl::convert<cnl::native_rounding_tag, std::int16_t>{}(-1000.5L)));
            static_assert(identical(
                    55,
                    cnl::convert<cnl::native_rounding_tag, std::int32_t>{}(55.2F)));

            static_assert(identical(
                    -0,
                    cnl::convert<cnl::native_rounding_tag, int>{}(-0.50)));
            static_assert(identical(
                    -0,
                    cnl::convert<cnl::native_rounding_tag, int>{}(-0.49)));
            static_assert(identical(
                    +0,
                    cnl::convert<cnl::native_rounding_tag, int>{}(0.49)));
            static_assert(identical(
                    +0,
                    cnl::convert<cnl::native_rounding_tag, int>{}(0.50)));
        }

        namespace divide {
            static_assert(identical(
                    -1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(-990, 661)));
            static_assert(identical(
                    2,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(-606, -404)));
            static_assert(identical(
                    1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(8, 9)));
            static_assert(identical(
                    -1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(9, -8)));
            static_assert(identical(
                    -2,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(-9, 6)));
            static_assert(identical(
                    1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(-9, -7)));
            static_assert(identical(
                    2,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(std::uint16_t{999}, 666)));
            static_assert(identical(
                    1LL,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::nearest_rounding_tag>{}(998, 666LL)));
        }

        namespace shift_right {
            static_assert(identical(
                    1 >> 1,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(1, 1)));
            static_assert(identical(
                    1 >> 2,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(1, 2)));

            static_assert(identical(
                    191 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(191, 7)));
            static_assert(identical(
                    192 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(192, 7)));
            static_assert(identical(
                    319 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(319, 7)));
            static_assert(identical(
                    320 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::nearest_rounding_tag>{}(320, 7)));
        }
    }

    namespace tie_to_pos_inf_rounding {

        namespace convert {
            static_assert(identical(
                    std::uint8_t{101},
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, std::uint8_t>{}(100.5)));
            static_assert(identical(
                    std::int16_t{-1000},
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, std::int16_t>{}(-1000.5L)));
            static_assert(identical(
                    55,
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, std::int32_t>{}(55.2F)));
            static_assert(identical(
                    -0,
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(-0.50)));
            static_assert(identical(
                    -0,
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(-0.49)));
            static_assert(identical(
                    +0,
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(0.49)));
            static_assert(identical(
                    +1,
                    cnl::convert<cnl::tie_to_pos_inf_rounding_tag, int>{}(0.50)));
        }

        namespace divide {
            static_assert(identical(
                    -1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(-990, 661)));
            static_assert(identical(
                    2,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(-606, -404)));
            static_assert(identical(
                    1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(8, 9)));
            static_assert(identical(
                    -1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(9, -8)));
            static_assert(identical(
                    -1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(-9, 6)));
            static_assert(identical(
                    1,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(-9, -7)));
            static_assert(identical(
                    2,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(
                            std::uint16_t{999}, 666)));
            static_assert(identical(
                    1LL,
                    cnl::_impl::operate<cnl::_impl::divide_op, cnl::tie_to_pos_inf_rounding_tag>{}(998, 666LL)));
        }

        namespace shift_right {
            static_assert(identical(
                    1 >> 1,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::tie_to_pos_inf_rounding_tag>{}(1, 1)));
            static_assert(identical(
                    1 >> 2,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::tie_to_pos_inf_rounding_tag>{}(1, 2)));
            static_assert(identical(
                    191 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::tie_to_pos_inf_rounding_tag>{}(191, 7)));
            static_assert(identical(
                    192 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::tie_to_pos_inf_rounding_tag>{}(192, 7)));
            static_assert(identical(
                    319 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::tie_to_pos_inf_rounding_tag>{}(319, 7)));
            static_assert(identical(
                    320 >> 7,
                    cnl::_impl::operate<cnl::_impl::shift_right_op, cnl::tie_to_pos_inf_rounding_tag>{}(320, 7)));
        }
    }
}
