
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/elastic_integer.h>
#include <sg14/auxiliary/precise_integer.h>

namespace sg14 {
    // precise elastic integer
    template<
            int IntegerDigits,
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int>
    using precise_elastic_integer = precise_integer<
            elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            RoundingTag>;

    template<
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int,
            class Input = int>
    precise_elastic_integer<
            std::numeric_limits<Input>::digits,
            RoundingTag,
            Narrowest>
    constexpr make_precise_elastic(Input const& input)
    {
        return input;
    }
}

namespace {
    using sg14::precise_elastic_integer;
    using std::is_same;
    using sg14::_impl::identical;

    namespace default_parameters {
        using sg14::precise_integer;
        using sg14::elastic_integer;

        static_assert(
                is_same<precise_elastic_integer<1>::rep::rep, int>::value,
                "sg14::precise_integer parameter default test failed");
    }

    namespace test_make_precise_elastic {
        static_assert(identical(precise_elastic_integer<1>{1}, precise_elastic_integer<1>{1}), "");
        static_assert(identical(sg14::make_precise_elastic(std::int16_t{7}), precise_elastic_integer<15>{7}), "");
    }

    namespace test_multiply {
        static_assert(identical(precise_elastic_integer<3>{7}*precise_elastic_integer<4>{10}, precise_elastic_integer<7>{70}), "precise_elastic_integer operator*");
        //static_assert(identical(precise_elastic_integer<3>{3}*.25, precise_elastic_integer<3>{.75}), "precise_elastic_integer operator*");
    }

    namespace test_scale {
        static_assert(identical(precise_elastic_integer<3>{7}*precise_elastic_integer<4>{10}, precise_elastic_integer<7>{70}), "precise_elastic_integer operator*");
    }
}
