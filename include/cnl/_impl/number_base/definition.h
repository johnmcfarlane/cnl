
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_BASE_DEFINITION_H)
#define CNL_IMPL_NUMBER_BASE_DEFINITION_H

#include "declaration.h"

#include "../../limits.h"
#include "../num_traits/to_rep.h"

#include <utility>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base {
        public:
            using rep = Rep;

            CNL_NODISCARD explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

        protected:
            static_assert(numeric_limits<Rep>::is_integer, "number_base must be specialized with integer Rep type template parameter");

            number_base() = default;

            explicit constexpr number_base(rep const& r)  // NOLINT(modernize-pass-by-value)
                : _rep(r) { }

            template<class T>
            CNL_RELAXED_CONSTEXPR number_base& operator=(T const& r) {
                _rep = r;
                // NOLINTNEXTLINE(cppcoreguidelines-c-copy-assignment-signature, misc-unconventional-assign-operator)
                return static_cast<Derived&>(*this);
            }

            friend struct cnl::to_rep<number_base>;
        private:
            rep _rep;
        };
    }
}

#endif  // CNL_IMPL_NUMBER_BASE_DEFINITION_H
