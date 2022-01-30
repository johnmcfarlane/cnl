
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/narrow_cast.h>

#include <cnl/_impl/cstdint/types.h>
#include <test.h>

namespace test_narrow_cast {
    static_assert(identical(
            std::int16_t{-32768},
            cnl::_impl::narrow_cast<std::int16_t>(-32768)));
    static_assert(identical(
            std::int16_t{32767},
            cnl::_impl::narrow_cast<std::int16_t>(32767)));

    static_assert(identical(
            std::uint16_t{0},
            cnl::_impl::narrow_cast<std::uint16_t>(0)));
    static_assert(identical(
            std::uint16_t{65535},
            cnl::_impl::narrow_cast<std::uint16_t>(65535)));

    static_assert(identical(
            3.25F,
            cnl::_impl::narrow_cast<float>(3.25L)));
    static_assert(identical(
            3.25L,
            cnl::_impl::narrow_cast<long double>(3.25F)));

    static_assert(identical(
            static_cast<long double>(static_cast<float>(0.1L)),
            cnl::_impl::narrow_cast<long double>(0.1F)));

    TEST(narrow_cast, trapping)  // NOLINT
    {
#if defined(CNL_DEBUG)
        ASSERT_DEATH((void)cnl::_impl::narrow_cast<std::int16_t>(-32769), "");  // NOLINT
        ASSERT_DEATH((void)cnl::_impl::narrow_cast<std::int16_t>(32768), "");  // NOLINT

        ASSERT_DEATH((void)cnl::_impl::narrow_cast<std::uint16_t>(-1), "");  // NOLINT
        ASSERT_DEATH((void)cnl::_impl::narrow_cast<std::uint16_t>(65536), "");  // NOLINT

        ASSERT_DEATH((void)cnl::_impl::narrow_cast<float>(0.1L), "");  // NOLINT
#endif
    }
}
