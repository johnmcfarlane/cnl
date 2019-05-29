
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing tests of the `cnl/fraction.h` definitions

#include <cnl/fraction.h>

#include <cnl/cstdint.h>
#include <cnl/_impl/type_traits/identical.h>

namespace {
    using cnl::_impl::identical;

    namespace test_copy_ctor {
        static_assert(
                identical(
                        cnl::fraction<short>(123),
                        cnl::fraction<short>(cnl::fraction<short>(123))),
                "cnl::fraction argument deduction");
    }

    namespace test_fraction_conversion_ctor {
        static_assert(
                identical(
                        cnl::fraction<cnl::int64>(123),
                        cnl::fraction<cnl::int64>(cnl::fraction<cnl::int8>(123))),
                "cnl::fraction widening conversion from fraction");
        static_assert(
                identical(
                        cnl::fraction<cnl::int64>(123),
                        cnl::fraction<cnl::int64>(cnl::fraction<cnl::int8>(123))),
                "cnl::fraction narrowing conversion from fraction");
    }
}
