#include <fixed_point_utils.h>

#include "test_utils.h"

namespace
{
	template <typename FP>
	constexpr auto magnitude(FP const & x, FP const & y, FP const & z)
	-> decltype(trunc_sqrt(trunc_add(trunc_square(x), trunc_square(y), trunc_square(z))))
	{
		return trunc_sqrt(trunc_add(trunc_square(x), trunc_square(y), trunc_square(z)));
	}
}

void proposal_test()
{
	using namespace std;
	using namespace sg14;

	////////////////////////////////////////////////////////////////////////////////
	// Tests of Examples in Proposal 

	// Class Template

	static_assert(fixed_point<uint16_t>::integer_digits == 8, "Incorrect information in proposal section, Class Template");
	static_assert(fixed_point<uint16_t>::fractional_digits == 8, "Incorrect information in proposal section, Class Template");

	static_assert(static_cast<float>(fixed_point<int32_t, -1>(10.5)) == 10.5, "Incorrect information in proposal section, Class Template");

	static_assert(static_cast<float>(fixed_point<uint8_t, -8>(0)) == 0, "Incorrect information in proposal section, Class Template");
	static_assert(static_cast<float>(fixed_point<uint8_t, -8>(.999999)) < 1, "Incorrect information in proposal section, Class Template");
	static_assert(static_cast<float>(fixed_point<uint8_t, -8>(.999999)) > .99, "Incorrect information in proposal section, Class Template");

	static_assert(fixed_point<>::fractional_digits == _impl::num_bits<int>() / 2, "Incorrect information in proposal section, Class Template");

	// Conversion

	auto conversion_lhs = make_ufixed<4, 4>(.006);
	auto conversion_rhs = make_ufixed<4, 4>(0);
	static_assert(is_same<decltype(conversion_lhs), decltype(conversion_rhs)>::value, "Incorrect information in proposal section, Conversion");
	ASSERT_EQUAL(conversion_lhs, conversion_rhs);

	// Arithmetic Operators (Overflow)
	static_assert(static_cast<int>(make_fixed<4, 3>(15) + make_fixed<4, 3>(1)) != 16, "Incorrect information in proposal section, Overflow");

	// Arithmetic Operators (Underflow)
	static_assert(static_cast<float>(make_fixed<7, 0>(15) / make_fixed<7, 0>(2)) == 7, "Incorrect information in proposal section, Underflow");

	// Type Promotion
	auto type_promotion = promote(make_fixed<5, 2>(15.5));
	static_assert(is_same<decltype(type_promotion), make_fixed<11, 4>>::value, "Incorrect information in proposal section, Type Promotion and Demotion Functions");
	ASSERT_EQUAL(static_cast<float>(type_promotion), 15.5);

	// Named Arithmetic Functions
	auto sq = trunc_multiply(make_ufixed<4, 4>(15.9375), make_ufixed<4, 4>(15.9375));
	ASSERT_EQUAL(static_cast<double>(sq), 254);

	auto most_negative = make_fixed<7, 0>(-128);
	ASSERT_EQUAL(static_cast<int>(most_negative), -128);
	ASSERT_EQUAL(static_cast<int>(trunc_square(promote(most_negative))), 16384);
	auto square = trunc_square(most_negative);
	static_assert(is_same<decltype(square), fixed_point<uint8_t, 6>>::value, "wrong type mentioned in proposal");
	ASSERT_EQUAL(static_cast<int>(square), 0);

	// Underflow
	static_assert(static_cast<int>(trunc_square(make_ufixed<8, 0>(15))) != 15 * 15, "wrong behavior reported in 'Overflow and Underflow' section");

	// Examples
	static_assert(static_cast<double>(magnitude(
		make_ufixed<4, 12>(1),
		make_ufixed<4, 12>(4),
		make_ufixed<4, 12>(9))) == 9.890625, "unexpected result from magnitude");

	static fixed_point<> zero;
	ASSERT_EQUAL(zero, fixed_point<>(0));

	// Bounded Integers
	make_ufixed<2, 6> three(3);
	auto n = trunc_square(trunc_square(three));
	ASSERT_EQUAL(static_cast<int>(n), 81);
	static_assert(is_same<decltype(n), make_ufixed<8, 0>>::value, "bad assumption about type in 'Bounded Integers' section");
	ASSERT_EQUAL(static_cast<int>(make_ufixed<7, 1>(81)), 81);
}
