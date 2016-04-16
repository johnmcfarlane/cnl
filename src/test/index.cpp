//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// example code presented on the front page of the Doxygen documentation


////////////////////////////////////////////////////////////////////////////////
//! [declaration example]
#include <sg14/fixed_point.h>
#include <iostream>

using namespace sg14;
using namespace std;

void declaration_example()
{
    // x is represented by an int and scaled down by 1 bit
    auto x = fixed_point<int, -1>{3.5};

    // another way to specify a fixed-point type is with make_fixed or make_ufixed
    static_assert(is_same<decltype(x), make_fixed<30, 1>>::value, "");  // assumes that int is 32-bit

    // under the hood, x stores a whole number
    cout << x.data() << endl;  // "7"

    // but it multiplies that whole number by 2^-1 to produce a real number
    cout << x << endl;  // "3.5"

    // like an int, x has limited precision
    cout << x/2 << endl;  // "1.5"
}
//! [declaration example]


////////////////////////////////////////////////////////////////////////////////
//! [basic arithmetic example]
void basic_arithmetic_example()
{
    // define a constant signed value with 3 integer and 28 fractional bits (s3:28)
    constexpr auto pi = fixed_point<int32_t, -28>{3.1415926535};

    // expressions involving integers return fixed_point results
    constexpr auto tau = pi*2;
    static_assert(is_same<decltype(tau), decltype(pi)>::value, "");

    // "6.28319"
    cout << tau << endl;

    // expressions involving floating-point values return floating-point results
    constexpr auto degrees = tau*(180/3.1415926534);
    static_assert(is_same<decltype(degrees), const double>::value, "");

    // "360"
    cout << degrees << '\n';
}
//! [basic arithmetic example]


////////////////////////////////////////////////////////////////////////////////
//! [advanced arithmetic example]
#include <iomanip>

void advanced_arithmetic_example()
{
    // this variable uses all of its capacity
    auto x = fixed_point<uint8_t, -4>{15.9375};

    // 15.9375 * 15.9375 = 254.00390625 ... overflow!
    cout << fixed_point<uint8_t, -4>{x*x} << endl;  // "14" instead!

    // by default, fixed-point follows similar promotion rules to native types
    auto xx = x*x;

    // x * x has type fixed_point<int, -4>
    static_assert(is_same<decltype(xx), fixed_point<int, -4>>::value, "");
    cout << x*x << endl;  // "254" - better but not perfect

    // for full control, use named functions:
    cout << setprecision(12)
            << multiply<fixed_point<uint16_t, -8>>(x, x) << endl;  // 254.00390625
}
//! [advanced arithmetic example]


////////////////////////////////////////////////////////////////////////////////
//! [boost example]
#include <sg14/auxiliary/multiprecision.h>

void boost_example()
{
    // define an unsigned type with 400 integer digits and 400 fractional digits
    // and use boost::multiprecision::uint128_t as the model for the Rep type
    using big_number = make_ufixed<400, 400, boost::multiprecision::uint128_t>;
    static_assert(big_number::digits==800, "");

    // a googol is 10^100
    auto googol = big_number{1};
    for (auto zeros = 0; zeros!=100; ++zeros) {
        googol *= 10;
    }

    // "1e+100"
    cout << googol << endl;

    // "1e-100" although this calculation is only approximate
    cout << big_number{1}/googol << endl;
}
//! [boost example]

////////////////////////////////////////////////////////////////////////////////
// test the examples

#include <gtest/gtest.h>

// calls the given function and checks that produces the given output
void test_function(void(* function)(), char const* output)
{
    // substitute cout for a string
    stringstream captured_cout;
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(captured_cout.rdbuf()); //redirect cout to out.txt!

    // run example from documentation
    function();

    // restore cout
    cout.rdbuf(coutbuf); //reset to standard output again

    // test the content of the string
    ASSERT_EQ(output, captured_cout.str());
}

TEST(index, examples)
{
    test_function(declaration_example, "7\n3.5\n1.5\n");
    test_function(basic_arithmetic_example, "6.28319\n360\n");
    test_function(advanced_arithmetic_example, "14\n254\n254.00390625\n");
    test_function(boost_example, "1e+100\n1e-100\n");
}
