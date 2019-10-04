
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/scaled_integer.h>
#include <cnl/overflow_integer.h>

#define TEST_THROWING_OVERFLOW_INTEGER
#define TEST_LABEL throwing_integer_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// integer types used as scaled_integer Rep type

using test_int = cnl::overflow_integer<int, cnl::throwing_overflow_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform scaled_integer tests with this type of scaled_integer specialization

#include "../scaled_integer_common.h"

#include <gtest/gtest.h>

////////////////////////////////////////////////////////////////////////////////
// throwing_integer-specific exceptions tests


#if defined(CNL_EXCEPTIONS_ENABLED)

TEST(throwing_integer_overflow_exception, narrow)
{
    ASSERT_THROW((uint8) 0x1234, std::overflow_error);
}

TEST(throwing_integer_overflow_exception, shift_left)
{
    auto scale = cnl::scale<30, 2, test_int>{};
    ASSERT_THROW((void)scale(2), std::overflow_error);
}

TEST(throwing_integer_overflow_exception, assignment)
{
    using fp_type = scaled_integer<int8, cnl::power<-7>>;
    ASSERT_THROW((void)fp_type(1), std::overflow_error);
}

#endif
