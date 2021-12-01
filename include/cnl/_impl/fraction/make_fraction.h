
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_MAKE_FRACTION_H)
#define CNL_IMPL_FRACTION_MAKE_FRACTION_H

#include "../cnl_assert.h"
#include "../numbers/set_signedness.h"
#include "definition.h"

#include <concepts>
#include <limits>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        /// creates a fraction with types deduced from the numerator and denominator
        template<typename Numerator, typename Denominator>
        [[nodiscard]] constexpr auto make_fraction(
                Numerator const& n, Denominator const& d)
        {
            return fraction<Numerator, Denominator>{n, d};
        }

        /// creates a fraction with types deduced from the numerator
        template<typename Numerator>
        [[nodiscard]] constexpr auto make_fraction(Numerator const& n)
        {
            return fraction<Numerator, Numerator>{n, 1};
        }

        /// creates a fraction from a floating-point number
        template<
                typename Numerator, typename Denominator = Numerator,
                std::floating_point FloatingPoint = float>
        [[nodiscard]] constexpr auto make_fraction(FloatingPoint d) -> cnl::fraction<Numerator, Denominator>
        {
            // merge requests gratefully received
            static_assert(
                    std::is_same<Numerator, Denominator>::value,
                    "This function only supports cnl::fraction<T, T> and not cnl::fraction<T1, "
                    "T2>.");

            using int_t = Numerator;
            static_assert(
                    numbers::signedness_v<int_t>,
                    "This function only supports cnl::fraction of signed integers.");

            using uint_t = numbers::set_signedness_t<int_t, false>;

            if (d < FloatingPoint{}) {
                return static_cast<cnl::fraction<Numerator, Denominator>>(-make_fraction<int_t>(-d));
            }
            CNL_ASSERT(d <= static_cast<FloatingPoint>(std::numeric_limits<int_t>::max()));
            auto left{fraction<int_t>(static_cast<int_t>(d), 1)};
            auto right{fraction<int_t>{static_cast<int_t>(left.numerator + 1), 1}};
            if (static_cast<FloatingPoint>(left) == d) {
                return left;
            }
            if (static_cast<FloatingPoint>(right) == d) {
                return right;
            }
            auto lefts{0};
            auto rights{0};
            for (;;) {
                auto const mid{fraction<uint_t>(
                        static_cast<uint_t>(left.numerator + right.numerator),
                        static_cast<uint_t>(left.denominator + right.denominator))};
                CNL_ASSERT(static_cast<int_t>(mid.numerator) >= 0);
                CNL_ASSERT(static_cast<int_t>(mid.denominator) >= 0);

                auto fn = [mid, d](int& fars, fraction<int_t>& f, int& nears, fraction<int_t>& n) {
                    nears = 0;
                    if (fars++ < 3) {
                        f = mid;
                        return false;
                    }
                    auto const dividend{
                            d * static_cast<FloatingPoint>(f.denominator) - static_cast<FloatingPoint>(f.numerator)};
                    auto const divisor{
                            static_cast<FloatingPoint>(n.numerator) - d * static_cast<FloatingPoint>(n.denominator)};
                    auto const n0{dividend / divisor};
                    CNL_ASSERT(n0 <= static_cast<FloatingPoint>(std::numeric_limits<int_t>::max()));

                    auto const n1{
                            ((static_cast<FloatingPoint>(f.denominator) + static_cast<FloatingPoint>(n.denominator) * n0)
                             > static_cast<FloatingPoint>(std::numeric_limits<int_t>::max()))
                                    ? static_cast<int_t>(static_cast<FloatingPoint>(std::numeric_limits<int_t>::max() - f.denominator) / static_cast<FloatingPoint>(n.denominator))
                                    : static_cast<int_t>(n0)};
                    auto const n2{
                            ((static_cast<FloatingPoint>(f.numerator) + static_cast<FloatingPoint>(n.numerator * n1))
                             > static_cast<FloatingPoint>(std::numeric_limits<int_t>::max()))
                                    ? static_cast<int_t>(static_cast<FloatingPoint>(std::numeric_limits<int_t>::max() - f.numerator - n.numerator) / static_cast<FloatingPoint>(n.numerator))
                                    : n1};
                    if (!n2) {
                        return true;
                    }
                    f.numerator = static_cast<int_t>(f.numerator + n2 * n.numerator);
                    f.denominator = static_cast<int_t>(f.denominator + n2 * n.denominator);
                    return static_cast<FloatingPoint>(f) == d;
                };

                auto mid_q{static_cast<FloatingPoint>(mid)};
                if (mid_q < d) {
                    if (fn(lefts, left, rights, right)) {
                        return left;
                    }
                } else if (mid_q > d) {
                    if (fn(rights, right, lefts, left)) {
                        return right;
                    }
                } else {
                    return mid;
                }
            }
        }
    }
}

#endif  // CNL_IMPL_FRACTION_MAKE_FRACTION_H
