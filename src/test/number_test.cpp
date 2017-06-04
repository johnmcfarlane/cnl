
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/numeric_traits>
#include "number_test.h"

template struct number_test<char>;

template struct number_test<std::int8_t>;
template struct number_test<std::uint8_t>;

template struct number_test<std::int16_t>;
template struct number_test<std::uint16_t>;

template struct number_test<std::int32_t>;
template struct number_test<std::uint32_t>;

template struct number_test<std::int64_t>;
template struct number_test<std::uint64_t>;

#if defined(SG14_INT128)
template struct number_test<SG14_INT128>;
template struct number_test<SG14_UINT128>;
#endif

template struct number_test<float>;
template struct number_test<double>;
template struct number_test<long double>;
