
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/overflow_integer.h>

#define TEST_THROWING_OVERFLOW_INTEGER
#define TEST_LABEL throwing_integer_

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = cnl::overflow_integer<int, cnl::throwing_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"

////////////////////////////////////////////////////////////////////////////////
// throwing_integer-specific exceptions tests


#if defined(CNL_EXCEPTIONS_ENABLED)

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), narrow)
{
    ASSERT_THROW((uint8) 0x1234, std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_left)
{
    auto scale = cnl::scale<30, 2, test_int>{};
    ASSERT_THROW(scale(2), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), assignment)
{
    using fp_type = fixed_point<int8, -7>;
    ASSERT_THROW(fp_type(1), std::overflow_error);
}

#endif
