
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>

namespace {
    using cnl::_impl::identical;

    namespace default_parameters {
        static_assert(
                std::is_same<cnl::static_number<1>::rep::rep::rep::rep, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_make_static_number {
        using namespace cnl::literals;
        static_assert(identical(cnl::make_static_number(cnl::int16{7}), cnl::static_number<15>{7}), "");
        static_assert(identical(cnl::make_static_number(444_c), cnl::static_number<7, 2>{444}), "");
    }

    namespace test_multiply {
        static_assert(identical(
                cnl::static_number<6>{7}*cnl::static_number<13>{321},
                cnl::static_number<19>{2247}), "");
    }

    namespace test_conversion1 {
        using namespace cnl;
        constexpr auto a = cnl::static_number<8, -4>{.4375};
        constexpr auto b = cnl::static_number<5, -1>{a};
        static_assert(identical(cnl::static_number<5, -1>{.5}, b), "");
    }
}
