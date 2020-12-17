
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/cmath.h` definitions

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cmath.h>

namespace {
    using cnl::_impl::identical;

    namespace test_abs {
        static_assert(identical(cnl::abs(-302398479), 302398479), "cnl::abs(int)");
        static_assert(identical(cnl::abs(302398479), 302398479), "cnl::abs(int)");

        static_assert(identical(cnl::abs(-302398479LL), 302398479LL), "cnl::abs(long long)");
        static_assert(
                identical(cnl::abs(302398479ULL), 302398479ULL), "cnl::abs(unsigned long long)");

        static_assert(
                identical(cnl::abs(CNL_INTMAX_C(-302398479)), CNL_INTMAX_C(302398479)),
                "cnl::abs(cnl::intmax)");
        static_assert(
                identical(cnl::abs(CNL_UINTMAX_C(302398479)), CNL_UINTMAX_C(302398479)),
                "cnl::abs(cnl::uintmax)");

        static_assert(identical(cnl::abs(-302398479.), 302398479.), "cnl::abs(double)");
        static_assert(identical(cnl::abs(302398479.F), 302398479.F), "cnl::abs(float)");
    }
}
