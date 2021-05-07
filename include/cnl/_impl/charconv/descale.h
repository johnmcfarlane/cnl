
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CHARCONV_DESCALE_H)
#define CNL_IMPL_CHARCONV_DESCALE_H

#include "../../integer.h"
#include "../../numeric_limits.h"
#include "../cstdint/types.h"
#include "../narrow_cast.h"
#include "../numbers/signedness.h"
#include "../scaled/is_scaled_tag.h"
#include "../scaled/power/declaration.h"
#include "../unreachable.h"

/// compositional numeric library
namespace cnl::_impl {
    template<integer Rep, int Radix>
    struct descaled {
        Rep significand;
        int exponent;
        static constexpr int radix = Radix;
    };

    template<
            integer Significand = int64, int OutRadix = 10,
            bool Precise = false,
            scaled_tag InScale = power<>,
            integer Rep = int>
    [[nodiscard]] constexpr auto descale(Rep const& input, InScale)
    {
        descaled<Significand, OutRadix> output{static_cast<Significand>(input), 0};

        if (!input) {
            return output;
        }

        auto const oob{
                (input < Rep{0})
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

        if constexpr (exponent_v<InScale> < 0) {
            for (int in_exponent = exponent_v<InScale>;
                 in_exponent != 0 || (Precise && !(output.significand % OutRadix));) {
                if (output.significand % radix_v<InScale>) {
                    if (oob(output.significand)) {
                        if (Precise) {
                            return unreachable<descaled<Significand, OutRadix>>("number cannot be represented in this form");
                        }
                    } else {
                        output.significand *= OutRadix;
                        output.exponent--;
                        continue;
                    }
                }

                output.significand = narrow_cast<Significand>(output.significand / radix_v<InScale>);
                in_exponent++;
            }
        } else {
            for (int in_exponent = exponent_v<InScale>;
                 in_exponent != 0 || !(output.significand % OutRadix);) {
                if (!(output.significand % OutRadix)) {
                    output.significand /= OutRadix;
                    output.exponent++;
                    continue;
                }

                if (!oob(output.significand)) {
                    output.significand *= radix_v<InScale>;
                    in_exponent--;
                }
            }
        }

        return output;
    }
}

#endif  // CNL_IMPL_CHARCONV_DESCALE_H
