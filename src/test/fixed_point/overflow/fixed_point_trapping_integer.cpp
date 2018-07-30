
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/overflow_integer.h>

#define TEST_TRAPPING_OVERFLOW_INTEGER
#define TEST_LABEL trapping_integer_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = cnl::overflow_integer<int, cnl::trapping_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"

////////////////////////////////////////////////////////////////////////////////
// trapping_integer-specific exceptions tests

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), scale_down)
{
    auto scale_down_fn = cnl::_impl::scale<-8, 2, uint16>;
    ASSERT_DEATH(scale_down_fn((uint8) 0x1234), "positive overflow");
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), scale_up)
{
    auto scale_up_fn = cnl::_impl::scale<8, 2, uint16>;
    ASSERT_DEATH(scale_up_fn((uint8) 0x1234), "positive overflow");
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), assignment)
{
    using fp_type = fixed_point<int8, -7>;
    ASSERT_DEATH(fp_type(1), "positive overflow");
}
