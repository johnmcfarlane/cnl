
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/integer/type.h>

#include <cnl/_impl/integer/type.h>

#include <cnl/_impl/integer/from_rep.h>
#include <cnl/_impl/integer/operators.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/type_traits/remove_signedness.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_parameters {
        static_assert(
                assert_same<cnl::_impl::integer<int>, cnl::_impl::integer<>>::value,
                "default parameters of cnl::integer");
    }

    namespace test_is_composite {
        static_assert(
                identical(true, cnl::is_composite<cnl::_impl::integer<>>::value),
                "is_composite<integer>");
    }

    namespace test_remove_signedness {
        static_assert(
                assert_same<
                        cnl::_impl::integer<unsigned short>,
                        cnl::remove_signedness<cnl::_impl::integer<signed short>>::type>::value,
                "cnl::remove_signedness<cnl::_impl::integer>");
    }

    namespace test_default_ctor {
        static_assert(
                identical(
                        cnl::_impl::integer<int>{0},
                        cnl::_impl::integer<int>{}),
                "default ctor of cnl::integer");
    }
}
