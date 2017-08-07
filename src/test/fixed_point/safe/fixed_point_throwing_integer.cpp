
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/safe_integer.h>

#define TEST_THROWING_OVERFLOW
#define TEST_LABEL throwing_integer_

#if defined(_MSC_VER)
#define TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING
#define TEST_IGNORE_MSVC_INTERNAL_ERRORS
#endif

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Rep type

using test_int = cnl::safe_integer<>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "../fixed_point_common.h"

////////////////////////////////////////////////////////////////////////////////
// throwing_integer-specific exceptions tests


#if defined(TEST_THROWING_OVERFLOW) && defined(CNL_EXCEPTIONS_ENABLED)

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_right)
{
    auto shift_left_fn = shift_left<-8, uint16, uint8>;
    ASSERT_THROW(shift_left_fn((uint8) 0x1234), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_left)
{
    auto shift_left_fn = shift_left<-8, uint16, uint8>;
    ASSERT_THROW(shift_left_fn((uint8) 0x1234), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), assignment)
{
    using fp_type = fixed_point<int8, -7>;
    ASSERT_THROW(fp_type(1), std::overflow_error);
}

#endif
