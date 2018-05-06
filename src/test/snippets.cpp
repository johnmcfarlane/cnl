
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_number.h>

using namespace cnl;

// contains snippets of code displayed in Doxygen documentation as examples
namespace {

namespace define_a_fixed_point_value {
//! [define a fixed_point value]
constexpr auto n = fixed_point<std::int8_t, -3>{-2.75};
static_assert(n==-2.75, "fixed-point type was unable to store the value");
//! [define a fixed_point value]
}

namespace define_a_fast_object_using_make_elastic {
//! [define an int-sized object using make_elastic_number and constant]
// std::uint8_t specifies the type of const_integer - not elastic
constexpr auto n = make_elastic_number(constant<0xAA>{});

static_assert(n==0xAA, "n now has the value, 1024");
static_assert(std::is_same<decltype(n), elastic_number<7, 1, int> const>::value, "by default make_elastic_number uses the most efficient type it can");
//! [define an int-sized object using make_elastic_number and constant]
}

namespace define_a_small_object_using_make_elastic {
//! [define a byte-sized object using \ref make_elastic_number and \ref _c]
constexpr auto n = make_elastic_number<char>(constant<1536>{});

static_assert(n==1536, "n now has the value, 1536");
static_assert(std::is_same<decltype(n), elastic_number<2, 9, char> const>::value, "by default make_elastic_number uses the most efficient type it can");
//! [define a byte-sized object using \ref make_elastic_number and \ref _c]
}

namespace define_a_fast_object_using_elastic_literal {
//! [define an object using elastic literal]
using namespace cnl::literals;
constexpr auto n = 34_elastic;

static_assert(n==34, "n now has the value, 1536");
static_assert(std::is_same<decltype(n), elastic_number<5, 1> const>::value, "type only uses 1 bit of range");
//! [define an object using elastic literal]
}

namespace use_resize_1 {
//! [use set_digits 1]
using new_type = set_digits_t<unsigned, 16>;
static_assert(std::is_same<new_type, uint16_t>::value, "failed to resize an unsigned int to 16-bytes");
//! [use set_digits 1]
}

namespace use_resize_2 {
//! [use set_digits 2]
using new_type = set_digits_t<signed char, 35>;
static_assert(std::is_same<new_type, int64_t>::value, "failed to resize a signed char to a type of at least 35 bits");
//! [use set_digits 2]
}

namespace use_resize_3 {
//! [use set_digits 3]
using new_type = set_digits_t<fixed_point<int8_t>, 24>;
static_assert(std::is_same<new_type, fixed_point<int32_t>>::value, "failed to resize a signed, 1-byte fixed-point type to a fixed-point type of at least 3 bytes");
//! [use set_digits 3]
}
}
