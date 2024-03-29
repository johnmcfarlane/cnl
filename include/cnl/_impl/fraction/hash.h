
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_HASH_H)
#define CNL_IMPL_FRACTION_HASH_H

#include "../../bit.h"
#include "../num_traits/width.h"
#include "canonical.h"
#include "definition.h"

#include <concepts>
#include <functional>

namespace std {
    template<typename Numerator, typename Denominator>
    struct hash<cnl::fraction<Numerator, Denominator>> {
        // Not implemented for floating-point components.
        // The problem (1./2.) == (2./4.) but the hashes are not equal.
        // There is no equivalent to GCD for floating-point fractions.
        // The solution may be to canonicalize the exponents somehow.
        static_assert(
                std::is_integral_v<Numerator> && std::is_integral_v<Denominator>,
                "std::hash<cnl::fractional<T>> - T must be an integer");

        [[nodiscard]] constexpr auto operator()(
                cnl::fraction<Numerator, Denominator> const& value) const
        {
            return from_canonical(cnl::_impl::canonical(value));
        }

    private:
        [[nodiscard]] static constexpr auto from_canonical(
                cnl::fraction<Numerator, Denominator> const& value)
        {
            return from_canonical_hashes(
                    hash<Numerator>{}(value.numerator), hash<Denominator>{}(value.denominator));
        }

        [[nodiscard]] static constexpr auto from_canonical_hashes(size_t const n, size_t const d)
        {
            return n ^ cnl::rotl(d, cnl::_impl::width<size_t> / 2);
        }
    };
}

#endif  // CNL_IMPL_FRACTION_HASH_H
