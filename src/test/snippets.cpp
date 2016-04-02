
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fixed_point.h>
#include <elastic.h>

using namespace sg14;

// contains snippets of code displayed in Doxygen documentation as examples
namespace {

namespace define_a_fixed_point_value {
//! [define a fixed_point value]
constexpr auto n = fixed_point<char, -3>{-2.75};
static_assert(n==-2.75, "fixed-point type was unable to store the value");
//! [define a fixed_point value]
}

namespace define_an_object_using_elasticate {
//! [define an object using elasticate]
constexpr auto n = elasticate<1024>();

static_assert(n==1024, "n now has the value, 1024");
static_assert(sizeof(n)==1, "elasticate uses the smallest type it can");
//! [define an object using elasticate]
}

namespace use_resize_1 {
//! [use resize 1]
using new_type = resize_t<unsigned, 2>;
static_assert(std::is_same<new_type, uint16_t>::value, "failed to resize an native-sized unsigned int to 2-bytes");
//! [use resize 1]
}

namespace use_resize_2 {
//! [use resize 2]
using new_type = resize_t<signed char, 5>;
static_assert(std::is_same<new_type, int64_t>::value, "failed to resize a signed byte type to a built-in signed type of at least 5 bytes");
//! [use resize 2]
}

namespace use_resize_3 {
//! [use resize 3]
using new_type = resize_t<fixed_point<int8_t>, 3>;
static_assert(std::is_same<new_type, fixed_point<int32_t>>::value, "failed to resize a signed, 1-byte fixed-point type to a fixed-point type of at least 3 bytes");
//! [use resize 3]
}

namespace use_make_fixed {
//! [use make_fixed]
static_assert(std::is_same<make_fixed<7, 8, signed>, fixed_point<int16_t, -8>>::value, "failed to use make_fixed");
//! [use make_fixed]
}
}
