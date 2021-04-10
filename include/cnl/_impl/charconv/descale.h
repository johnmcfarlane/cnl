
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CHARCONV_DESCALE_H)
#define CNL_IMPL_CHARCONV_DESCALE_H

#include "../../integer.h"
#include "../../numeric_limits.h"
#include "../cstdint/types.h"
#include "../numbers/signedness.h"
#include "../scaled/power.h"
#include "../scaled_integer/definition.h"
#include "../unreachable.h"
#include "../wrapper/to_rep.h"

/// compositional numeric library
namespace cnl::_impl {
    template<integer Rep, int Radix>
    struct descaled {
        Rep significand;
        int exponent;
    };

    template<integer Significand = int64, int OutRadix = 10, integer Rep = int, int InExponent = 0, int InRadix = 2>
    [[nodiscard]] constexpr auto descale(scaled_integer<Rep, power<InExponent, InRadix>> const& input)
    {
        descaled<Significand, OutRadix> output{static_cast<Significand>(_impl::to_rep(input)), 0};

        auto const oob{
                (input < 0.)
                ? []([[maybe_unused]] Significand const& n) -> bool {
                      if constexpr (numbers::signedness_v<Significand>) {
                          return n < -numeric_limits<Significand>::max() / OutRadix;
                      } else {
                          return unreachable<bool>("negative unsigned integer");
                      }
                  }
                : [](Significand const& n) {
                      return n > Significand{numeric_limits<Significand>::max() / OutRadix};
                  }};

        for (int in_exponent = InExponent; in_exponent != 0;) {
            if constexpr (InExponent < 0) {
                if (output.significand % InRadix) {
                    if (!oob(output.significand)) {
                        output.significand *= OutRadix;
                        output.exponent--;
                        continue;
                    }
                }

                output.significand /= InRadix;
                in_exponent++;
            } else if constexpr (InExponent > 0) {
                if (!oob(output.significand)) {
                    output.significand *= InRadix;
                    in_exponent--;
                }

                if (!(output.significand % OutRadix)) {
                    output.significand /= OutRadix;
                    output.exponent++;
                }
            } else {
                // prevents compilers from hanging
                return unreachable<descaled<Significand, OutRadix>>("impossible for condition");
            }
        }

        return output;
    }
}

#endif  // CNL_IMPL_CHARCONV_DESCALE_H
