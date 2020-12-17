
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_POLARITY_H)
#define CNL_IMPL_OVERFLOW_POLARITY_H

#include "config.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        enum class polarity { negative = -1, neutral = 0, positive = 1 };

        CNL_NODISCARD constexpr polarity operator-(polarity const& p)
        {
            return static_cast<polarity>(-static_cast<int>(p));
        }

        CNL_NODISCARD constexpr polarity operator*(polarity const& lhs, polarity const& rhs)
        {
            return static_cast<polarity>(static_cast<int>(lhs) * static_cast<int>(rhs));
        }

        template<typename T>
        CNL_NODISCARD constexpr polarity measure_polarity(T const& value)
        {
            return (value > T{}) ? polarity::positive
                                 : (value < T{}) ? polarity::negative : polarity::neutral;
        }
    }
}

#endif // CNL_IMPL_OVERFLOW_POLARITY_H
