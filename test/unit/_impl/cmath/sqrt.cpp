
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/cmath/sqrt.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

static_assert(identical(0xffffffffLLU, cnl::sqrt(0xfffffffe00000001LLU)));
static_assert(identical(0xfffffffeLLU, cnl::sqrt(0xfffffffe00000000LLU)));
static_assert(identical(0xffffU, cnl::sqrt(0xffffffffU)));
static_assert(identical(0xB505U, cnl::sqrt(0x80001219U)));
static_assert(identical(0xB504U, cnl::sqrt(0x80001218U)));
static_assert(identical(0xB504U, cnl::sqrt(0x80000000U)));
static_assert(identical(0xB504, cnl::sqrt(0x7fffffff)));
static_assert(identical(100, cnl::sqrt(cnl::int16{10000})));
static_assert(identical(10, cnl::sqrt(cnl::int8{100})));
static_assert(identical(0, cnl::sqrt(0)));
