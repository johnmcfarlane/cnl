
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <sstream>

//! [include the header]
#include <sg14/fixed_point.h>
//! [include the header]

//! [using directive]
using namespace sg14;
//! [using directive]


TEST(index, print_pi_1) {
using std::setprecision;
std::stringstream cout;

//! [print pi]
fixed_point<std::int32_t, -28> pi(3.1415926535);
cout << "pi=" << setprecision(10) << pi;
//! [print pi]

ASSERT_EQ(cout.str(), "pi=3.141592652");
}


TEST(index, print_pi_2) {
using std::setprecision;
std::stringstream cout;

//! [print pi again]
make_fixed<3, 28> pi(3.1415926535);
//! [print pi again]
cout << "pi=" << setprecision(10) << pi;

ASSERT_EQ(cout.str(), "pi=3.141592652");
}
