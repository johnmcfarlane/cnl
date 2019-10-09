
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/overflow_integer.h>
#include <cnl/scaled_integer.h>

#define TEST_TRAPPING_OVERFLOW_INTEGER
#define TEST_LABEL trapping_integer_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// integer types used as scaled_integer Rep type

using test_int = cnl::overflow_integer<int, cnl::trapping_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform scaled_integer tests with this type of scaled_integer specialization

#include "../scaled_integer_common.h"

////////////////////////////////////////////////////////////////////////////////
// trapping_integer-specific exceptions tests

#if !defined(CNL_UNREACHABLE_UB_ENABLED)
TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), scale_down)  // NOLINT
{
    auto scale_down_fn = cnl::_impl::scale<-8, 2, uint16>;
    ASSERT_DEATH(scale_down_fn((uint8) 0x1234), "positive overflow");
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), scale_up)  // NOLINT
{
    auto scale_up_fn = cnl::_impl::scale<8, 2, uint16>;
    ASSERT_DEATH(scale_up_fn((uint8) 0x1234), "positive overflow");
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), assignment)  // NOLINT
{
    using fp_type = scaled_integer<int8, cnl::power<-7>>;
    ASSERT_DEATH(fp_type(1), "positive overflow");
}
#endif
