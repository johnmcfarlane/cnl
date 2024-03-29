
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::_impl::identical;

namespace cnl {
    // rounding elastic integer
    template<
            int IntegerDigits, rounding_tag RoundingTag = cnl::_impl::tag_of_t<rounding_integer<>>,
            class Narrowest = int>
    using rounding_elastic_integer =
            rounding_integer<elastic_integer<IntegerDigits, Narrowest>, RoundingTag>;

    template<
            rounding_tag RoundingTag = cnl::_impl::tag_of_t<rounding_integer<>>, class Narrowest = int,
            class Input = int>
    [[nodiscard]] auto constexpr make_rounding_elastic(Input const& input)
    {
        return rounding_elastic_integer<
                std::numeric_limits<Input>::digits, RoundingTag,
                Narrowest>{input};
    }
}

namespace {
    using cnl::rounding_elastic_integer;
    using std::is_same;

    namespace default_parameters {
        using cnl::elastic_integer;
        using cnl::rounding_integer;
        using cnl::_impl::rep_of_t;

        static_assert(
                is_same<rep_of_t<rep_of_t<rounding_elastic_integer<1>>>, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_is_wrapper {
        static_assert(
                cnl::_impl::is_wrapper<cnl::elastic_integer<>> && !cnl::_impl::is_wrapper<int>);
    }

    namespace test_from_rep {
        static_assert(
                identical(
                        rounding_elastic_integer<16>{0},
                        cnl::from_rep<rounding_elastic_integer<24>, cnl::elastic_integer<16>>{}(
                                cnl::elastic_integer<16>(0))),
                "from_rep<rounding_elastic_integer>");
        static_assert(
                std::is_same_v<
                        cnl::elastic_integer<7, int>,
                        cnl::_impl::set_rep_t<cnl::elastic_integer<7, int>, int>>);
        static_assert(
                identical(
                        cnl::elastic_integer<7, int>{42},
                        cnl::from_rep<cnl::elastic_integer<7>, int>{}(42)));
        static_assert(
                identical(
                        rounding_elastic_integer<16>{0},
                        cnl::_impl::from_rep<rounding_elastic_integer<24>>(
                                cnl::elastic_integer<16>(0))),
                "from_rep<rounding_elastic_integer>");
    }

    namespace test_make_rounding_elastic {
        static_assert(
                identical(
                        cnl::make_rounding_elastic(std::int16_t{7}), rounding_elastic_integer<15>{7}));
    }

    namespace test_multiply {
        static_assert(
                identical(
                        rounding_elastic_integer<3>{7} * rounding_elastic_integer<4>{10},
                        rounding_elastic_integer<7>{70}),
                "rounding_elastic_integer operator*");
        static_assert(
                identical(
                        cnl::_impl::wrapper<
                                cnl::elastic_integer<37, int>, cnl::nearest_rounding_tag>{54},
                        cnl::custom_operator<
                                cnl::_impl::multiply_op,
                                cnl::op_value<cnl::_impl::wrapper<
                                        cnl::elastic_integer<24, int>, cnl::nearest_rounding_tag>>,
                                cnl::op_value<cnl::elastic_integer<13, int>>>{}(
                                cnl::_impl::wrapper<
                                        cnl::elastic_integer<24, int>, cnl::nearest_rounding_tag>{
                                        6},
                                cnl::elastic_integer<13, int>{9})),
                "binary_arithmetic_operator<multiply_op, native_tag, native_tag, rounding_elastic_integer, "
                "elastic_integer>");
    }

    namespace test_from_value {
        static_assert(
                identical(
                        rounding_elastic_integer<3>{5},
                        cnl::from_value<rounding_elastic_integer<24>, cnl::constant<5>>{}(
                                cnl::constant<5>{})),
                "from_value<rounding_elastic_integer>");
        static_assert(
                identical(
                        rounding_elastic_integer<3>{5},
                        cnl::_impl::from_value<rounding_elastic_integer<24>>(cnl::constant<5>{})),
                "from_value<rounding_elastic_integer>");
    }

    namespace test_integer_scale {
        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24>{256},
                        cnl::scale<8, 2, cnl::rounding_elastic_integer<16>>{}(
                                cnl::rounding_elastic_integer<16>{1})),
                "cnl::scale<cnl::rounding_elastic_integer>");

        static_assert(
                identical(
                        cnl::rounding_elastic_integer<2>{2},
                        cnl::scale<-1, 2, cnl::rounding_elastic_integer<2>>{}(
                                cnl::rounding_elastic_integer<2>{3})),
                "cnl::scale<cnl::rounding_elastic_integer>");
        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24>{1},
                        cnl::scale<-8, 2, cnl::rounding_elastic_integer<24>>{}(
                                cnl::rounding_elastic_integer<24>{383})),
                "cnl::scale<cnl::rounding_elastic_integer>");
        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24>{2},
                        cnl::scale<-8, 2, cnl::rounding_elastic_integer<24>>{}(
                                cnl::rounding_elastic_integer<24>{384})),
                "cnl::scale<cnl::rounding_elastic_integer>");

        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24, cnl::nearest_rounding_tag>{2},
                        cnl::scale<
                                -8, 2,
                                cnl::rounding_elastic_integer<24, cnl::nearest_rounding_tag>>{}(
                                cnl::rounding_elastic_integer<24, cnl::nearest_rounding_tag>{511})),
                "cnl::scale<cnl::rounding_elastic_integer>");

        static_assert(
                identical(
                        cnl::rounding_elastic_integer<1, cnl::native_rounding_tag>{0},
                        cnl::scale<
                                -1, 2,
                                cnl::rounding_elastic_integer<1, cnl::native_rounding_tag>>{}(
                                cnl::rounding_elastic_integer<1, cnl::native_rounding_tag>{-1})),
                "cnl::scale<cnl::rounding_elastic_integer>");
    }

    namespace test_integer_impl_scale {
        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24>{256},
                        cnl::_impl::scale<8, 2>(cnl::rounding_elastic_integer<16>{1})),
                "cnl::_impl::scale<cnl::rounding_elastic_integer>");

        static_assert(
                identical(
                        cnl::rounding_elastic_integer<2>{2},
                        cnl::_impl::scale<-1>(cnl::rounding_elastic_integer<2>{3})),
                "cnl::_impl::scale<cnl::rounding_elastic_integer>");
        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24>{1},
                        cnl::_impl::scale<-8, 2>(cnl::rounding_elastic_integer<24>{383})),
                "cnl::_impl::scale<cnl::rounding_elastic_integer>");
        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24>{2},
                        cnl::_impl::scale<-8, 2>(cnl::rounding_elastic_integer<24>{384})),
                "cnl::_impl::scale<cnl::rounding_elastic_integer>");

        static_assert(
                identical(
                        cnl::rounding_elastic_integer<24, cnl::nearest_rounding_tag>{2},
                        cnl::_impl::scale<-8, 2>(
                                cnl::rounding_elastic_integer<24, cnl::nearest_rounding_tag>{511})),
                "cnl::_impl::scale<cnl::rounding_elastic_integer>");

        static_assert(
                identical(
                        cnl::rounding_elastic_integer<1, cnl::native_rounding_tag>{0},
                        cnl::_impl::scale<-1>(
                                cnl::rounding_elastic_integer<1, cnl::native_rounding_tag>{-1})),
                "cnl::_impl::scale<cnl::rounding_elastic_integer>");
    }
}
