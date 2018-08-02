
//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <cnl/fixed_point.h>
#include <cnl/_impl/fixed_point/math.h>

//Fails due to cast being out-of-range:
#include "fixed_point_math_Q0.h"
#include "fixed_point_math_Q1.h"
#include "fixed_point_math_Q15.h"
#include "fixed_point_math_Q31.h"

