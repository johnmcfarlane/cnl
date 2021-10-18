
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_BENCHMARK_SAMPLE_FUNCTIONS_H)
#define CNL_BENCHMARK_SAMPLE_FUNCTIONS_H

#include <cnl/scaled_integer.h>

#include <cmath>

template<typename FP>
[[nodiscard]] constexpr FP magnitude_squared(FP const& x, FP const& y, FP const& z)
{
    return static_cast<FP>(x * x + y * y + z * z);
}

template<typename Real>
bool circle_intersect_generic(Real x1, Real y1, Real r1, Real x2, Real y2, Real r2)
{
    auto x_diff = x2 - x1;
    auto y_diff = y2 - y1;
    auto distance_squared = x_diff * x_diff + y_diff * y_diff;

    auto touch_distance = r1 + r2;
    auto touch_distance_squared = touch_distance * touch_distance;

    return distance_squared <= touch_distance_squared;
}

#endif  // CNL_BENCHMARK_SAMPLE_FUNCTIONS_H
