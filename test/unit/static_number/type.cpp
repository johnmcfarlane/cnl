
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/static_number.h>

namespace {
    using cnl::_impl::assert_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        using cnl::_impl::rep_of_t;
        static_assert(
                assert_same<
                        cnl::wide_integer<>,
                        rep_of_t<rep_of_t<rep_of_t<rep_of_t<cnl::static_number<1>>>>>>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_ctor {
        static_assert(
                identical(
                        cnl::static_number<20, cnl::power<-10>>{1. / 3},
                        cnl::static_number<20, cnl::power<-10>>{cnl::fraction<int>(1, 3)}));
        static_assert(
                identical(
                        cnl::static_number<
                                5, cnl::power<>, cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>{31},
                        cnl::static_number<
                                5, cnl::power<>, cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>{
                                31.5}));
    }

    namespace test_make_static_number {
        using namespace cnl::literals;
        static_assert(
                identical(cnl::make_static_number(cnl::int16{7}), cnl::static_number<15>{7}));
        static_assert(identical(
                cnl::make_static_number(444_c),
                cnl::static_number<7, cnl::CNL_IMPL_DEFAULT_STATIC_NUMBER_SCALE<2>>{444}));
    }

    namespace test_conversion1 {
        constexpr auto a = cnl::static_number<8, cnl::power<-4>>{.4375};
        constexpr auto b = cnl::static_number<5, cnl::power<-1>>{a};
        static_assert(identical(cnl::static_number<5, cnl::power<-1>>{.5}, b));
    }

    namespace test_conversion2 {
        constexpr auto a = cnl::static_number<8, cnl::quasi_exact<-4>>{.4375};
        constexpr auto b = cnl::static_number<5, cnl::quasi_exact<-1>>{a};
        static_assert(identical(cnl::static_number<5, cnl::quasi_exact<-1>>{.5}, b));
    }
}
