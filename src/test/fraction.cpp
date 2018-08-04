
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/bits/common.h` definitions

#include <cnl/fraction.h>
#include <cnl/_impl/type_traits.h>

using cnl::_impl::identical;

namespace {
    namespace test_make_fraction {
        static_assert(identical(
                3.14285714285714285714f,
                static_cast<float>(cnl::make_fraction(22, 7))), "cnl::make_fraction");
    }

#if defined(__cpp_deduction_guides)
    namespace test_deduction_guide {
        static_assert(identical(
                3.14285714285714285714f,
                static_cast<float>(cnl::fraction{22, 7})), "cnl::fraction deduction guide");

        static_assert(identical(cnl::fraction<int, int>{4321, 1}, cnl::fraction{4321}));
    }
#endif

    namespace test_add {
        static_assert(identical(
                cnl::make_fraction(9LL, 9),
                cnl::make_fraction(1LL, 3)+cnl::make_fraction(2, 3)), "operator+(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                cnl::make_fraction(7, 12),
                cnl::make_fraction(1, short{4})+cnl::make_fraction(1, short{3})), "operator+(cnl::fraction, cnl::fraction)");
    }

    namespace test_subtract {
        static_assert(identical(
                cnl::make_fraction(-3LL, 9),
                cnl::make_fraction(1LL, 3)-cnl::make_fraction(2, 3)), "operator+(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                cnl::make_fraction(-1, 12),
                cnl::make_fraction(1, short{4})-cnl::make_fraction(1, short{3})), "operator+(cnl::fraction, cnl::fraction)");
    }

    namespace test_multiply {
        static_assert(identical(
                cnl::make_fraction(2LL, 9),
                cnl::make_fraction(1LL, 3)*cnl::make_fraction(2, 3)), "operator*(cnl::fraction, cnl::fraction)");
    }

    namespace test_divide {
        static_assert(identical(
                cnl::make_fraction(3LL, 6),
                cnl::make_fraction(1LL, 3)/cnl::make_fraction(2, 3)), "operator*(cnl::fraction, cnl::fraction)");

        static_assert(identical(
                cnl::make_fraction(1)/cnl::make_fraction(3),
                cnl::make_fraction(1, 3)), "operator/(cnl::fraction, cnl::fraction)");
    }

    namespace test_operator_equal {
        static_assert(identical(
                true,
                cnl::make_fraction(2LL, 9) ==cnl::make_fraction(4, 18LL)),
                      "operator==(cnl::fraction, cnl::fraction)");

        static_assert(identical(
                false,
                cnl::make_fraction(2LL, 9) ==cnl::make_fraction(2LL, 8)),
                      "operator==(cnl::fraction, cnl::fraction)");
    }

#if defined(__cpp_lib_experimental_gcd_lcm)
    namespace test_reduce {
        static_assert(identical(
                cnl::make_fraction(long{128}, long{45}),
                cnl::reduce(cnl::make_fraction(short{1024}, long{360}))), "operator*(cnl::fraction, cnl::fraction)");
    }
#endif
}
