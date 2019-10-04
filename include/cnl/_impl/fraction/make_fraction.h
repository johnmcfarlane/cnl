
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_MAKE_FRACTION_H)
#define CNL_IMPL_FRACTION_MAKE_FRACTION_H

#include "type.h"
#include "../assert.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/remove_signedness.h"
#include "../../limits.h"

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
        CNL_NODISCARD CNL_RELAXED_CONSTEXPR auto make_fraction(FloatingPoint d)
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
            auto lefts{0}, rights{0};
            for (;;) {
                auto const mid{fraction<uint_t>(
                        uint_t(left.numerator+right.numerator),
                        uint_t(left.denominator+right.denominator))};
                CNL_ASSERT(int_t(mid.numerator) >= 0);
                CNL_ASSERT(int_t(mid.denominator) >= 0);

                auto f = [mid, d](
                        int& fars, fraction<int_t>& far,
                        int& nears, fraction<int_t>& near) {
                    nears = 0;
                    if (fars++ < 3) {
                        far = mid;
                        return false;
                    }
                    auto const dividend{d*FloatingPoint(far.denominator)-FloatingPoint(far.numerator)};
                    auto const divisor{FloatingPoint(near.numerator)-d*FloatingPoint(near.denominator)};
                    auto const n0{dividend/divisor};
                    CNL_ASSERT(n0 <= FloatingPoint(cnl::numeric_limits<int_t>::max()));

                    auto const n1{((FloatingPoint(far.denominator)+FloatingPoint(near.denominator)*n0)
                            > FloatingPoint(cnl::numeric_limits<int_t>::max()))
                            ? int_t(FloatingPoint(cnl::numeric_limits<int_t>::max()-far.denominator)
                                    /FloatingPoint(near.denominator))
                            : int_t(n0)};
                    auto const n2{((FloatingPoint(far.numerator)+FloatingPoint(near.numerator*n1))
                            > FloatingPoint(cnl::numeric_limits<int_t>::max()))
                            ? int_t(FloatingPoint(cnl::numeric_limits<int_t>::max()-far.numerator-near.numerator)
                                    /FloatingPoint(near.numerator))
                            : n1};
                    if (!n2) {
                        return true;
                    }
                    far.numerator = int_t(far.numerator+n2*near.numerator);
                    far.denominator = int_t(far.denominator+n2*near.denominator);
                    return FloatingPoint(far) == d;
                };

                auto mid_q{FloatingPoint(mid)};
                if (mid_q < d) {
                    if (f(lefts, left, rights, right)) {
                        return left;
                    }
                }
                else if (mid_q > d) {
                    if (f(rights, right, lefts, left)) {
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
