
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/elastic_integer/sqrt.h>

#include <cnl/elastic_integer.h>

#include <test.h>

using namespace cnl::literals;

static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xffffffffU},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0xfffffffe00000001ULL))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xfffffffeU},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0xfffffffe00000000LLU))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xffffU},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0xffffffffU))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xB505U},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0x80001219U))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xB504U},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0x80001218U))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xB504U},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0x80000000U))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0xB504},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0x7fffffff))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{100},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(std::int16_t{10000}))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{10},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(std::int8_t{100}))));
static_assert(identical(
        cnl::elastic_integer<32, unsigned>{0},
        cnl::sqrt(cnl::elastic_integer<64, unsigned>(0))));
