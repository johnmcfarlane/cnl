
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of cnl::_impl::abs from <cnl/math.h>

#include <cnl/_impl/cmath/abs.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cstdint.h>

using cnl::_impl::identical;

namespace test_abs {
    static_assert(identical(cnl::_impl::abs(-302398479), 302398479));
    static_assert(identical(cnl::_impl::abs(302398479), 302398479));

    static_assert(identical(cnl::_impl::abs(-302398479LL), 302398479LL));
    static_assert(identical(cnl::_impl::abs(302398479ULL), 302398479ULL));

    static_assert(identical(cnl::_impl::abs(CNL_INTMAX_C(-302398479)), CNL_INTMAX_C(302398479)));
    static_assert(identical(cnl::_impl::abs(CNL_UINTMAX_C(302398479)), CNL_UINTMAX_C(302398479)));

    static_assert(identical(cnl::_impl::abs(-302398479.), 302398479.));
    static_assert(identical(cnl::_impl::abs(302398479.F), 302398479.F));
}
