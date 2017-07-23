
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/elastic_integer.h>
#include <sg14/auxiliary/precise_integer.h>
#include <sg14/auxiliary/safe_integer.h>

namespace cnl {
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
    using cnl::make_psei;
    using cnl::psei;
    using std::is_same;
    using cnl::_impl::identical;

    namespace default_parameters {
        using cnl::precise_integer;
        using cnl::safe_integer;
        using cnl::elastic_integer;

        static_assert(
                is_same<psei<1>::rep::rep::rep, int>::value,
                "cnl::precise_integer parameter default test failed");
    }

    namespace test_multiply {
        static_assert(identical(cnl::psei<6>{7}*cnl::psei<13>{321}, cnl::psei<19>{2247}), "");
    }

    namespace test_make_psei {
        using namespace cnl::literals;
        static_assert(identical(make_psei(std::int16_t{7}), psei<15>{7}), "");
        static_assert(identical(make_psei(7_c), psei<3>{7}), "");
    }
}
