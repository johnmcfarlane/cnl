
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/fixed_point>
#include <sg14/auxiliary/elastic_integer.h>
#include <sg14/auxiliary/precise_integer.h>
#include <sg14/auxiliary/safe_integer.h>

namespace sg14 {
    template<
            int IntegerDigits,
            int FractionalDigits = 0,
            class OverflowPolicy = safe_integer<>::overflow,
            class RoundingPolicy = precise_integer<>::rounding,
            class Narrowest = int>
    using precise_safe_elastic_fixed_point =
    fixed_point<
            precise_integer<
                    safe_integer<
                            elastic_integer<
                                    IntegerDigits,
                                    Narrowest>,
                            OverflowPolicy>,
                    RoundingPolicy>,
            -FractionalDigits>;
}

namespace {
    using sg14::precise_safe_elastic_fixed_point;
    using std::is_same;

    namespace default_parameters {
        using sg14::precise_integer;
        using sg14::safe_integer;
        using sg14::elastic_integer;

        static_assert(
                is_same<precise_safe_elastic_fixed_point<1>::rep::rep::rep::rep, int>::value,
                "sg14::precise_integer parameter default test failed");
    }
}
