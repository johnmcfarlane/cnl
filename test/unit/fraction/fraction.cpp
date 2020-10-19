
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fraction.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;
    using cnl::_impl::assert_same;

    template<typename Numerator, typename Denominator>
    CNL_NODISCARD constexpr bool identical(
            cnl::fraction<Numerator, Denominator> const& a,
            cnl::fraction<Numerator, Denominator> const& b)
    {
        return identical(a.numerator, b.numerator) && identical(a.denominator, b.denominator);
    }

    namespace test_parameters {
        static_assert(
                assert_same<
                        cnl::fraction<int, int>,
                        cnl::fraction<>>::value,
                "cnl::fraction first parameter should default to int");
        static_assert(
                assert_same<
                        cnl::fraction<short, short>,
                        cnl::fraction<short>>::value,
                "cnl::fraction second parameter should default to first");
    }

    namespace test_make_fraction {
        static_assert(identical(
                3.14285714285714285714F,
                static_cast<float>(cnl::make_fraction(22, 7))), "cnl::make_fraction");
    }

    namespace test_deduction_guide {
        static_assert(identical(
                cnl::fraction<short>(123),
                cnl::fraction(short{123})), "cnl::fraction argument deduction");
        static_assert(identical(
                cnl::fraction<int>(3.14285714285714285714F),
                cnl::fraction(3.14285714285714285714F)), "cnl::fraction argument deduction");

        static_assert(identical(cnl::fraction<int, int>{4321, 1}, cnl::fraction{4321}));
    }

    namespace test_conversion_op {
        static_assert(identical(
                3.14285714285714285714F,
                static_cast<float>(cnl::fraction<int>{22, 7})), "cnl::fraction conversion operator");
    }

    namespace test_minus {
        static_assert(identical(
                cnl::make_fraction(-2, 3),
                -cnl::make_fraction(2, 3)), "operator-(cnl::fraction)");
        static_assert(identical(
                cnl::make_fraction(-1, short{-3}),
                -cnl::make_fraction(1, short{-3})), "operator-(cnl::fraction)");
    }

    namespace test_add {
        static_assert(identical(
                cnl::make_fraction(9LL, 9),
                cnl::make_fraction(1LL, 3)+cnl::make_fraction(2, 3)), "operator+(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                cnl::make_fraction(7, 12),
                cnl::make_fraction(1, short{4})+cnl::make_fraction(1, short{3})), "operator+(cnl::fraction, cnl::fraction)");
    }

    namespace test_subtract {
        static_assert(
                identical(
                        cnl::make_fraction(-3LL, 9),
                        cnl::make_fraction(1LL, 3)-cnl::make_fraction(2, 3)),
                "operator-(cnl::fraction, cnl::fraction)");
        static_assert(
                identical(
                        cnl::make_fraction(-1, 12),
                        cnl::make_fraction(1, short{4})-cnl::make_fraction(1, short{3})),
                "operator-(cnl::fraction, cnl::fraction)");
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

    namespace test_compare {
        static_assert(identical(
                true,
                cnl::make_fraction(8892, 19)<cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                false,
                cnl::make_fraction(8892, 19)>cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                true,
                cnl::make_fraction(8892, 19)<=cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                false,
                cnl::make_fraction(8892, 19)>=cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");

        static_assert(identical(
                false,
                cnl::make_fraction(8893, 19)<cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                false,
                cnl::make_fraction(8893, 19)>cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                true,
                cnl::make_fraction(8893, 19)<=cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
        static_assert(identical(
                true,
                cnl::make_fraction(8893, 19)>=cnl::make_fraction(8893, 19)),
                "operator<(cnl::fraction, cnl::fraction)");
    }

    namespace test_abs {
        static_assert(identical(
                cnl::make_fraction(short{1024}, long{360}),
                cnl::abs(cnl::make_fraction(short{1024}, long{360}))), "reduce(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(6, 3),
                cnl::abs(cnl::fraction<>(-6, -3))), "reduce(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(6, 3),
                cnl::abs(cnl::fraction<>(-6, 3))), "reduce(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(6, 3),
                cnl::abs(cnl::fraction<>(6, -3))), "reduce(cnl::fraction)");
    }

    namespace test_reduce {
        static_assert(identical(
                cnl::make_fraction(long{128}, long{45}),
                cnl::reduce(cnl::make_fraction(short{1024}, long{360}))), "reduce(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(-2, -1),
                cnl::reduce(cnl::fraction<>(-6, -3))), "reduce(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(-2, 1),
                cnl::reduce(cnl::fraction<>(-6, 3))), "reduce(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(2, -1),
                cnl::reduce(cnl::fraction<>(6, -3))), "reduce(cnl::fraction)");
    }

    namespace test_canonical {
        static_assert(identical(
                cnl::fraction<>(long{128}, long{45}),
                cnl::canonical(cnl::fraction<>(short{1024}, long{360}))), "canonical(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(2, 1),
                cnl::canonical(cnl::fraction<>(-6, -3))), "canonical(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(-2, 1),
                cnl::canonical(cnl::fraction<>(-6, 3))), "canonical(cnl::fraction)");
        static_assert(identical(
                cnl::fraction<>(-2, 1),
                cnl::canonical(cnl::fraction<>(6, -3))), "canonical(cnl::fraction)");
    }

    TEST(fraction, hash)  // NOLINT
    {
        ASSERT_NE(
                (std::hash<cnl::fraction<>>{}(cnl::fraction<>{1, 2})),
                (std::hash<cnl::fraction<>>{}(cnl::fraction<>{2, 1})));
        ASSERT_EQ(
                (std::hash<cnl::fraction<>>{}(cnl::fraction<>{1, 2})),
                (std::hash<cnl::fraction<>>{}(cnl::fraction<>{12, 24})));
    }
}
