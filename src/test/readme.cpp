
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// tests the examples from README.md

#include <gtest/gtest.h>

// 1. Add the [*include*](include) directory to your list of system headers.

// 2. Essential definitions are in, [*fixed_point.h*](include/fixed_point.h):
#include <fixed_point.h>

// 3. Supplemental definitions are in, [*fixed_point_utils.h*](include/fixed_point_utils.h):
#include <fixed_point_utils.h>

// 4. All definitions are in the `sg14` namespace:
using namespace sg14;

TEST(readme, instance)
{
   std::stringstream std_cout;

   // 5. Use the `fixed_point` type to define variables.
   fixed_point<std::int32_t, -28> pi(3.1415926535);
   std_cout << "pi=" << std::setprecision(10) << pi;

   ASSERT_EQ(std_cout.str(), "pi=3.141592652");

   // 6. Alternatively, use `make_fixed` and `make_ufixed` to instantiate signed and unsigned variables
   //    with exactly the number of integer and fractional digits you require:
   static_assert(std::is_same<decltype(pi), sg14::make_fixed<3, 28>>::value, "error in README.md example code");
}
