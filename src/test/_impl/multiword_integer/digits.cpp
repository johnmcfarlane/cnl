
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/multiword_integer/from_rep.h>

#include <cnl/_impl/multiword_integer/digits.h>

#include <cnl/_impl/type_traits/identical.h>

using cnl::_impl::identical;

namespace {
    namespace test_digits {
        static_assert(
                identical(
                        95,
                        cnl::digits<cnl::_impl::multiword_integer<cnl::int32, 3>>::value),
                "");
    }
}
