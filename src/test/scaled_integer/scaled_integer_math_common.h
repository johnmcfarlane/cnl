
//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//No include guards: we mean to include this for each case separately

#include <cnl/_impl/scaled_integer/math.h>

#include <algorithm>
#include <array>
#include <stdint.h>


TEST(math, FPTESTFORMAT) {
    using fp = cnl::scaled_integer<int32_t, cnl::power<FPTESTEXP>>;

    //Test integer powers
    for (int i = 0; i < cnl::_impl::integer_digits<fp>::value; i++) {
        ASSERT_EQ(exp2(fp{ static_cast<double>(i) }), fp{ static_cast<double>(1 << i) });
    }

    //Test negative integer powers (which are representable in the format)
#if (FPTESTEXP < 0)
    for (int i = std::max(-cnl::_impl::fractional_digits<fp>::value, -(cnl::_impl::scale<cnl::_impl::integer_digits<fp>::value, 2, int32_t>(1)) + 1); i < std::min(0, cnl::_impl::integer_digits<fp>::value - 1); i++) {
        fp lhs{ exp2(fp{ static_cast<double>(i) }) };
        EXPECT_EQ(lhs, cnl::_impl::from_rep<fp>(1 << (-FPTESTEXP + i)))
            << "i = " << i << ", fixed point raw: " << cnl::_impl::to_rep(lhs) << " should be: " << (1 << (-FPTESTEXP + i))
            ;
    }
#endif

    //Select a number of fractions to test for each integer power
    //TODO: think if there are special values that would be most likely to fail
    //TODO: it should be possible in a non-routine unit test to test over all
    //2^32 values of a 32-bit integer
    constexpr std::array<double, 13> fracts{ {
        static_cast<double>(cnl::numeric_limits<fp>::min()), //As close to zero as possible
        0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
            cnl::_impl::min(1., static_cast<double>(cnl::numeric_limits<fp>::max())) //As close to one as possible
    } };

    for (int i = -cnl::_impl::fractional_digits<fp>::value; i < cnl::_impl::integer_digits<fp>::value; i++) {
        for (double frac : fracts) {

            //Build the double so that it's
            //guaranteed to match with the
            //fixed point representation
            //of the exponent,
            //i.e. the input to the function should
            //have the same rounding error as the number
            //being tested
            fp fprep{ i + frac };
            double doublerep{ fprep };

            //Check for at most 1 LSB error
            fp lhs{ exp2(fp{ fprep }) };
            fp rhs{ exp2(doublerep) }; //Will use the double overload
            EXPECT_LE(std::abs(cnl::_impl::to_rep(lhs)-cnl::_impl::to_rep(rhs)), 1)
                            << "fail at " << i+frac << ", fixed point raw: " << cnl::_impl::to_rep(lhs)
                            << " double raw " << cnl::_impl::to_rep(rhs)
                ;
            //bit-accurate:: not without a rounding multiply
            //EXPECT_EQ(exp2(fp{fprep}), fp{exp2(doublerep)});
        }
    }

    //the largest exponent which's result doesn't overflow
    auto maximum = cnl::_impl::from_rep<fp>(
            cnl::_impl::to_rep(fp{static_cast<double>(cnl::_impl::integer_digits<fp>::value)})-1);

    //The next-to-smallest exponent whose result doesn't overflow
    //(The very smallest was already tested with the integer exponents)
    auto minimum = cnl::_impl::from_rep<fp>(
            cnl::_impl::to_rep(fp{static_cast<double>(-cnl::_impl::fractional_digits<fp>::value)})+1);

    double doublerep{ maximum };
    double doublerepmini{ minimum };

    EXPECT_LE(std::abs(cnl::_impl::to_rep(exp2(maximum))-cnl::_impl::to_rep(fp{exp2(doublerep)})), 1)
                    << "fixed point raw: " << cnl::_impl::to_rep(exp2(maximum))
                    << ", double raw: " << cnl::_impl::to_rep(fp{exp2(doublerep)});

    EXPECT_EQ(exp2(minimum), fp{ exp2(doublerepmini) });
}

