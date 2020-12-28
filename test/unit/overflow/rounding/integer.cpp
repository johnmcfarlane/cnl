
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/overflow_integer.h>
#include <cnl/rounding_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    template<
            typename Rep = int, cnl::overflow_tag OverflowTag = cnl::undefined_overflow_tag,
            cnl::rounding_tag RoundingTag = cnl::nearest_rounding_tag>
    using rounding_overflow_integer =
            cnl::overflow_integer<cnl::rounding_integer<Rep, RoundingTag>, OverflowTag>;

    namespace test_is_same_tag_family {
        static_assert(
                cnl::_impl::is_same_tag_family<
                        cnl::nearest_rounding_tag, cnl::nearest_rounding_tag>::value);
        static_assert(
                cnl::_impl::is_same_tag_family<
                        cnl::native_rounding_tag, cnl::nearest_rounding_tag>::value);

        static_assert(
                !cnl::_impl::is_same_tag_family<
                        cnl::native_overflow_tag, cnl::native_rounding_tag>::value);
        static_assert(
                !cnl::_impl::is_same_tag_family<
                        cnl::native_rounding_tag, cnl::native_overflow_tag>::value);
        static_assert(
                !cnl::_impl::is_same_tag_family<
                        cnl::saturated_overflow_tag, cnl::nearest_rounding_tag>::value);
        static_assert(
                !cnl::_impl::is_same_tag_family<
                        cnl::nearest_rounding_tag, cnl::saturated_overflow_tag>::value);
    }

    namespace test_construct {
        namespace from_int {
            constexpr auto ex{rounding_overflow_integer<
                    short, cnl::undefined_overflow_tag, cnl::native_rounding_tag>{42}};
            constexpr auto in{42};
            constexpr auto ac{rounding_overflow_integer<
                    short, cnl::undefined_overflow_tag, cnl::native_rounding_tag>{in}};
            static_assert(identical(ex, ac));
        }

        namespace from_rounding_integer {
            constexpr auto ex{rounding_overflow_integer<
                    short, cnl::undefined_overflow_tag, cnl::native_rounding_tag>{42}};
            constexpr auto in{cnl::rounding_integer<short, cnl::native_rounding_tag>{42}};
            constexpr auto ac{rounding_overflow_integer<
                    short, cnl::undefined_overflow_tag, cnl::native_rounding_tag>{in}};
            static_assert(identical(ex, ac));
        }
    }

    namespace test_from_rep {
        namespace from_rounding_integer {
            constexpr auto ex{rounding_overflow_integer<
                    short, cnl::undefined_overflow_tag, cnl::native_rounding_tag>{42}};
            constexpr auto in{cnl::rounding_integer<short, cnl::native_rounding_tag>{42}};
            constexpr auto ac{cnl::from_rep<
                    rounding_overflow_integer<>,
                    cnl::rounding_integer<short, cnl::native_rounding_tag>>{}(in)};
            static_assert(identical(ex, ac));
        }

        namespace from_overflow_integer {
            constexpr auto ex{cnl::overflow_integer<short, cnl::native_overflow_tag>{42}};
            constexpr auto in{cnl::overflow_integer<short, cnl::native_overflow_tag>{42}};
            constexpr auto ac{cnl::from_rep<
                    rounding_overflow_integer<
                            short, cnl::native_overflow_tag, cnl::nearest_rounding_tag>,
                    cnl::overflow_integer<short, cnl::native_overflow_tag>>{}(in)};
            static_assert(identical(ex, ac));
        }
    }
}
