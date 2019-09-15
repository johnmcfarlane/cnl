
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "sample_functions.h"

#include <cnl/all.h>

#include <benchmark/benchmark.h>

using std::numeric_limits;
using cnl::scaled_integer;
using cnl::static_number;

////////////////////////////////////////////////////////////////////////////////
// entry point

BENCHMARK_MAIN();

////////////////////////////////////////////////////////////////////////////////
// benchmarking functions

template<class T>
static void add(benchmark::State& state)
{
    auto addend1 = static_cast<T>(numeric_limits<T>::max()/5);
    auto addend2 = static_cast<T>(numeric_limits<T>::max()/3);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(addend1);
        benchmark::DoNotOptimize(addend2);
        auto value = addend1+addend2;
        benchmark::DoNotOptimize(value);
    }
}

template<class T>
static void sub(benchmark::State& state)
{
    auto minuend = static_cast<T>(numeric_limits<T>::max()/5);
    auto subtrahend = static_cast<T>(numeric_limits<T>::max()/3);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(minuend);
        benchmark::DoNotOptimize(subtrahend);
        auto value = minuend+subtrahend;
        benchmark::DoNotOptimize(value);
    }
}

template<class T>
static void mul(benchmark::State& state)
{
    auto factor1 = static_cast<T>(numeric_limits<T>::max()/int8_t{5});
    auto factor2 = static_cast<T>(numeric_limits<T>::max()/int8_t{3});
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(factor1);
        benchmark::DoNotOptimize(factor2);
        auto value = factor1*factor2;
        benchmark::DoNotOptimize(value);
    }
}

template<class T>
static void div(benchmark::State& state)
{
    auto nume = static_cast<T>(numeric_limits<T>::max()/int8_t{5});
    auto denom = static_cast<T>(numeric_limits<T>::max()/int8_t{3});
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(nume);
        benchmark::DoNotOptimize(denom);
        auto value = nume/denom;
        benchmark::DoNotOptimize(value);
    }
}

template<class T>
static void bm_sqrt(benchmark::State& state)
{
    auto input = static_cast<T>(numeric_limits<T>::max()/int8_t{5});
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(input);
        auto output = cnl::sqrt(input);
        benchmark::DoNotOptimize(output);
    }
}

template<class T>
static void bm_magnitude_squared(benchmark::State& state)
{
    auto x = T {1LL};
    auto y = T {4LL};
    auto z = T {9LL};
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(x);
        benchmark::DoNotOptimize(y);
        benchmark::DoNotOptimize(z);
        auto value = magnitude_squared(x, y, z);
        CNL_ASSERT(value==98.);
        benchmark::DoNotOptimize(value);
    }
}

template<class T>
static void bm_circle_intersect_generic(benchmark::State& state)
{
    auto x1 = T {0LL};
    auto y1 = T {10LL};
    auto r1 = T {14LL};
    auto x2 = T {4LL};
    auto y2 = T {13LL};
    auto r2 = T {9LL};
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(x1);
        benchmark::DoNotOptimize(y1);
        benchmark::DoNotOptimize(r1);
        benchmark::DoNotOptimize(x2);
        benchmark::DoNotOptimize(y2);
        benchmark::DoNotOptimize(r2);
        auto value = circle_intersect_generic(x1, y1, r1, x2, y2, r2);
        benchmark::DoNotOptimize(value);
    }
}

template<class T>
static void circle_intersect_generic(benchmark::State& state)
{
    auto x1 = T {0};
    auto y1 = T {10};
    auto r1 = T {14};
    auto x2 = T {4};
    auto y2 = T {13};
    auto r2 = T {9};
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(x1);
        benchmark::DoNotOptimize(y1);
        benchmark::DoNotOptimize(r1);
        benchmark::DoNotOptimize(x2);
        benchmark::DoNotOptimize(y2);
        benchmark::DoNotOptimize(r2);
        auto value = circle_intersect_generic(x1, y1, r1, x2, y2, r2);
        benchmark::DoNotOptimize(value);
    }
}

////////////////////////////////////////////////////////////////////////////////
// scaled_integer types

using u4_4 = scaled_integer<uint8_t, cnl::power<-4>>;
using s4_3 = scaled_integer<int8_t, cnl::power<-3>>;
using n4_3 = static_number<7, -3>;
using u8_8 = scaled_integer<uint16_t, cnl::power<-8>>;
using s7_8 = scaled_integer<int16_t, cnl::power<-8>>;
using n7_8 = static_number<15, -8>;
using u20_12 = scaled_integer<uint32_t, cnl::power<-12>>;
using s19_12 = scaled_integer<int32_t, cnl::power<-12>>;
using n19_12 = static_number<31, -12>;
using u36_28 = scaled_integer<uint64_t, cnl::power<-28>>;
using s35_28 = scaled_integer<int64_t, cnl::power<-28>>;
using n35_28 = static_number<63, -28>;

////////////////////////////////////////////////////////////////////////////////
// multi-type benchmark macros

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FIXED_POINT_BENCHMARK_FLOAT(fn) \
    BENCHMARK_TEMPLATE1(fn, float); \
    BENCHMARK_TEMPLATE1(fn, double); \
    BENCHMARK_TEMPLATE1(fn, long double);

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
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
#if defined(CNL_INT128_ENABLED)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FIXED_POINT_BENCHMARK_FIXED(fn) \
    BENCHMARK_TEMPLATE1(fn, u4_4); \
    BENCHMARK_TEMPLATE1(fn, s4_3); \
    BENCHMARK_TEMPLATE1(fn, n4_3); \
    BENCHMARK_TEMPLATE1(fn, u8_8); \
    BENCHMARK_TEMPLATE1(fn, s7_8); \
    BENCHMARK_TEMPLATE1(fn, n7_8); \
    BENCHMARK_TEMPLATE1(fn, u20_12); \
    BENCHMARK_TEMPLATE1(fn, s19_12); \
    BENCHMARK_TEMPLATE1(fn, n19_12); \
    BENCHMARK_TEMPLATE1(fn, u36_28); \
    BENCHMARK_TEMPLATE1(fn, s35_28); \
    BENCHMARK_TEMPLATE1(fn, n35_28);
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FIXED_POINT_BENCHMARK_FIXED(fn) \
    BENCHMARK_TEMPLATE1(fn, u4_4); \
    BENCHMARK_TEMPLATE1(fn, s4_3); \
    BENCHMARK_TEMPLATE1(fn, n4_3); \
    BENCHMARK_TEMPLATE1(fn, u8_8); \
    BENCHMARK_TEMPLATE1(fn, s7_8); \
    BENCHMARK_TEMPLATE1(fn, n7_8); \
    BENCHMARK_TEMPLATE1(fn, u20_12); \
    BENCHMARK_TEMPLATE1(fn, s19_12); \
    BENCHMARK_TEMPLATE1(fn, n19_12);
#endif

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FIXED_POINT_BENCHMARK_REAL(fn) \
    FIXED_POINT_BENCHMARK_FLOAT(fn) \
    FIXED_POINT_BENCHMARK_FIXED(fn)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FIXED_POINT_BENCHMARK_COMPLETE(fn) \
    FIXED_POINT_BENCHMARK_REAL(fn) \
    FIXED_POINT_BENCHMARK_INT(fn)

////////////////////////////////////////////////////////////////////////////////
// benchmark invocations

// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_COMPLETE(add)

// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_COMPLETE(sub)

// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_COMPLETE(mul)

// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_COMPLETE(div)

// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_REAL(bm_magnitude_squared)

// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_REAL(bm_circle_intersect_generic)

// tests involving unoptimized math function, cnl::sqrt
// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
FIXED_POINT_BENCHMARK_REAL(bm_sqrt)
