
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/bits/common.h` definitions

#include <cnl/fractional.h>
#include <cnl/bits/type_traits.h>

using cnl::_impl::identical;

namespace {
    namespace test_make_fractional {
        static_assert(identical(
                3.14285714285714285714f,
                static_cast<float>(cnl::make_fractional(22, 7))), "cnl::make_fractional");
    }

#if defined(__cpp_deduction_guides)
    namespace test_deduction_guide {
        static_assert(identical(
                3.14285714285714285714f,
                static_cast<float>(cnl::fractional{22, 7})), "cnl::fractional deduction guide");

        static_assert(identical(cnl::fractional<int, int>{4321, 1}, cnl::fractional{4321}));
    }
#endif

    namespace test_add {
        static_assert(identical(
                cnl::make_fractional(9LL, 9),
                cnl::make_fractional(1LL, 3)+cnl::make_fractional(2, 3)), "operator+(cnl::fractional, cnl::fractional)");
        static_assert(identical(
                cnl::make_fractional(7, 12),
                cnl::make_fractional(1, short{4})+cnl::make_fractional(1, short{3})), "operator+(cnl::fractional, cnl::fractional)");
    }

    namespace test_subtract {
        static_assert(identical(
                cnl::make_fractional(-3LL, 9),
                cnl::make_fractional(1LL, 3)-cnl::make_fractional(2, 3)), "operator+(cnl::fractional, cnl::fractional)");
        static_assert(identical(
                cnl::make_fractional(-1, 12),
                cnl::make_fractional(1, short{4})-cnl::make_fractional(1, short{3})), "operator+(cnl::fractional, cnl::fractional)");
    }

    namespace test_multiply {
        static_assert(identical(
                cnl::make_fractional(2LL, 9),
                cnl::make_fractional(1LL, 3)*cnl::make_fractional(2, 3)), "operator*(cnl::fractional, cnl::fractional)");
    }

    namespace test_divide {
        static_assert(identical(
                cnl::make_fractional(3LL, 6),
                cnl::make_fractional(1LL, 3)/cnl::make_fractional(2, 3)), "operator*(cnl::fractional, cnl::fractional)");

        static_assert(identical(
                cnl::make_fractional(1)/cnl::make_fractional(3),
                cnl::make_fractional(1, 3)), "operator/(cnl::fractional, cnl::fractional)");
    }

    namespace test_operator_equal {
        static_assert(identical(
                true,
                cnl::make_fractional(2LL, 9) == cnl::make_fractional(4, 18LL)),
                      "operator==(cnl::fractional, cnl::fractional)");

        static_assert(identical(
                false,
                cnl::make_fractional(2LL, 9) == cnl::make_fractional(2LL, 8)),
                      "operator==(cnl::fractional, cnl::fractional)");
    }

#if defined(__cpp_lib_experimental_gcd_lcm)
    namespace test_reduce {
        static_assert(identical(
                cnl::make_fractional(long{128}, long{45}),
                cnl::reduce(cnl::make_fractional(short{1024}, long{360}))), "operator*(cnl::fractional, cnl::fractional)");
    }
#endif
}
