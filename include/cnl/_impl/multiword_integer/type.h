
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_TYPE_H)
#define CNL_IMPL_MULTIWORD_INTEGER_TYPE_H 1

#include "../../numeric.h"
#include "../duplex_integer/numeric_limits.h"
#include "../duplex_integer/type.h"
#include "../num_traits/digits.h"
#include "../number_base.h"
#include "../operators.h"
#include "../type_traits/set_signedness.h"
#include "forward_declaration.h"

#include <iterator>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // multiword_integer_rep
        template<typename Word, int NumWords>
        struct multiword_integer_rep;

        template<typename Word>
        struct multiword_integer_rep<Word, 1> {
            using type = Word;
        };

        template<typename Word>
        struct multiword_integer_rep<Word, 2> {
            using lower = Word;
            using upper = set_signedness_t<lower, false>;
            using type = duplex_integer<lower, upper>;
        };

        template<typename Word, int NumWords>
        struct multiword_integer_rep {
        private:
            static_assert(NumWords > 2, "");
            static constexpr auto num_words = NumWords;
            static constexpr auto num_words_rounded_up = (1 << used_digits(num_words-1));
            static constexpr auto upper_num_words = num_words_rounded_up / 2;
            static constexpr auto lower_num_words = num_words - upper_num_words;
            using lower = typename multiword_integer_rep<Word, lower_num_words>::type;
            using upper = typename multiword_integer_rep<set_signedness_t<Word, false>, upper_num_words>::type;
        public:
            using type = duplex_integer<lower, upper>;
        };

        template<typename Word, int NumWords>
        using multiword_integer_rep_t = typename multiword_integer_rep<Word, NumWords>::type;

        // multiword_integer
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

    ////////////////////////////////////////////////////////////////////////////////
    // operator overloading system

    namespace _impl {
        template<typename Word, int NumWords>
        struct wants_generic_ops<multiword_integer<Word, NumWords>> : std::true_type {
        };

        template<class Operator, typename Word, int NumWords>
        struct comparison_operator<Operator, multiword_integer<Word, NumWords>, multiword_integer<Word, NumWords>> {};
    }
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_TYPE_H
