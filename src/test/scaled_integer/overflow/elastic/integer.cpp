
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/scaled_integer.h>
#include <cnl/num_traits.h>
#include <cnl/overflow_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <cstdint>
#include <type_traits>

using cnl::_impl::identical;

namespace {
    template<
            int IntegerDigits,
            int FractionalDigits,
            class Narrowest>
    using saturated_elastic_scaled_integer = cnl::scaled_integer<
            cnl::overflow_integer<
                    cnl::elastic_integer<
                            IntegerDigits+FractionalDigits,
                            Narrowest>,
                    cnl::saturated_overflow_tag>,
            cnl::power<-FractionalDigits>>;

    namespace test_unwrap {
        using type = saturated_elastic_scaled_integer<7, 8, std::int16_t>;
#if !defined(_MSC_VER)
        static_assert(std::is_trivial<type>::value, "type is not trivial");
#endif
        static_assert(identical(std::int16_t{384}, cnl::unwrap(type{1.5})), "cnl::unwrap");
        static_assert(identical(type{1.5}, cnl::wrap<type>(std::int16_t{384})), "cnl::wrap");
    }
}
