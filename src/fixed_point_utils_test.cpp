#include <fixed_point_utils.h>

#include "test_utils.h"

using namespace std;
using namespace sg14;

void fixed_point_utils_test()
{
	////////////////////////////////////////////////////////////////////////////////
	// sin

	ASSERT_EQUAL(static_cast<float>(sin(fixed_point<std::uint8_t, -6>(0))), 0);
	ASSERT_EQUAL(static_cast<float>(sin(fixed_point<std::int16_t, -13>(3.1415926))), 0);
	ASSERT_EQUAL(static_cast<double>(sin(fixed_point<std::uint16_t, -14>(3.1415926 / 2))), 1);
	ASSERT_EQUAL(static_cast<float>(sin(fixed_point<std::int32_t, -24>(3.1415926 * 7. / 2.))), -1);
	ASSERT_EQUAL(static_cast<float>(sin(fixed_point<std::int32_t, -28>(3.1415926 / 4))), .707106769f);
	ASSERT_EQUAL(static_cast<double>(sin(fixed_point<std::int16_t, -10>(-3.1415926 / 3))), -.865234375);

	////////////////////////////////////////////////////////////////////////////////
	// cos

	ASSERT_EQUAL(static_cast<float>(cos(fixed_point<std::uint8_t, -6>(0))), 1);
	ASSERT_EQUAL(static_cast<float>(cos(fixed_point<std::int16_t, -13>(3.1415926))), -1);
	ASSERT_EQUAL(static_cast<double>(cos(fixed_point<std::uint16_t, -14>(3.1415926 / 2))), 0);
	ASSERT_EQUAL(static_cast<float>(cos(fixed_point<std::int32_t, -20>(3.1415926 * 7. / 2.))), 0);
	ASSERT_EQUAL(static_cast<float>(cos(fixed_point<std::int32_t, -28>(3.1415926 / 4))), .707106829f);
	ASSERT_EQUAL(static_cast<double>(cos(fixed_point<std::int16_t, -10>(-3.1415926 / 3))), .5);
}

////////////////////////////////////////////////////////////////////////////////
// sg14::abs

static_assert(static_cast<int>(abs(make_fixed<7, 0>(66))) == 66, "sg14::abs test failed");
static_assert(static_cast<int>(abs(make_fixed<7, 0>(-123))) == 123, "sg14::abs test failed");
static_assert(static_cast<std::int64_t>(abs(make_fixed<63, 0>(9223372036854775807))) == 9223372036854775807, "sg14::abs test failed");
static_assert(static_cast<std::int64_t>(abs(make_fixed<63, 0>(-9223372036854775807))) == 9223372036854775807, "sg14::abs test failed");
static_assert(static_cast<int>(abs(make_fixed<7, 8>(-5))) == 5, "sg14::abs test failed");
