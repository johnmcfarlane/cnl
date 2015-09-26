#if defined(RUN_BENCHMARKS)

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
	static auto v = numeric_limits<T>::max() / 2;
	for (auto input = 0u; state.KeepRunning(); ++ input)
	{
		escape(&v);
		auto value = v + v;
		escape(& value);
	}
}

template <typename T>
static void div(benchmark::State& state)
{
	static auto nume = numeric_limits<T>::max() / 5;
	static auto denom = numeric_limits<T>::max() / 3;
	for (auto input = 0u; state.KeepRunning(); ++ input)
	{
		escape(&nume);
		escape(&denom);
		auto value = nume / denom;
		escape(& value);
	}
}

using u4_4 = make_ufixed<4, 4>;
using u0_8 = make_ufixed<0, 8>;
using u8_0 = make_ufixed<8, 0>;
using u8_8 = make_ufixed<8, 8>;
using u0_16 = make_ufixed<0, 16>;
using u16_0 = make_ufixed<16, 0>;
using u16_16 = make_ufixed<16, 16>;
using u0_32 = make_ufixed<0, 32>;
using u32_0 = make_ufixed<32, 0>;
using u32_32 = make_ufixed<32, 32>;
using u0_64 = make_ufixed<0, 64>;
using u64_0 = make_ufixed<64, 0>;

#define FIXED_POINT_BENCHMARK_PARTIAL(fn) \
	BENCHMARK_TEMPLATE1(fn, float); \
	BENCHMARK_TEMPLATE1(fn, double); \
	BENCHMARK_TEMPLATE1(fn, long double); \
	BENCHMARK_TEMPLATE1(fn, int8_t); \
	BENCHMARK_TEMPLATE1(fn, uint8_t); \
	BENCHMARK_TEMPLATE1(fn, u8_0); \
	BENCHMARK_TEMPLATE1(fn, u4_4); \
	BENCHMARK_TEMPLATE1(fn, int16_t); \
	BENCHMARK_TEMPLATE1(fn, uint16_t); \
	BENCHMARK_TEMPLATE1(fn, u16_0); \
	BENCHMARK_TEMPLATE1(fn, u8_8); \
	BENCHMARK_TEMPLATE1(fn, int32_t); \
	BENCHMARK_TEMPLATE1(fn, uint32_t); \
	BENCHMARK_TEMPLATE1(fn, u32_0); \
	BENCHMARK_TEMPLATE1(fn, u16_16); \
	BENCHMARK_TEMPLATE1(fn, int64_t); \
	BENCHMARK_TEMPLATE1(fn, uint64_t); \
	BENCHMARK_TEMPLATE1(fn, u32_32); \
	BENCHMARK_TEMPLATE1(fn, u64_0);

#define FIXED_POINT_BENCHMARK_COMPLETE(fn) \
	FIXED_POINT_BENCHMARK_PARTIAL(fn); \
	BENCHMARK_TEMPLATE1(fn, u0_8); \
	BENCHMARK_TEMPLATE1(fn, u0_16); \
	BENCHMARK_TEMPLATE1(fn, u0_32); \
	BENCHMARK_TEMPLATE1(fn, u0_64);

FIXED_POINT_BENCHMARK_COMPLETE(add);
FIXED_POINT_BENCHMARK_PARTIAL(div);

#endif	// RUN_BENCHMARKS
