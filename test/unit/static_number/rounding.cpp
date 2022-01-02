
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>

#include <type_traits>

namespace {
    namespace test_rounding_t {
        static_assert(
                std::is_same_v<
                        cnl::nearest_rounding_tag, cnl::rounding_t<cnl::static_number<1>>>);
    }
}
