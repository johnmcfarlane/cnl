
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/num_traits.h>
#include "number_test.h"

template struct number_test<char>;

template struct number_test<cnl::int8>;
template struct number_test<cnl::uint8>;

template struct number_test<cnl::int16>;
template struct number_test<cnl::uint16>;

template struct number_test<cnl::int32>;
template struct number_test<cnl::uint32>;

template struct number_test<cnl::int64>;
template struct number_test<cnl::uint64>;

#if defined(CNL_INT128)
template struct number_test<CNL_INT128>;
template struct number_test<CNL_UINT128>;
#endif
