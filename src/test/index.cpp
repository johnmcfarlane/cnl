
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// example code presented on the front page of the Doxygen documentation

////////////////////////////////////////////////////////////////////////////////
// test_function - run given function and test its output

#include <gtest/gtest.h>
#include <iostream>

// calls the given function and checks that it produces the expected output
void test_function(void(* function)(), char const* output)
{
    // substitute cout for a string
    std::stringstream captured_cout;
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(captured_cout.rdbuf()); //redirect cout to out.txt!

    // run example from documentation
    function();

    // restore cout
    std::cout.rdbuf(coutbuf); //reset to standard output again

    // test the content of the string
    ASSERT_EQ(output, captured_cout.str());
}


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
    auto y = make_fixed<30, 1>{3.5};  // (s30:1)
    static_assert(is_same<decltype(x), decltype(y)>::value, "");  // assumes that int is 32-bit

    // under the hood, x stores a whole number
    cout << x.data() << endl;  // "7"

    // but it multiplies that whole number by 2^-1 to produce a real number
    cout << x << endl;  // "3.5"

    // like an int, x has limited precision
    x /= 2;
    cout << x << endl;  // "1.5"
}
//! [declaration example]

TEST(index, declaration_example)
{
    test_function(declaration_example, "7\n3.5\n1.5\n");
}


////////////////////////////////////////////////////////////////////////////////
//! [basic arithmetic example]
void basic_arithmetic_example()
{
    // define a constant signed value with 3 integer and 28 fractional bits (s3:28)
    auto pi = fixed_point<int32_t, -28>{3.1415926535};

    // expressions involving integers return fixed_point results
    auto tau = pi*2;
    static_assert(is_same<decltype(tau), fixed_point<int64_t, -28>>::value, "");

    // "6.28319"
    cout << tau << endl;

    // expressions involving floating-point values return floating-point results
    auto degrees = tau*(180/3.1415926534);
    static_assert(is_same<decltype(degrees), double>::value, "");

    // "360"
    cout << degrees << '\n';
}
//! [basic arithmetic example]

TEST(index, basic_arithmetic_example)
{
    test_function(basic_arithmetic_example, "6.28319\n360\n");
}


////////////////////////////////////////////////////////////////////////////////
//! [advanced arithmetic example]
#include <iomanip>

void advanced_arithmetic_example()
{
    // this variable uses all of its capacity
    auto x = fixed_point<uint8_t, -4>{15.9375};

    // 15.9375 * 15.9375 = 254.00390625 ... overflow!
    cout << fixed_point<uint8_t, -4>{x*x} << endl;  // "14" instead!

    // fixed-point multiplication operator widens result
    auto xx = x*x;

    // x * x has type fixed_point<uint16_t, -8>
    static_assert(is_same<decltype(xx), fixed_point<uint16_t, -8>>::value, "");
    cout << setprecision(12) << xx << endl;  // "254.00390625" - correct

    // for maximum efficiency, use named functions:
    auto named_xx = multiply(x, x);

    // multiply result is same as underlying representation's operation
    static_assert(is_same<decltype(named_xx), fixed_point<int, -8>>::value, "");
    cout << named_xx << endl;  // "254.00390625" - also correct but prone to overflow
}
//! [advanced arithmetic example]

TEST(index, advanced_arithmetic_example)
{
    test_function(advanced_arithmetic_example, "14\n254.00390625\n254.00390625\n");
}


#if defined(SG14_BOOST_ENABLED)
////////////////////////////////////////////////////////////////////////////////
//! [boost example]
#include <sg14/auxiliary/multiprecision.h>

void boost_example()
{
    // define an unsigned type with 400 integer digits and 400 fractional digits
    // and use boost::multiprecision::uint128_t as the archetype for the Rep type
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

TEST(index, boost_example)
{
    test_function(boost_example, "1e+100\n1e-100\n");
}
#endif


#if defined(SG14_EXCEPTIONS_ENABLED) && defined(SG14_SAFE_NUMERICS_ENABLED)
////////////////////////////////////////////////////////////////////////////////
//! [safe numerics example]
#include <sg14/auxiliary/safe_integer.h>

#include <gtest/gtest.h>
void safe_integer_example()
{
    // a safe, 8-bit fixed-point type with range -8 <= x < 7.9375
    using safe_byte = make_fixed<3, 4, boost::numeric::safe<int>>;

    // prints "-8"
    try {
        auto a = safe_byte{-8};
        cout << a << endl;
    }
    catch (std::range_error e) {
        cout << e.what() << endl;
    }

    // prints "Value out of range for this safe type"
    try {
        auto b = safe_byte{10};
        cout << b << endl;
    }
    catch (std::range_error e) {
        cout << e.what() << endl;
    }
}
//! [safe numerics example]

TEST(index, safe_integer_example)
{
    test_function(safe_integer_example, "-8\nValue out of range for this safe type\n");
}
#endif


////////////////////////////////////////////////////////////////////////////////
//! [elastic example]
#include <sg14/auxiliary/elastic_integer.h>

void elastic_example1()
{
    // Consider an integer type which keeps count of the bits that it uses.
    auto a = elastic_integer<6, int8_t>{ 63 };

    // Results of its operations widen as required.
    auto aa = a*a;
    static_assert(is_same<decltype(aa), elastic_integer<12, int8_t >> ::value, "");

    // Obviously, this type no longer fits in a byte.
    static_assert(sizeof(aa)==2, "");

    // Addition requires smaller results
    auto a2 = a+a;
    static_assert(is_same<decltype(a2), elastic_integer<7, int8_t >> ::value, "");
}

// Such a type can be used to specialize fixed_point.
template<int IntegerDigits, int FractionalDigits, typename Archetype>
using elastic = fixed_point<elastic_integer<IntegerDigits+FractionalDigits, Archetype>, -FractionalDigits>;

void elastic_example2()
{
    // Now arithmetic operations are more efficient and less error-prone.
    auto b = elastic<4, 28, unsigned>{15.9375};
    auto bb = b*b;

    cout << bb << endl;  // "254.00390625"
    static_assert(is_same<decltype(bb), elastic<8, 56, unsigned>>::value, "");
}
//! [elastic example]

TEST(index, elastic_example)
{
    test_function(elastic_example1, "");
    test_function(elastic_example2, "254.00390625\n");
}
