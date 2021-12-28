
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CHARCONV_DESCALE_H)
#define CNL_IMPL_CHARCONV_DESCALE_H

#include "../../integer.h"
#include "../cnl_assert.h"
#include "../cstdint/types.h"
#include "../numbers/signedness.h"
#include "../scaled/declaration.h"
#include "../unreachable.h"

#include <limits>

/// compositional numeric library
namespace cnl::_impl {
    template<integer Rep>
    struct descaled {
        Rep significand;
        int exponent;
        int radix;
    };

    template<
            integer Significand = std::int64_t,
            bool Precise = false,
            int InExponent = 0, int InRadix = 2,
            integer Rep = int>
    [[nodiscard]] constexpr auto descale(Rep const& input, power<InExponent, InRadix>, int out_radix = 10)
    {
        CNL_ASSERT(out_radix >= 2);
        CNL_ASSERT(out_radix <= 36);

        descaled<Significand> output{static_cast<Significand>(input), 0, out_radix};

        if (!input) {
            return output;
        }

        auto const oob{
                (input < Rep{0})
                ? []([[maybe_unused]] Significand const& n, int radix) -> bool {
                      if constexpr (numbers::signedness_v<Significand>) {
                          CNL_ASSERT(radix >= 0);
                          return n < -std::numeric_limits<Significand>::max() / radix;
                      } else {
                          return unreachable<bool>("negative unsigned integer");
                      }
                  }
                : [](Significand const& n, int radix) {
                      CNL_ASSERT(radix >= 0);
                      return n > std::numeric_limits<Significand>::max() / radix;
                  }};

        if constexpr (InExponent < 0) {
            for (int in_exponent = InExponent;
                 in_exponent != 0 || (Precise && !(output.significand % out_radix));) {
                if (output.significand % InRadix) {
                    if (oob(output.significand, out_radix)) {
                        if (Precise) {
                            unreachable<descaled<Significand>>("number cannot be represented in this form");
                        }
                    } else {
                        output.significand = static_cast<Significand>(output.significand * out_radix);
                        output.exponent--;
                        continue;
                    }
                }

                output.significand /= InRadix;
                in_exponent++;
            }
        } else {
            for (int in_exponent = InExponent;
                 in_exponent != 0 || !(output.significand % out_radix);) {
                if (!(output.significand % out_radix)) {
                    output.significand /= out_radix;
                    output.exponent++;
                    continue;
                }

                if (!oob(output.significand, out_radix)) {
                    output.significand *= InRadix;
                    in_exponent--;
                }
            }
        }

        return output;
    }
}

#endif  // CNL_IMPL_CHARCONV_DESCALE_H
