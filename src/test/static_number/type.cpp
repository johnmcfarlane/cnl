
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/type_traits/assert_same.h>

namespace {
    using cnl::_impl::identical;
    using cnl::_impl::assert_same;

    namespace default_parameters {
        static_assert(
                assert_same<
                        cnl::wide_integer<>,
                        cnl::static_number<1>::rep::rep::rep::rep>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_ctor {
        static_assert(
                identical(
                        cnl::static_number<20, -10>{1./3},
                        cnl::static_number<20, -10>{cnl::fraction<int>(1, 3)}), "");
    }

    namespace test_make_static_number {
        using namespace cnl::literals;
        static_assert(identical(cnl::make_static_number(cnl::int16{7}), cnl::static_number<15>{7}), "");
        static_assert(identical(cnl::make_static_number(444_c), cnl::static_number<7, 2>{444}), "");
    }

    namespace test_conversion1 {
        constexpr auto a = cnl::static_number<8, -4>{.4375};
        constexpr auto b = cnl::static_number<5, -1>{a};
        static_assert(identical(cnl::static_number<5, -1>{.5}, b), "");
    }
}
