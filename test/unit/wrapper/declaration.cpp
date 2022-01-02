
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wrapper/declaration.h>

#include <cnl/_impl/wrapper/declaration.h>

#include <cnl/_impl/numbers/set_signedness.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/_impl/wrapper/integer.h>
#include <cnl/_impl/wrapper/is_composite.h>
#include <cnl/_impl/wrapper/operators.h>
#include <cnl/_impl/wrapper/rep_of.h>
#include <cnl/_impl/wrapper/set_rep.h>
#include <cnl/fixed_point.h>
#include <cnl/number.h>

#include <type_traits>

using cnl::_impl::identical;

namespace {
    namespace test_concepts {
        static_assert(cnl::integer<cnl::_impl::wrapper<int>>);
        static_assert(cnl::integer<cnl::_impl::wrapper<long long unsigned int> const>);
        static_assert(cnl::fixed_point<cnl::_impl::wrapper<int>>);
        static_assert(cnl::fixed_point<cnl::_impl::wrapper<int> const>);

        static_assert(cnl::number<cnl::_impl::wrapper<int>>);
        static_assert(cnl::number<cnl::_impl::wrapper<int> const>);

        static_assert(cnl::arithmetic<cnl::_impl::wrapper<int>>);
        static_assert(cnl::arithmetic<cnl::_impl::wrapper<long long unsigned int> const>);
    }

    namespace test_parameters {
        static_assert(std::is_same_v<cnl::_impl::wrapper<int>, cnl::_impl::wrapper<>>);
    }

    namespace test_is_composite {
        static_assert(
                identical(true, cnl::is_composite<cnl::_impl::wrapper<>>::value),
                "is_composite<number>");
    }

    namespace test_set_signedness {
        static_assert(
                std::is_same_v<
                        cnl::_impl::wrapper<unsigned short>,
                        cnl::numbers::set_signedness_t<cnl::_impl::wrapper<signed short>, false>>);
    }

    namespace test_default_ctor {
        static_assert(
                identical(cnl::_impl::wrapper<int>{0}, cnl::_impl::wrapper<int>{}),
                "default ctor of cnl::number");
    }
}
