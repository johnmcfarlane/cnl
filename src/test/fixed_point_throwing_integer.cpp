
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <integer.h>
#include <fixed_point.h>

#if defined(_SG14_FIXED_POINT_EXCEPTIONS_ENABLED)
#define TEST_THROWING_OVERFLOW
#define TEST_LABEL throwing_integer_
#else
#define TEST_NATIVE_OVERFLOW
#define TEST_LABEL hobbled_throwing_integer_
#endif

////////////////////////////////////////////////////////////////////////////////
// integer types used as fixed_point Repr type

using test_unsigned = sg14::throwing_integer<unsigned>;
using test_signed = sg14::throwing_integer<signed>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

#include "fixed_point_common.h"

////////////////////////////////////////////////////////////////////////////////
// throwing_integer-specific exceptions tests

#if defined(TEST_THROWING_OVERFLOW)
TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_right)
{
	constexpr auto shift_right_fn = _impl::shift_right<8, uint16, uint8>;
	ASSERT_THROW(shift_right_fn((uint8)0x1234), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), shift_left)
{
	constexpr auto shift_right_fn = _impl::shift_left<-8, uint16, uint8>;
	ASSERT_THROW(shift_right_fn((uint8)0x1234), std::overflow_error);
}

TEST(TOKENPASTE2(TEST_LABEL, overflow_exception), assignment)
{
	using fp_type = fixed_point<int8, -7>;
	ASSERT_THROW(fp_type(1), std::overflow_error);
}
#endif
