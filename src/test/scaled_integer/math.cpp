
//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <cnl/_impl/scaled_integer/math.h>
#include <cnl/scaled_integer.h>

//Fails due to cast being out-of-range:
#include "scaled_integer_math_Q0.h"
#include "scaled_integer_math_Q1.h"
#include "scaled_integer_math_Q15.h"
#include "scaled_integer_math_Q31.h"

