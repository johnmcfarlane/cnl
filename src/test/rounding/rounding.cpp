
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::_impl::identical;

    namespace test_nearest_rounding {

        namespace convert {
            static_assert(
                    identical(cnl::convert<cnl::nearest_rounding_tag, cnl::uint8>(200.5), cnl::uint8{201}),
                    "cnl::convert test failed");
            static_assert(identical(cnl::convert<cnl::nearest_rounding_tag, cnl::int16>(-1000.5L),
                                    cnl::int16{-1001}),
                          "cnl::convert test failed");
            static_assert(identical(cnl::convert<cnl::nearest_rounding_tag, cnl::int32>(55.2f), 55),
                          "cnl::convert test failed");

            static_assert(identical(-1, cnl::convert<cnl::nearest_rounding_tag, int>(-0.50)),
                          "cnl::convert test failed");
            static_assert(identical(-0, cnl::convert<cnl::nearest_rounding_tag, int>(-0.49)),
                          "cnl::convert test failed");
            static_assert(identical(+0, cnl::convert<cnl::nearest_rounding_tag, int>(0.49)),
                          "cnl::convert test failed");
            static_assert(identical(+1, cnl::convert<cnl::nearest_rounding_tag, int>(0.50)),
                          "cnl::convert test failed");

            static_assert(
                    identical(cnl::convert<cnl::native_rounding_tag, cnl::uint8>(200.5), cnl::uint8{200}),
                    "cnl::convert test failed");
            static_assert(identical(cnl::convert<cnl::native_rounding_tag, cnl::int16>(-1000.5L),
                                    cnl::int16{-1000}),
                          "cnl::convert test failed");
            static_assert(identical(cnl::convert<cnl::native_rounding_tag, cnl::int32>(55.2f), 55),
                          "cnl::convert test failed");

            static_assert(identical(-0, cnl::convert<cnl::native_rounding_tag, int>(-0.50)),
                          "cnl::convert test failed");
            static_assert(identical(-0, cnl::convert<cnl::native_rounding_tag, int>(-0.49)),
                          "cnl::convert test failed");
            static_assert(identical(+0, cnl::convert<cnl::native_rounding_tag, int>(0.49)),
                          "cnl::convert test failed");
            static_assert(identical(+0, cnl::convert<cnl::native_rounding_tag, int>(0.50)),
                          "cnl::convert test failed");
        }

        namespace divide {
            static_assert(identical(-1, cnl::divide<cnl::nearest_rounding_tag, int, int>{}(-990, 661)),
                    "cnl::divide test failed");
            static_assert(identical(2, cnl::divide<cnl::nearest_rounding_tag, int, int>{}(-606, -404)),
                    "cnl::divide test failed");
            static_assert(identical(1, cnl::divide<cnl::nearest_rounding_tag, int, int>{}(8, 9)),
                    "cnl::divide test failed");
            static_assert(identical(-1, cnl::divide<cnl::nearest_rounding_tag, int, int>{}(9, -8)),
                    "cnl::divide test failed");
            static_assert(identical(-2, cnl::divide<cnl::nearest_rounding_tag, int, int>{}(-9, 6)),
                    "cnl::divide test failed");
            static_assert(identical(1, cnl::divide<cnl::nearest_rounding_tag, int, int>{}(-9, -7)),
                    "cnl::divide test failed");
            static_assert(identical(2, cnl::divide<cnl::nearest_rounding_tag, cnl::uint16, int>{}(999, 666)),
                    "cnl::divide test failed");
            static_assert(identical(1LL, cnl::divide<cnl::nearest_rounding_tag, int, long long>{}(998, 666LL)),
                    "cnl::divide test failed");
        }

        namespace shift_right {
            static_assert(identical(1, cnl::shift_right<cnl::nearest_rounding_tag, int, int>{}(1, 1)),
                    "cnl::shift_right test failed");
            static_assert(identical(0, cnl::shift_right<cnl::nearest_rounding_tag, int, int>{}(1, 2)),
                    "cnl::shift_right test failed");

            static_assert(identical(1, cnl::shift_right<cnl::nearest_rounding_tag, int, int>{}(191, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(2, cnl::shift_right<cnl::nearest_rounding_tag, int, int>{}(192, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(2, cnl::shift_right<cnl::nearest_rounding_tag, int, int>{}(319, 7)),
                    "cnl::shift_right test failed");
            static_assert(identical(3, cnl::shift_right<cnl::nearest_rounding_tag, int, int>{}(320, 7)),
                    "cnl::shift_right test failed");
        }
    }
}
