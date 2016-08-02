
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "sample_functions.h"

#include <sg14/bits/int128.h>

#include <benchmark/benchmark.h>

#define ESCAPE(X) escape_cppcon2015(&X)
//#define ESCAPE(X) escape_codedive2015(&X)
//#define ESCAPE(x) benchmark::DoNotOptimize(x)

using std::numeric_limits;
using sg14::make_fixed;
using sg14::make_ufixed;

////////////////////////////////////////////////////////////////////////////////
// entry point

BENCHMARK_MAIN();

////////////////////////////////////////////////////////////////////////////////
// optimization circumvention:
// https://youtu.be/nXaxk27zwlk?t=40m40s or
// https://youtu.be/vrfYLlR8X8k?t=29m25s

#if defined(__clang__) || defined(__GNUG__)

void escape_cppcon2015(void const* p)
{
    asm volatile(""::"g"(p):"memory");
}

template <typename T>
void escape_codedive2015(T&& p)
{
    asm volatile("": "+r" (p));
}

void clobber()
{
    asm volatile("":::"memory");
}

#else
// TODO: Find equivalents `if defined(_MSC_VER)`
template <typename T>
void escape_cppcon2015(T&&)
{
}

void clobber()
{
}
#endif

////////////////////////////////////////////////////////////////////////////////
// benchmarking functions

template<typename T>
static void add(benchmark::State& state)
{
    auto addend1 = static_cast<T>(numeric_limits<T>::max()/5);
    auto addend2 = static_cast<T>(numeric_limits<T>::max()/3);
    while (state.KeepRunning()) {
        ESCAPE(addend1);
        ESCAPE(addend2);
        auto value = addend1+addend2;
        ESCAPE(value);
    }
}

template<typename T>
static void sub(benchmark::State& state)
{
    auto minuend = static_cast<T>(numeric_limits<T>::max()/5);
    auto subtrahend = static_cast<T>(numeric_limits<T>::max()/3);
    while (state.KeepRunning()) {
        ESCAPE(minuend);
        ESCAPE(subtrahend);
        auto value = minuend+subtrahend;
        ESCAPE(value);
    }
}

template<typename T>
static void mul(benchmark::State& state)
{
    auto factor1 = numeric_limits<T>::max()/5;
    auto factor2 = numeric_limits<T>::max()/3;
    while (state.KeepRunning()) {
        ESCAPE(factor1);
        ESCAPE(factor2);
        auto value = factor1*factor2;
        ESCAPE(value);
    }
}

template<typename T>
static void div(benchmark::State& state)
{
    auto nume = static_cast<T>(numeric_limits<T>::max()/int8_t{5});
    auto denom = static_cast<T>(numeric_limits<T>::max()/int8_t{3});
    while (state.KeepRunning()) {
        ESCAPE(nume);
        ESCAPE(denom);
        auto value = nume/denom;
        ESCAPE(value);
    }
}

template<typename T>
static void bm_sqrt(benchmark::State& state)
{
    auto input = numeric_limits<T>::max()/5;
    while (state.KeepRunning()) {
        ESCAPE(input);
        auto output = sqrt(input);
        ESCAPE(output);
    }
}

template<typename T>
static void bm_magnitude_squared(benchmark::State& state)
{
    auto x = T {1};
    auto y = T {4};
    auto z = T {9};
    while (state.KeepRunning()) {
        ESCAPE(x);
        ESCAPE(y);
        ESCAPE(z);
        auto value = magnitude_squared(x, y, z);
        ESCAPE(value);
    }
}

template<typename T>
static void bm_circle_intersect_generic(benchmark::State& state)
{
    auto x1 = T {0};
    auto y1 = T {10};
    auto r1 = T {14};
    auto x2 = T {4};
    auto y2 = T {13};
    auto r2 = T {9};
    while (state.KeepRunning()) {
        ESCAPE(x1);
        ESCAPE(y1);
        ESCAPE(r1);
        ESCAPE(x2);
        ESCAPE(y2);
        ESCAPE(r2);
        auto value = circle_intersect_generic(x1, y1, r1, x2, y2, r2);
        ESCAPE(value);
    }
}

template<typename T>
static void circle_intersect_generic(benchmark::State& state)
{
    auto x1 = T {0};
    auto y1 = T {10};
    auto r1 = T {14};
    auto x2 = T {4};
    auto y2 = T {13};
    auto r2 = T {9};
    while (state.KeepRunning()) {
        ESCAPE(x1);
        ESCAPE(y1);
        ESCAPE(r1);
        ESCAPE(x2);
        ESCAPE(y2);
        ESCAPE(r2);
        auto value = circle_intersect_generic(x1, y1, r1, x2, y2, r2);
        ESCAPE(value);
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
#if defined(SG14_INT128_ENABLED)
#define FIXED_POINT_BENCHMARK_FIXED(fn) \
    BENCHMARK_TEMPLATE1(fn, u4_4); \
    BENCHMARK_TEMPLATE1(fn, s3_4); \
    BENCHMARK_TEMPLATE1(fn, u8_8); \
    BENCHMARK_TEMPLATE1(fn, s7_8); \
    BENCHMARK_TEMPLATE1(fn, u16_16); \
    BENCHMARK_TEMPLATE1(fn, s15_16); \
    BENCHMARK_TEMPLATE1(fn, u32_32); \
    BENCHMARK_TEMPLATE1(fn, s31_32);
#else
#define FIXED_POINT_BENCHMARK_FIXED(fn) \
    BENCHMARK_TEMPLATE1(fn, u4_4); \
    BENCHMARK_TEMPLATE1(fn, s3_4); \
    BENCHMARK_TEMPLATE1(fn, u8_8); \
    BENCHMARK_TEMPLATE1(fn, s7_8); \
    BENCHMARK_TEMPLATE1(fn, u16_16); \
    BENCHMARK_TEMPLATE1(fn, s15_16);
#endif

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

// tests involving unoptimized math function, sg14::sqrt
FIXED_POINT_BENCHMARK_REAL(bm_sqrt);
