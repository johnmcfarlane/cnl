#include <fixed_point.h>

#include <cmath>

template <typename FP>
constexpr FP magnitude_squared(const FP & x, const FP & y, const FP & z)
{
	return x * x + y * y + z * z;
}

template <typename FP>
constexpr auto magnitude_trunc(const FP & x, const FP & y, const FP & z)
-> decltype(trunc_sqrt(trunc_add(trunc_square(x), trunc_square(y), trunc_square(z))))
{
	return trunc_sqrt(trunc_add(trunc_square(x), trunc_square(y), trunc_square(z)));
}

template <typename Real>
bool circle_intersect_generic(Real x1, Real y1, Real r1, Real x2, Real y2, Real r2)
{
	auto x_diff = x2 - x1;
	auto y_diff = y2 - y1;
	auto distance_squared = x_diff * x_diff + y_diff * y_diff;

	auto touch_distance = r1 + r2;
	auto touch_distance_squared = touch_distance * touch_distance;

	return distance_squared <= touch_distance_squared;
}

template <typename Real>
bool circle_intersect_trunc(Real x1, Real y1, Real r1, Real x2, Real y2, Real r2)
{
	using namespace sg14;

	auto x_diff = trunc_subtract(x2, x1);
	auto y_diff = trunc_subtract(y2, y1);
	auto distance_squared = trunc_add(trunc_square(x_diff), trunc_square(y_diff));

	auto touch_distance = trunc_add(r1, r2);
	auto touch_distance_squared = trunc_square(touch_distance);

	return distance_squared <= touch_distance_squared;
}
