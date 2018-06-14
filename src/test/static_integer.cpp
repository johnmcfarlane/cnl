
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_integer.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    namespace default_parameters {
        static_assert(
                cnl::digits<int>::value == cnl::digits<cnl::static_integer<>>::value,
                "cnl::static_integer parameter default test failed");

        static_assert(
                std::is_same<cnl::trapping_overflow_tag, cnl::static_integer<1>::rep::overflow_tag>::value,
                "cnl::static_integer parameter default test failed");
        static_assert(
                std::is_same<cnl::overflow_integer<>::overflow_tag, cnl::static_integer<1>::rep::overflow_tag>::value,
                "cnl::static_integer parameter default test failed");

        static_assert(
                std::is_same<cnl::native_rounding_tag, cnl::static_integer<1>::rounding>::value,
                "cnl::static_integer parameter default test failed");
        static_assert(
                !std::is_same<cnl::_impl::native_rounding_tag, cnl::native_overflow_tag>::value,
                "cnl::static_integer parameter default test failed");

        static_assert(
                std::is_same<int, cnl::static_integer<>::rep::rep::rep>::value,
                "cnl::static_integer parameter default test failed");
        static_assert(
                std::is_same<cnl::elastic_integer<>::rep, cnl::static_integer<>::rep::rep::rep>::value,
                "cnl::static_integer parameter default test failed");
    }

    namespace test_conversion_native {
        static_assert(identical(cnl::static_integer<2>{-1}, cnl::static_integer<2>{-1.500}), "");
        static_assert(identical(cnl::static_integer<2>{-1}, cnl::static_integer<2>{-1.499}), "");
        static_assert(identical(cnl::static_integer<2>{-0}, cnl::static_integer<2>{-.500}), "");
        static_assert(identical(cnl::static_integer<2>{0}, cnl::static_integer<2>{.499}), "");
        static_assert(identical(cnl::static_integer<2>{0}, cnl::static_integer<2>{.500}), "");
        static_assert(identical(cnl::static_integer<2>{1}, cnl::static_integer<2>{1.499}), "");
        static_assert(identical(cnl::static_integer<2>{1}, cnl::static_integer<2>{1.500}), "");
        static_assert(identical(cnl::static_integer<2>{2}, cnl::static_integer<2>{2.499}), "");
        static_assert(identical(cnl::static_integer<2>{2}, cnl::static_integer<2>{2.500}), "");
    }

    namespace test_conversion_nearest {
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-2},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-1.500}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-1},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-1.499}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-1},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-.500}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{0},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{.499}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{1},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{.500}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{1},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{1.499}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{2},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{1.500}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{2},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{2.499}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{3},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{2.500}), "");
    }

    namespace test_division_native {
        TEST(a, b) {
            auto ex = cnl::static_integer<2, cnl::native_rounding_tag>{3/4};
            auto n = cnl::static_integer<2, cnl::native_rounding_tag>{3};
            auto d = cnl::static_integer<3, cnl::native_rounding_tag>{4};
            auto q = n / d;
            ASSERT_EQ(ex, q);
        }
        static_assert(identical(cnl::static_integer<2, cnl::native_rounding_tag>{3/4},
                cnl::static_integer<2, cnl::native_rounding_tag>{3}
                / cnl::static_integer<3, cnl::native_rounding_tag>{4}), "");
        static_assert(identical(
                cnl::static_integer<31, cnl::native_rounding_tag>{0},
                -9 / cnl::static_integer<4, cnl::native_rounding_tag>{10}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::native_rounding_tag>{0},
                cnl::static_integer<2, cnl::native_rounding_tag>{-2} / 3), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::native_rounding_tag>{0},
                cnl::static_integer<2, cnl::native_rounding_tag>{1} / -3), "");
    }

    namespace test_division_nearest {
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{1},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{3} /
                cnl::static_integer<3, cnl::nearest_rounding_tag>{4}), "");
        static_assert(identical(
                cnl::static_integer<31, cnl::nearest_rounding_tag>{-1},
                -9 / cnl::static_integer<4, cnl::nearest_rounding_tag>{10}), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-1},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{-2} / 3), "");
        static_assert(identical(
                cnl::static_integer<2, cnl::nearest_rounding_tag>{0},
                cnl::static_integer<2, cnl::nearest_rounding_tag>{1} / -3), "");
    }

    namespace test_multiply {
        static_assert(identical(cnl::static_integer<6>{7}*cnl::static_integer<13>{321},
                cnl::static_integer<19>{2247}), "");
    }

    namespace test_make_static_integer {
        using namespace cnl::literals;
        static_assert(identical(cnl::make_static_integer(cnl::int16{7}), cnl::static_integer<15>{7}), "");
        static_assert(identical(cnl::make_static_integer(7_c), cnl::static_integer<3>{7}), "");
    }

    namespace test_shift_right_native {
        static_assert(identical(
                cnl::static_integer<3, cnl::native_rounding_tag>{1},
                cnl::static_integer<3, cnl::native_rounding_tag>{7} >> cnl::constant<2>{}),
                        "");
        static_assert(identical(
                cnl::static_integer<3, cnl::native_rounding_tag>{1},
                cnl::static_integer<3, cnl::native_rounding_tag>{7} >> 2), "");
        static_assert(identical(
                cnl::static_integer<4, cnl::native_rounding_tag>{1},
                cnl::static_integer<4, cnl::native_rounding_tag>{12} >> cnl::constant<3>{}),
                        "");
    }

    namespace test_shift_right_nearest {
        static_assert(identical(
                cnl::static_integer<3, cnl::nearest_rounding_tag>{2},
                cnl::static_integer<3, cnl::nearest_rounding_tag>{7} >> cnl::constant<2>{}),
                        "");
        static_assert(identical(
                cnl::static_integer<3, cnl::nearest_rounding_tag>{2},
                cnl::static_integer<3, cnl::nearest_rounding_tag>{7} >> 2), "");
        static_assert(identical(
                cnl::static_integer<4, cnl::nearest_rounding_tag>{2},
                cnl::static_integer<4, cnl::nearest_rounding_tag>{12} >> cnl::constant<3>{}),
                        "");
    }

    namespace test_shift_native {
        static_assert(identical(
                cnl::static_integer<3, cnl::native_rounding_tag>{1},
                cnl::_impl::shift<-2, 2>(cnl::static_integer<3, cnl::native_rounding_tag>{7})), "");
    }

    namespace test_shift_nearest {
        static_assert(identical(
                cnl::static_integer<3, cnl::nearest_rounding_tag>{2},
                cnl::_impl::shift<-2, 2>(cnl::static_integer<3, cnl::nearest_rounding_tag>{7})), "");
    }

    namespace test_scale_native {
        static_assert(identical(
                cnl::static_integer<3, cnl::native_rounding_tag>{1},
                cnl::_impl::scale<-2, 2>(cnl::static_integer<3, cnl::native_rounding_tag>{7})), "");
    }

    namespace test_scale_nearest {
        static_assert(identical(
                cnl::static_integer<3, cnl::nearest_rounding_tag>{2},
                cnl::_impl::scale<-2, 2>(cnl::static_integer<3, cnl::nearest_rounding_tag>{7})), "");
    }

    TEST(static_integer, conversion_overflow_trapping) {
        using si = cnl::static_integer<5>;
        ASSERT_DEATH(si{32}, "positive overflow in conversion");
    }

    static_assert(identical(
            cnl::static_integer<5, cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>{31},
            cnl::static_integer<5, cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>{32}), "");
}
