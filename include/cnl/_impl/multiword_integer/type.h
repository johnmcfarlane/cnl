
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_TYPE_H)
#define CNL_IMPL_MULTIWORD_INTEGER_TYPE_H 1

#include "forward_declaration.h"
#include "numeric_limits.h"
#include "rep.h"
#include "../duplex_integer/numeric_limits.h"
#include "../number_base.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // multiword_integer - a tree of duplex_integers big enough to store `NumWords` number of `Word` integers
        template<typename Word, int NumWords>
        class multiword_integer
                : public number_base<
                        multiword_integer<Word, NumWords>,
                        typename multiword_integer_rep<Word, NumWords>::type> {
            using _base = number_base<
                    multiword_integer<Word, NumWords>,
                    multiword_integer_rep_t<Word, NumWords>>;
        public:
            using rep = typename _base::rep;

            multiword_integer() = default;

            template<typename T>
            constexpr multiword_integer(T const& value)
                    : _base(static_cast<rep>(value)) { }

            template<class S>
            explicit constexpr operator S() const
            {
                return static_cast<S>(_impl::to_rep(*this));
            }
        };
    }
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_TYPE_H
