
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/elastic_integer.h>
#include <sg14/auxiliary/precise_integer.h>
#include <sg14/auxiliary/safe_integer.h>

namespace sg14 {
    // precise safe elastic fixed-point
    template<
            int IntegerDigits,
            class OverflowTag = safe_integer<>::overflow_tag,
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int>
    using psei = elastic_integer<
            IntegerDigits,
            precise_integer<
                    safe_integer<
                            Narrowest,
                            OverflowTag
                    >,
                    RoundingTag
            >
    >;

    template<
            class OverflowTag = safe_integer<>::overflow_tag,
            class RoundingTag = precise_integer<>::rounding,
            class Narrowest = int,
            class Input = int>
    psei<
            std::numeric_limits<Input>::digits,
            OverflowTag, RoundingTag,
            Narrowest>
    constexpr make_psei(Input const& input)
    {
        return input;
    }
}

namespace {
    using sg14::make_psei;
    using sg14::psei;
    using std::is_same;
    using sg14::_impl::identical;

    namespace default_parameters {
        using sg14::precise_integer;
        using sg14::safe_integer;
        using sg14::elastic_integer;

        static_assert(
                is_same<psei<1>::rep::rep::rep, int>::value,
                "sg14::precise_integer parameter default test failed");
    }

    namespace test_multiply {
        static_assert(identical(sg14::psei<6>{7}*sg14::psei<13>{321}, sg14::psei<19>{2247}), "");
    }

    namespace test_make_psei {
        using namespace sg14::literals;
        static_assert(identical(make_psei(std::int16_t{7}), psei<15>{7}), "");
        static_assert(identical(make_psei(7_c), psei<3>{7}), "");
    }
}
