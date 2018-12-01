
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/duplex_integer/operators.h>

#include <cnl/_impl/duplex_integer/ctors.h>
#include <cnl/_impl/duplex_integer/operators.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace test_add {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234+0x7625},
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234}
                                +cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234+0x7625},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                +cnl::_impl::duplex_integer<int, unsigned>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{123+654},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{123}
                                +cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{654}),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{39487298182LL+8238723LL},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{39487298182LL}
                                +cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{8238723LL}),
                "");
#endif
    }

    namespace test_subtract {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234-0x7625},
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234}
                                -cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x7625-0x1234},
                        cnl::_impl::duplex_integer<int, unsigned>{0x7625}
                                -cnl::_impl::duplex_integer<int, unsigned>{0x1234}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234LL-0x7625LL},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                -cnl::_impl::duplex_integer<int, unsigned>{0x7625}),
                "");
    }

    namespace test_multiply {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234*0x7625},
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x1234}
                                *cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234*0x7625},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                *cnl::_impl::duplex_integer<int, unsigned>{0x7625}),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7}
                                *cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{13}),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7}
                                *cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{13}),
                "");
#endif
    }

    namespace test_long_multiply {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{}(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{7},
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>>{}(
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7},
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{}(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{7},
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>, cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>>{}(
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{7},
                                cnl::_impl::duplex_integer<cnl::uint32, cnl::uint32>{13}
                        )),
                "");
#if defined(CNL_INT128_ENABLED)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{}(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{7},
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>, cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>>{}(
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7},
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{}(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{7},
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{13}
                        )),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>, cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{7*13},
                        cnl::_impl::long_multiply<cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>>{}(
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{7},
                                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{13}
                        )),
                "");
#endif
    }

    namespace test_divide {
#if (__cpp_constexpr >= 201304L)
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned>{0x12},
                        cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                                /cnl::_impl::duplex_integer<int, unsigned>{0x100}),
                "");
#endif

        TEST(duplex_integer, divide)
        {
            auto expected = cnl::_impl::duplex_integer<int, unsigned>{0x12};
            auto actual = cnl::_impl::duplex_integer<int, unsigned>{0x1234}
                    /cnl::_impl::duplex_integer<int, unsigned>{0x100};
            ASSERT_EQ(expected, actual);
        }
    }

    namespace test_equal {
        static_assert(
                cnl::_impl::duplex_integer<int, unsigned>{0}==cnl::_impl::duplex_integer<int, unsigned>{},
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}==
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123}==
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123}==
                        cnl::_impl::duplex_integer<short, unsigned short>{-124}),
                "");
    }

    namespace test_not_equal {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0}!=cnl::_impl::duplex_integer<int, unsigned>{}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}!=
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{512}!=
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{1024},
                "");
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123}!=
                        cnl::_impl::duplex_integer<short, unsigned short>{-124},
                "");

        static_assert(cnl::_impl::duplex_integer<short, unsigned short>{123}!=cnl::int8{-124}, "");
        static_assert(cnl::int64{-12356}<cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{12356}, "");
    }

    namespace test_less {
        static_assert(
                !(cnl::_impl::duplex_integer<int, unsigned>{0}<cnl::_impl::duplex_integer<int, unsigned>{}),
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}<
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123}<
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<short, unsigned short>{123}<
                        cnl::_impl::duplex_integer<short, unsigned short>{-124}),
                "");
    }

    namespace test_greater_than_or_equal {
        static_assert(
                cnl::_impl::duplex_integer<int, unsigned>{0}>=cnl::_impl::duplex_integer<int, unsigned>{},
                "");
        static_assert(
                cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123}>=
                        cnl::_impl::duplex_integer<cnl::uint64, cnl::uint64>{123},
                "");
        static_assert(
                !(cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{123}>=
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{124}),
                "");
        static_assert(
                cnl::_impl::duplex_integer<short, unsigned short>{123}>=
                        cnl::_impl::duplex_integer<short, unsigned short>{-124},
                "");
    }

    namespace test_bitwise_and_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{0x1234 & 0x5858},
                        cnl::_impl::binary_operator<
                                cnl::_impl::bitwise_and_op,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>>{}(0x1234, 0x5858)),
                "");
    }

    namespace test_shift_left_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<signed, unsigned>,
                                cnl::_impl::duplex_integer<signed, unsigned>>{}(0, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>{0x2468ACE0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>,
                                cnl::_impl::duplex_integer<cnl::int16, cnl::uint16>>{}(0x12345670, 1)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{1},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<signed, unsigned>,
                                cnl::_impl::duplex_integer<signed, unsigned>>{}(1, 0)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{246},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<signed, unsigned>,
                                cnl::_impl::duplex_integer<signed, unsigned>>{}(
                                cnl::_impl::duplex_integer<signed, unsigned>{123},
                                cnl::_impl::duplex_integer<signed, unsigned>{1})),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>{0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_right_op,
                                cnl::_impl::duplex_integer<cnl::uint8, cnl::uint8>,
                                int>{}(0, 9)), "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, cnl::_impl::duplex_integer<unsigned, unsigned>>{0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<signed, cnl::_impl::duplex_integer<unsigned, unsigned>>,
                                int>{}(0, 1)),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<int, unsigned int>{1LL << 34},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<int, unsigned int>,
                                int>{}(1, 34)),
                "");

        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<int, unsigned int>, cnl::_impl::duplex_integer<unsigned int, unsigned int>>{
                                1LL << 34},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_left_op,
                                cnl::_impl::duplex_integer<cnl::_impl::duplex_integer<int, unsigned int>, cnl::_impl::duplex_integer<unsigned int, unsigned int> >,
                                int>{}(1, 34)),
                "");
    }

    namespace test_shift_right_op {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>{0x8394uLL >> 0},
                        cnl::_impl::binary_operator<
                                cnl::_impl::shift_right_op,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>,
                                cnl::_impl::duplex_integer<cnl::int8, cnl::uint8>>{}(0x8394uLL, 0)),
                "");
    }

    namespace test_shift_left {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{246},
                        cnl::_impl::duplex_integer<signed, unsigned>{123} << 1),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{246},
                        cnl::_impl::duplex_integer<signed, unsigned>{123}
                                << cnl::_impl::duplex_integer<signed, unsigned>{1}),
                "");
    }

    namespace test_shift_right {
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{123},
                        cnl::_impl::duplex_integer<signed, unsigned>{246} >> 1),
                "");
        static_assert(
                identical(
                        cnl::_impl::duplex_integer<signed, unsigned>{123},
                        cnl::_impl::duplex_integer<signed, unsigned>{246}
                                >> cnl::_impl::duplex_integer<signed, unsigned>{1}),
                "");
    }
}
