
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/rounding.h>

namespace {
    using cnl::_impl::identical;

    namespace test_nearest_rounding {

        namespace convert {
            static_assert(
                    identical(
                            cnl::uint8{201},
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::uint8>(200.5)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            cnl::int16{-1001},
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::int16>(-1000.5L)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            55,
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, cnl::int32>(55.2F)),
                    "cnl::convert test failed");

            static_assert(
                    identical(
                            -1,
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, int>(-0.50)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            -0,
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, int>(-0.49)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            +0,
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, int>(0.49)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            +1,
                            cnl::convert<cnl::nearest_rounding_tag, cnl::_impl::native_tag, int>(0.50)),
                    "cnl::convert test failed");

            static_assert(
                    identical(
                            cnl::uint8{200},
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::uint8>(200.5)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            cnl::int16{-1000},
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::int16>(-1000.5L)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            55,
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, cnl::int32>(55.2F)),
                    "cnl::convert test failed");

            static_assert(
                    identical(
                            -0,
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int>(-0.50)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            -0,
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int>(-0.49)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            +0,
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int>(0.49)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            +0,
                            cnl::convert<cnl::native_rounding_tag, cnl::_impl::native_tag, int>(0.50)),
                    "cnl::convert test failed");
        }

        namespace divide {
            static_assert(identical(-1, cnl::divide<cnl::nearest_rounding_tag>(-990, 661)),
                    "cnl::divide test failed");
            static_assert(identical(2, cnl::divide<cnl::nearest_rounding_tag>(-606, -404)),
                    "cnl::divide test failed");
            static_assert(identical(1, cnl::divide<cnl::nearest_rounding_tag>(8, 9)),
                    "cnl::divide test failed");
            static_assert(identical(-1, cnl::divide<cnl::nearest_rounding_tag>(9, -8)),
                    "cnl::divide test failed");
            static_assert(identical(-2, cnl::divide<cnl::nearest_rounding_tag>(-9, 6)),
                    "cnl::divide test failed");
            static_assert(identical(1, cnl::divide<cnl::nearest_rounding_tag>(-9, -7)),
                    "cnl::divide test failed");
            static_assert(identical(2, cnl::divide<cnl::nearest_rounding_tag, cnl::uint16, int>(999, 666)),
                    "cnl::divide test failed");
            static_assert(identical(1LL, cnl::divide<cnl::nearest_rounding_tag>(998, 666LL)),
                    "cnl::divide test failed");
        }

        namespace shift_right {
            static_assert(identical(1>>1, cnl::shift_right<cnl::nearest_rounding_tag>(1, 1)),
                    "cnl::shift_right test failed");
            static_assert(identical(1>>2, cnl::shift_right<cnl::nearest_rounding_tag>(1, 2)),
                    "cnl::shift_right test failed");

            static_assert(identical(191>>7, cnl::shift_right<cnl::nearest_rounding_tag>(191, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(192>>7, cnl::shift_right<cnl::nearest_rounding_tag>(192, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(319>>7, cnl::shift_right<cnl::nearest_rounding_tag>(319, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(320>>7, cnl::shift_right<cnl::nearest_rounding_tag>(320, 7)),
                    "cnl::shift_right test failed");
        }
    }

    namespace tie_to_pos_inf_rounding {

        namespace convert {
            static_assert(
                    identical(
                            cnl::uint8{101},
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::uint8>(100.5)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            cnl::int16{-1000},
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::int16>(
                                    -1000.5L)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            55,
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, cnl::int32>(55.2F)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            -0,
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, int>(-0.50)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            -0,
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, int>(-0.49)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            +0,
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, int>(0.49)),
                    "cnl::convert test failed");
            static_assert(
                    identical(
                            +1,
                            cnl::convert<cnl::tie_to_pos_inf_rounding_tag, cnl::_impl::native_tag, int>(0.50)),
                    "cnl::convert test failed");
        }

        namespace divide {
            static_assert(identical(-1, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(-990, 661)),
                    "cnl::divide test failed");
            static_assert(identical(2, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(-606, -404)),
                    "cnl::divide test failed");
            static_assert(identical(1, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(8, 9)),
                    "cnl::divide test failed");
            static_assert(identical(-1, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(9, -8)),
                    "cnl::divide test failed");
            static_assert(identical(-1, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(-9, 6)),
                    "cnl::divide test failed");
            static_assert(identical(1, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(-9, -7)),
                    "cnl::divide test failed");
            static_assert(identical(2, cnl::divide<cnl::tie_to_pos_inf_rounding_tag, cnl::uint16, int>(999, 666)),
                    "cnl::divide test failed");
            static_assert(identical(1LL, cnl::divide<cnl::tie_to_pos_inf_rounding_tag>(998, 666LL)),
                    "cnl::divide test failed");
        }

        namespace shift_right {
            static_assert(identical(1 >> 1, cnl::shift_right<cnl::tie_to_pos_inf_rounding_tag>(1, 1)),
                    "cnl::shift_right test failed");
            static_assert(identical(1 >> 2, cnl::shift_right<cnl::tie_to_pos_inf_rounding_tag>(1, 2)),
                    "cnl::shift_right test failed");
            static_assert(identical(191 >> 7, cnl::shift_right<cnl::tie_to_pos_inf_rounding_tag>(191, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(192 >> 7, cnl::shift_right<cnl::tie_to_pos_inf_rounding_tag>(192, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(319 >> 7, cnl::shift_right<cnl::tie_to_pos_inf_rounding_tag>(319, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(320 >> 7, cnl::shift_right<cnl::tie_to_pos_inf_rounding_tag>(320, 7)),
                    "cnl::shift_right test failed");
        }
    }
}
