
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/fixed_point.h>
#include <sg14/auxiliary/elastic.h>

using namespace sg14;

// contains snippets of code displayed in Doxygen documentation as examples
namespace {

namespace define_a_fixed_point_value {
//! [define a fixed_point value]
constexpr auto n = fixed_point<char, -3>{-2.75};
static_assert(n==-2.75, "fixed-point type was unable to store the value");
//! [define a fixed_point value]
}

namespace define_a_fast_object_using_elasticate {
//! [define a fast object using elasticate]
constexpr auto n = elasticate<1024>();

static_assert(n==1024, "n now has the value, 1024");
static_assert(sizeof(n)==sizeof(int), "by default elasticate uses the most eficient type it can");
//! [define a fast object using elasticate]
}

namespace define_a_small_object_using_elasticate {
//! [define a small object using elasticate]
constexpr auto n = elasticate<1024, char>();

static_assert(n==1024, "n now has the value, 1024");
static_assert(sizeof(n)==sizeof(char), "by default elasticate uses the most eficient type it can");
//! [define a small object using elasticate]
}

namespace use_resize_1 {
//! [use set_width 1]
using new_type = set_width_t<unsigned, 16>;
static_assert(std::is_same<new_type, uint16_t>::value, "failed to resize an unsigned int to 16-bytes");
//! [use set_width 1]
}

namespace use_resize_2 {
//! [use set_width 2]
using new_type = set_width_t<signed char, 35>;
static_assert(std::is_same<new_type, int64_t>::value, "failed to resize a signed char to a type of at least 35 bits");
//! [use set_width 2]
}

namespace use_resize_3 {
//! [use set_width 3]
using new_type = set_width_t<fixed_point<int8_t>, 24>;
static_assert(std::is_same<new_type, fixed_point<int32_t>>::value, "failed to resize a signed, 1-byte fixed-point type to a fixed-point type of at least 3 bytes");
//! [use set_width 3]
}

namespace use_make_fixed {
//! [use make_fixed]
static_assert(std::is_same<make_fixed<7, 8, signed>, fixed_point<int16_t, -8>>::value, "failed to use make_fixed");
//! [use make_fixed]
}
}
