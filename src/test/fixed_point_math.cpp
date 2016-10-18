
//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <sg14/fixed_point>
#include <sg14/bits/fixed_point_math.h>

//Fails due to cast being out-of-range:
#include "fixed_point_math_Q0.cpp"
#include "fixed_point_math_Q1.cpp"
#include "fixed_point_math_Q15.cpp"
#include "fixed_point_math_Q31.cpp"

