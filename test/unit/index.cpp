
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
#include <cnl/all.h>
#include <iostream>

using cnl::power;
using cnl::scaled_integer;
using namespace std;

void declaration_example()
{
#if 1
    // x is represented by an int and scaled down by 1 bit
    auto* x = new scaled_integer<int, power<-1>>{3.5};

    // under the hood, x stores a whole number
    cout << cnl::_impl::to_rep(*x) << endl;  // "7"

    // but it multiplies that whole number by 2^-1 to produce a real number
    cout << *x << endl;  // "3.5"

    // like an int, *x has limited precision
    (*x) /= 2;
    cout << *x << endl;  // "1.5"

    if (*x == 1.5) {
        return;
    }

    delete x;  // NOLINT(cppcoreguidelines-owning-memory)
#else
    // x is represented by an int and scaled down by 1 bit
    auto x = scaled_integer<int, power<-1>>{3.5};

    // under the hood, x stores a whole number
    cout << to_rep(x) << endl;  // "7"

    // but it multiplies that whole number by 2^-1 to produce a real number
    cout << x << endl;  // "3.5"

    // like an int, x has limited precision
    x /= 2;
    cout << x << endl;  // "1.5"
#endif
}
//! [declaration example]

TEST(index, declaration_example)  // NOLINT
{
    test_function(declaration_example, "7\n3.5\n1.5\n");
}


////////////////////////////////////////////////////////////////////////////////
//! [basic arithmetic example]
void basic_arithmetic_example()
{
    // define a constant signed value with 3 integer and 28 fraction bits (s3:28)
    auto pi = scaled_integer<int32_t, power<-28>>{3.1415926535};

    // expressions involving integers return scaled_integer results
    auto tau = pi*2;
    static_assert(is_same<decltype(tau), scaled_integer<int32_t, power<-28>>>::value, "");

    // "6.28319"
    cout << tau << endl;

    // expressions involving floating-point values return floating-point results
    auto degrees = tau*(180/3.1415926534);
    static_assert(is_same<decltype(degrees), double>::value, "");

    // "360"
    cout << degrees << '\n';
}
//! [basic arithmetic example]

TEST(index, basic_arithmetic_example)  // NOLINT
{
    test_function(basic_arithmetic_example, "6.283185303211212158203125\n360\n");
}


////////////////////////////////////////////////////////////////////////////////
//! [advanced arithmetic example]
using cnl::elastic_scaled_integer;

void advanced_arithmetic_example()
{
    // this variable uses all of its capacity
    auto x = scaled_integer<uint8_t, power<-4>>{15.9375};

    // 15.9375 * 15.9375 = 254.00390625 ... overflow!
    auto xx1 = scaled_integer<uint8_t, power<-4>>{x*x};
    cout << xx1 << endl;  // "14" instead!

    // fixed-point multiplication operator obeys usual promotion and implicit conversion rules
    auto xx = x*x;

    // x*x is promoted to scaled_integer<int, -8>
    static_assert(is_same<decltype(xx), scaled_integer<int, power<-8>>>::value, "");
    cout << xx << endl;  // "254.00390625" - correct

    // you can avoid the pitfalls of integer promotion for good by using the elastic_scaled_integer type
    auto named_xx = make_elastic_scaled_integer(x) * make_elastic_scaled_integer(x);

    // this type tracks both the number of digits and the exponent to ensure lossless multiplication
    static_assert(is_same<decltype(named_xx), elastic_scaled_integer<16, -8, unsigned>>::value, "");
    cout << named_xx << endl;  // "254.00390625" - also correct but prone to overflow
}
//! [advanced arithmetic example]

TEST(index, advanced_arithmetic_example)  // NOLINT
{
    test_function(advanced_arithmetic_example, "14\n254.00390625\n254.00390625\n");
}


#if defined(CNL_BOOST_ENABLED)
#include "boost.throw_exception.h"

////////////////////////////////////////////////////////////////////////////////
//! [boost example]
#include <cnl/auxiliary/boost.multiprecision.h>

// With Boost.Multiprecision, scaled_integer can be any size.
// cnl::multiprecision aliases to a BMP type that works especially well in CNL.
using cnl::multiprecision;

// Here's a fixed-point type with any number of binary digits.
template<int NumDigits, int Exponent = 0>
using mp_scaled_integer = scaled_integer<multiprecision<NumDigits>, power<Exponent>>;

void boost_example()
{
    // Create an integer with 400 binary digits and 0 fraction digits.
    auto googol = mp_scaled_integer<400>{1};

    // A googol is 10^100.
    for (auto zeros = 0; zeros!=100; ++zeros) {
        googol *= 10;
    }

    // "1e+100"
    cout << googol << endl;

    // Dividing a s1.0 number by a u400.0 number
    auto googolth = quotient(mp_scaled_integer<1>{1}, googol);

    // produces a number with one integer digit and 400 fraction digits.
    static_assert(is_same<decltype(googolth), mp_scaled_integer<401, -400>>::value, "");

    // Prints "1e-100" (although this value is only approximate).
    cout << googolth << endl;
}
//! [boost example]

TEST(index, boost_example)  // NOLINT
{
    test_function(boost_example, "1000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "0000000000000000\n0.00000000000000000000000000000000000000000000000000000000000000000"
                                 "0000000000000000000000000000000000099999999999999999999770774623585337708826862887672"
                                 "4831177561317388688460742223401456454683384940352996717699238685486962227178573245938"
                                 "1164968077139637589293145550457554693525163515374773301313531059788797333232314526850"
                                 "92147632256423245538160582575967991474270857299444514865172095596790313720703125\n");
}
#endif


////////////////////////////////////////////////////////////////////////////////
//! [elastic example]
using cnl::elastic_integer;

void elastic_example1()
{
    // Consider an integer type which keeps count of the bits that it uses.
    auto a = elastic_integer<6, int8_t>{ 63 };

    // Results of its operations widen as required.
    auto aa = a*a;
    static_assert(is_same<decltype(aa), elastic_integer<12, int8_t >> ::value, "");

    // Obviously, this type no longer fits in a byte.
    static_assert(sizeof(aa)==2, "");

    // Addition requires smaller results.
    auto a2 = a+a;
    static_assert(is_same<decltype(a2), elastic_integer<7, int8_t >> ::value, "");

    (void)aa;
    (void)a2;
}

using cnl::elastic_scaled_integer;

void elastic_example2()
{
    // A type such as elastic_integer can be used to specialize scaled_integer.
    // Now arithmetic operations are more efficient and less error-prone.
    auto b = elastic_scaled_integer<31, -27, unsigned>{15.9375};
    auto bb = b*b;

    cout << bb << endl;  // "254.00390625"
    static_assert(is_same<decltype(bb), elastic_scaled_integer<62, -54, unsigned>>::value, "");
}
//! [elastic example]

TEST(index, elastic_example)  // NOLINT
{
    test_function(elastic_example1, "");
    test_function(elastic_example2, "254.00390625\n");
}
