
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/precise_integer.h>

namespace cnl {
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
            numeric_limits<Input>::digits,
            RoundingTag,
            Narrowest>
    constexpr make_precise_elastic(Input const& input)
    {
        return input;
    }
}

namespace {
    using cnl::precise_elastic_integer;
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        using cnl::precise_integer;
        using cnl::elastic_integer;

        static_assert(
                is_same<precise_elastic_integer<1>::rep::rep, int>::value,
                "cnl::precise_integer parameter default test failed");
    }

    namespace test_make_precise_elastic {
        static_assert(identical(precise_elastic_integer<1>{1}, precise_elastic_integer<1>{1}), "");
        static_assert(identical(cnl::make_precise_elastic(std::int16_t{7}), precise_elastic_integer<15>{7}), "");
    }

    namespace test_multiply {
        static_assert(identical(precise_elastic_integer<3>{7}*precise_elastic_integer<4>{10}, precise_elastic_integer<7>{70}), "precise_elastic_integer operator*");
        //static_assert(identical(precise_elastic_integer<3>{3}*.25, precise_elastic_integer<3>{.75}), "precise_elastic_integer operator*");
    }

    namespace test_scale {
        static_assert(identical(precise_elastic_integer<3>{7}*precise_elastic_integer<4>{10}, precise_elastic_integer<7>{70}), "precise_elastic_integer operator*");
    }
}
