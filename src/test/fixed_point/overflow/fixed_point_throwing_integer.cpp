
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/overflow_integer.h>

#define TEST_THROWING_OVERFLOW
#define TEST_LABEL throwing_integer_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = cnl::overflow_integer<>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"

////////////////////////////////////////////////////////////////////////////////
// throwing_integer-specific exceptions tests


#if defined(CNL_EXCEPTIONS_ENABLED)

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_right)
{
    auto shift_left_fn = cnl::_impl::shift<-8, 2, uint16>;
    ASSERT_THROW(shift_left_fn((uint8) 0x1234), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_left)
{
    auto shift_left_fn = cnl::_impl::shift<-8, 2, uint16>;
    ASSERT_THROW(shift_left_fn((uint8) 0x1234), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), assignment)
{
    using fp_type = fixed_point<int8, -7>;
    ASSERT_THROW(fp_type(1), std::overflow_error);
}

#endif
