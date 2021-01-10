
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/declaration.h>

#include <cnl/_impl/wrapper/declaration.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/type_traits/remove_signedness.h>
#include <cnl/_impl/wrapper/is_composite.h>
#include <cnl/_impl/wrapper/operators.h>
#include <cnl/_impl/wrapper/rep_of.h>
#include <cnl/_impl/wrapper/set_rep.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_parameters {
        static_assert(
                assert_same<cnl::_impl::wrapper<int>, cnl::_impl::wrapper<>>::value,
                "default parameters of cnl::number");
    }

    namespace test_is_composite {
        static_assert(
                identical(true, cnl::is_composite<cnl::_impl::wrapper<>>::value),
                "is_composite<number>");
    }

    namespace test_remove_signedness {
        static_assert(
                assert_same<
                        cnl::_impl::wrapper<unsigned short>,
                        cnl::remove_signedness<cnl::_impl::wrapper<signed short>>::type>::value,
                "cnl::remove_signedness<cnl::_impl::wrapper>");
    }

    namespace test_default_ctor {
        static_assert(
                identical(cnl::_impl::wrapper<int>{0}, cnl::_impl::wrapper<int>{}),
                "default ctor of cnl::number");
    }
}
