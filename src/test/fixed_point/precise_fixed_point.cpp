
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/precise_integer.h>

namespace {
    using cnl::_impl::identical;
    using cnl::precise_integer;

    template<class Rep = int, int Exponent = 0, class RoundingTag = cnl::closest_rounding_tag>
    using precise_fixed_point = cnl::fixed_point<cnl::precise_integer<Rep, RoundingTag>, Exponent>;

    namespace test_numeric_limits {
        static_assert(cnl::numeric_limits<precise_integer<>>::is_specialized, "cnl::numeric_limits<precise_integer<>> test failed");
        static_assert(cnl::numeric_limits<precise_integer<>>::is_integer, "cnl::numeric_limits<precise_integer<>> test failed");
    }

    namespace test_ctor {
        static_assert(identical(
                cnl::_impl::to_rep(precise_fixed_point<>(-8)),
                precise_integer<>(-8)), "precise_fixed_point ctor test failed");
        static_assert(precise_fixed_point<>(0) == cnl::_impl::from_rep<precise_fixed_point<>>(0), "precise_fixed_point ctor test failed");
    }
}
