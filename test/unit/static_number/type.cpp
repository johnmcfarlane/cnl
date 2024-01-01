
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>
#include <test.h>

#include <type_traits>

namespace {
    namespace default_parameters {
        using cnl::_impl::rep_of_t;
        static_assert(
                std::is_same_v<
                        cnl::wide_integer<>,
                        rep_of_t<rep_of_t<rep_of_t<rep_of_t<cnl::static_number<1>>>>>>);
    }

    namespace test_ctor {
        static_assert(
                identical(
                        cnl::static_number<20, -10>{1. / 3},
                        cnl::static_number<20, -10>{cnl::fraction<int>(1, 3)}));
        static_assert(
                identical(
                        cnl::static_number<
                                5, 0, cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>{31},
                        cnl::static_number<
                                5, 0, cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>{
                                31.5}));
    }

    namespace test_make_static_number {
        using namespace cnl::literals;
        static_assert(
                identical(cnl::make_static_number(std::int16_t{7}), cnl::static_number<15>{7}));
        static_assert(identical(cnl::make_static_number(444_c), cnl::static_number<7, 2>{444}));
    }

    namespace test_conversion1 {
        constexpr auto a = cnl::static_number<8, -4>{.4375};
        constexpr auto b = cnl::static_number<5, -1>{a};
        static_assert(identical(cnl::static_number<5, -1>{.5}, b));
    }
}
