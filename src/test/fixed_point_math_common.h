//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//No include guards: we mean to include this for each case separately

#include <sg14/bits/fixed_point_math.h>
#include <stdint.h>


TEST(math, FPTESTFORMAT){

using fp = sg14::fixed_point<int32_t, FPTESTEXP>;

//Test integer powers
for (int i = 0; i < fp::integer_digits; i++){
			ASSERT_EQ(exp2(fp{i}), fp{1 << i});
}

//Test negative integer powers (which are representable in the format)
if (fp::fractional_digits > 0) {
	for (int i = std::max(-fp::fractional_digits, -(sg14::_fixed_point_impl::shift_left<fp::integer_digits, int32_t>(1)) + 1); i < std::min(0, fp::integer_digits - 1); i++) {
		fp lhs{exp2(fp{i})};
		EXPECT_EQ(lhs,  fp::from_data(1 << (-fp::exponent + i)))
		  << "i = " << i << ", fixed point raw: " << lhs.data() << " should be: " << (1 << (-fp::exponent + i))
		;
	}
}


//Select a number of fractions to test for each integer power
//TODO: think if there are special values that would be most likely to fail
//TODO: it should be possible in a non-routine unit test to test over all
//2^32 values of a 32-bit integer
constexpr std::array<double, 13> fracts{
				static_cast<double>(fp::from_data(1)), //As close to zero as possible
				0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
				0.0001, 0.9999,
				static_cast<double>(fp::from_data((1ll << -fp::exponent) - 1)) //As close to one as possible
			};

for (int i = -fp::fractional_digits; i < fp::integer_digits; i++) {
	for (double frac : fracts) {

		//Build the double so that it's
		//guaranteed to match with the
		//fixed point representation
		//of the exponent,
		//i.e. the input to the function should
		//have the same rounding error as the number
		//being tested
		fp fprep{i + frac};
		double doublerep{fprep};

		//Check for at most 1 LSB error
		fp lhs{exp2(fp{fprep})};
		fp rhs{exp2(doublerep)}; //Will use the double overload
		EXPECT_LE(std::abs(lhs.data() - rhs.data()), 1)
		 << "fail at " << i + frac << ", fixed point raw: " << lhs.data() << " double raw " << rhs.data()
		 ;
		//bit-accurate:: not without a rounding multiply
		//EXPECT_EQ(exp2(fp{fprep}), fp{exp2(doublerep)});
	}
}

//the largest exponent which's result doesn't overflow
auto maximum = fp::from_data(fp{fp::integer_digits}.data() - 1);

//The next-to-smallest exponent whose result doesn't overflow
//(The very smallest was already tested with the integer exponents)
auto minimum = fp::from_data(fp{-fp::fractional_digits}.data() + 1);

double doublerep{maximum};
double doublerepmini{minimum};

EXPECT_LE(std::abs(exp2(maximum).data() - fp{exp2(doublerep)}.data()), 1)
 << "fixed point raw: " << exp2(maximum).data() << ", double raw: " << fp{exp2(doublerep)}.data();

EXPECT_EQ(exp2(minimum), fp{exp2(doublerepmini)});



}

