#if defined(RUN_BENCHMARKS)

#include "sample_functions.h"

#include <fixed_point_utils.h>

#include <benchmark/benchmark.h>

#include <iostream>
#include <limits>

using namespace sg14;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// entry point

BENCHMARK_MAIN();

////////////////////////////////////////////////////////////////////////////////
// optimization circumvention

void escape(void const *p)
{
	asm volatile("":"+r"(p)::"memory");
}

void clobber()
{
	asm volatile("":::"memory");
}

////////////////////////////////////////////////////////////////////////////////
// benchmarking functions

template <typename T>
static void add(benchmark::State& state)
{
	auto addend1 = numeric_limits<T>::max() / 5;
	auto addend2 = numeric_limits<T>::max() / 3;
	while (state.KeepRunning())
	{
		escape(&addend1);
		escape(&addend2);
		auto value = addend1 + addend2;
		escape(& value);
	}
}

template <typename T>
static void sub(benchmark::State& state)
{
	auto minuend = numeric_limits<T>::max() / 5;
	auto subtrahend = numeric_limits<T>::max() / 3;
	while (state.KeepRunning())
	{
		escape(&minuend);
		escape(&subtrahend);
		auto value = minuend + subtrahend;
		escape(& value);
	}
}

template <typename T>
static void mul(benchmark::State& state)
{
	auto factor1 = numeric_limits<T>::max() / 5;
	auto factor2 = numeric_limits<T>::max() / 3;
	while (state.KeepRunning())
	{
		escape(&factor1);
		escape(&factor2);
		auto value = factor1 * factor2;
		escape(& value);
	}
}

template <typename T>
static void div(benchmark::State& state)
{
	auto nume = numeric_limits<T>::max() / 5;
	auto denom = numeric_limits<T>::max() / 3;
	while (state.KeepRunning())
	{
		escape(&nume);
		escape(&denom);
		auto value = nume / denom;
		escape(& value);
	}
}

template <typename T>
static void bm_sqrt(benchmark::State& state)
{
	auto input = numeric_limits<T>::max() / 5;
	while (state.KeepRunning())
	{
		escape(&input);
		auto output = sqrt(input);
		escape(& output);
	}
}

template <typename T>
static void bm_magnitude_squared(benchmark::State& state)
{
	auto x = T { 1 };
	auto y = T { 4 };
	auto z = T { 9 };
	while (state.KeepRunning())
	{
		escape(&x);
		escape(&y);
		escape(&z);
		auto value = magnitude_squared(x, y, z);
		escape(& value);
	}
}

template <typename T>
static void bm_magnitude_trunc(benchmark::State& state)
{
	auto x = T { 1 };
	auto y = T { 4 };
	auto z = T { 9 };
	while (state.KeepRunning())
	{
		escape(&x);
		escape(&y);
		escape(&z);
		auto value = magnitude_trunc(x, y, z);
		escape(& value);
	}
}

template <typename T>
static void bm_circle_intersect_generic(benchmark::State& state)
{
	auto x1 = T { 0 };
	auto y1 = T { 10 };
	auto r1 = T { 14 };
	auto x2 = T { 4 };
	auto y2 = T { 13 };
	auto r2 = T { 9 };
	while (state.KeepRunning())
	{
		escape(&x1);
		escape(&y1);
		escape(&r1);
		escape(&x2);
		escape(&y2);
		escape(&r2);
		auto value = circle_intersect_generic(x1, y1, r1, x2, y2, r2);
		escape(& value);
	}
}

template <typename T>
static void circle_intersect_generic(benchmark::State& state)
{
	auto x1 = T { 0 };
	auto y1 = T { 10 };
	auto r1 = T { 14 };
	auto x2 = T { 4 };
	auto y2 = T { 13 };
	auto r2 = T { 9 };
	while (state.KeepRunning())
	{
		escape(&x1);
		escape(&y1);
		escape(&r1);
		escape(&x2);
		escape(&y2);
		escape(&r2);
		auto value = circle_intersect_generic(x1, y1, r1, x2, y2, r2);
		escape(& value);
	}
}

template <typename T>
static void bm_circle_intersect_trunc(benchmark::State& state)
{
	auto x1 = T { 0 };
	auto y1 = T { 10 };
	auto r1 = T { 14 };
	auto x2 = T { 4 };
	auto y2 = T { 13 };
	auto r2 = T { 9 };
	while (state.KeepRunning())
	{
		escape(&x1);
		escape(&y1);
		escape(&r1);
		escape(&x2);
		escape(&y2);
		escape(&r2);
		auto value = circle_intersect_trunc(x1, y1, r1, x2, y2, r2);
		escape(& value);
	}
}

////////////////////////////////////////////////////////////////////////////////
// fixed-point types

using u4_4 = make_ufixed<4, 4>;
using s3_4 = make_fixed<3, 4>;
using u8_8 = make_ufixed<8, 8>;
using s7_8 = make_fixed<7, 8>;
using u16_16 = make_ufixed<16, 16>;
using s15_16 = make_fixed<15, 16>;
using u32_32 = make_ufixed<32, 32>;
using s31_32 = make_fixed<31, 32>;

////////////////////////////////////////////////////////////////////////////////
// multi-type benchmark macros

#define FIXED_POINT_BENCHMARK_FLOAT(fn) \
	BENCHMARK_TEMPLATE1(fn, float); \
	BENCHMARK_TEMPLATE1(fn, double); \
	BENCHMARK_TEMPLATE1(fn, long double);

#define FIXED_POINT_BENCHMARK_INT(fn) \
	BENCHMARK_TEMPLATE1(fn, int8_t); \
	BENCHMARK_TEMPLATE1(fn, uint8_t); \
	BENCHMARK_TEMPLATE1(fn, int16_t); \
	BENCHMARK_TEMPLATE1(fn, uint16_t); \
	BENCHMARK_TEMPLATE1(fn, int32_t); \
	BENCHMARK_TEMPLATE1(fn, uint32_t); \
	BENCHMARK_TEMPLATE1(fn, int64_t); \
	BENCHMARK_TEMPLATE1(fn, uint64_t);

// types that can store values >= 1
#define FIXED_POINT_BENCHMARK_FIXED(fn) \
	BENCHMARK_TEMPLATE1(fn, u4_4); \
	BENCHMARK_TEMPLATE1(fn, s3_4); \
	BENCHMARK_TEMPLATE1(fn, u8_8); \
	BENCHMARK_TEMPLATE1(fn, s7_8); \
	BENCHMARK_TEMPLATE1(fn, u16_16); \
	BENCHMARK_TEMPLATE1(fn, s15_16); \
	BENCHMARK_TEMPLATE1(fn, u32_32); \
	BENCHMARK_TEMPLATE1(fn, s31_32);

#define FIXED_POINT_BENCHMARK_REAL(fn) \
	FIXED_POINT_BENCHMARK_FLOAT(fn); \
	FIXED_POINT_BENCHMARK_FIXED(fn);

#define FIXED_POINT_BENCHMARK_COMPLETE(fn) \
	FIXED_POINT_BENCHMARK_REAL(fn); \
	FIXED_POINT_BENCHMARK_INT(fn);

////////////////////////////////////////////////////////////////////////////////
// benchmark invocations

FIXED_POINT_BENCHMARK_COMPLETE(add);
FIXED_POINT_BENCHMARK_COMPLETE(sub);
FIXED_POINT_BENCHMARK_COMPLETE(mul);
FIXED_POINT_BENCHMARK_COMPLETE(div);

FIXED_POINT_BENCHMARK_REAL(bm_magnitude_squared);

FIXED_POINT_BENCHMARK_REAL(bm_circle_intersect_generic);
FIXED_POINT_BENCHMARK_FIXED(bm_circle_intersect_trunc);

// tests involving unoptimized math function, sg14::sqrt
//FIXED_POINT_BENCHMARK_REAL(bm_sqrt);
//FIXED_POINT_BENCHMARK_FIXED(bm_magnitude_trunc);

#endif	// RUN_BENCHMARKS
