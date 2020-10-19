
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_MAKE_FRACTION_H)
#define CNL_IMPL_FRACTION_MAKE_FRACTION_H

#include "../../limits.h"
#include "../assert.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/remove_signedness.h"
#include "type.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        /// creates a fraction with types deduced from the numerator and denominator
        template<typename Numerator, typename Denominator>
        CNL_NODISCARD constexpr fraction<Numerator, Denominator> make_fraction(Numerator const& n, Denominator const& d)
        {
            return fraction<Numerator, Denominator>{n, d};
        }

        /// creates a fraction with types deduced from the numerator
        template<typename Numerator>
        CNL_NODISCARD constexpr fraction<Numerator, Numerator> make_fraction(Numerator const& n)
        {
            return fraction<Numerator, Numerator>{n, 1};
        }

        /// creates a fraction from a floating-point number
        template<typename Numerator, typename Denominator=Numerator, typename FloatingPoint=float>
        CNL_NODISCARD constexpr auto make_fraction(FloatingPoint d)
        -> _impl::enable_if_t<numeric_limits<FloatingPoint>::is_iec559, cnl::fraction<Numerator, Denominator>>
        {
            // merge requests gratefully received
            static_assert(
                    std::is_same<Numerator, Denominator>::value,
                    "This function only supports cnl::fraction<T, T> and not cnl::fraction<T1, T2>.");

            using int_t = Numerator;
            static_assert(
                    is_signed<int_t>::value,
                    "This function only supports cnl::fraction of signed integers.");

            using uint_t = cnl::remove_signedness_t<int_t>;

            if (d < FloatingPoint{}) {
                return cnl::fraction<Numerator, Denominator>(-make_fraction<int_t>(-d));
            }
            CNL_ASSERT(d <= FloatingPoint(cnl::numeric_limits<int_t>::max()));
            auto left{fraction<int_t>(int_t(d), 1)};
            auto right{fraction<int_t>{int_t(left.numerator+1), 1}};
            if (FloatingPoint(left)==d) {
                return left;
            }
            if (FloatingPoint(right)==d) {
                return right;
            }
            auto lefts{0};
            auto rights{0};
            for (;;) {
                auto const mid{fraction<uint_t>(
                        uint_t(left.numerator+right.numerator),
                        uint_t(left.denominator+right.denominator))};
                CNL_ASSERT(int_t(mid.numerator) >= 0);
                CNL_ASSERT(int_t(mid.denominator) >= 0);

                auto fn = [mid, d](
                        int& fars, fraction<int_t>& f,
                        int& nears, fraction<int_t>& n) {
                    nears = 0;
                    if (fars++ < 3) {
                        f = mid;
                        return false;
                    }
                    auto const dividend{d*FloatingPoint(f.denominator)-FloatingPoint(f.numerator)};
                    auto const divisor{FloatingPoint(n.numerator)-d*FloatingPoint(n.denominator)};
                    auto const n0{dividend/divisor};
                    CNL_ASSERT(n0 <= FloatingPoint(cnl::numeric_limits<int_t>::max()));

                    auto const n1{((FloatingPoint(f.denominator)+FloatingPoint(n.denominator)*n0)
                            > FloatingPoint(cnl::numeric_limits<int_t>::max()))
                            ? int_t(FloatingPoint(cnl::numeric_limits<int_t>::max()-f.denominator)
                                    /FloatingPoint(n.denominator))
                            : int_t(n0)};
                    auto const n2{((FloatingPoint(f.numerator)+FloatingPoint(n.numerator*n1))
                            > FloatingPoint(cnl::numeric_limits<int_t>::max()))
                            ? int_t(FloatingPoint(cnl::numeric_limits<int_t>::max()-f.numerator-n.numerator)
                                    /FloatingPoint(n.numerator))
                            : n1};
                    if (!n2) {
                        return true;
                    }
                    f.numerator = int_t(f.numerator+n2*n.numerator);
                    f.denominator = int_t(f.denominator+n2*n.denominator);
                    return FloatingPoint(f) == d;
                };

                auto mid_q{FloatingPoint(mid)};
                if (mid_q < d) {
                    if (fn(lefts, left, rights, right)) {
                        return left;
                    }
                }
                else if (mid_q > d) {
                    if (fn(rights, right, lefts, left)) {
                        return right;
                    }
                }
                else {
                    return mid;
                }
            }
        }
    }
}

#endif  // CNL_IMPL_FRACTION_MAKE_FRACTION_H
