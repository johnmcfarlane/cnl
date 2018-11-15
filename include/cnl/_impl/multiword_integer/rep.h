
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_REP_H)
#define CNL_IMPL_MULTIWORD_INTEGER_REP_H 1

#include "digits.h"
#include "forward_declaration.h"
#include "set_digits.h"
#include "../duplex_integer/ctors.h"
#include "../duplex_integer/type.h"
#include "../num_traits/digits.h"
#include "../type_traits/set_signedness.h"
#include "../../numeric.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // multiword_integer_rep - figures out what multiword_integer should be made of
        template<typename Word, int NumWords>
        struct multiword_integer_rep;

        template<typename Word>
        struct multiword_integer_rep<Word, 1> {
            using type = Word;
        };

        template<typename Word>
        struct multiword_integer_rep<Word, 2> {
            using upper = Word;
            using lower = set_signedness_t<upper, false>;
            using type = duplex_integer<upper, lower>;
        };

        template<typename Word, int NumWords>
        struct multiword_integer_rep {
        private:
            static_assert(NumWords>2, "");
            static constexpr auto num_words = NumWords;
            static constexpr auto num_words_rounded_up = (1 << used_digits(num_words-1));
            static constexpr auto upper_num_words = num_words_rounded_up/2;
            static constexpr auto lower_num_words = num_words-upper_num_words;
            using upper = typename multiword_integer_rep<Word, upper_num_words>::type;
            using lower = typename multiword_integer_rep<set_signedness_t<Word, false>, lower_num_words>::type;
        public:
            using type = duplex_integer<upper, lower>;
        };

        template<typename Word, int NumWords>
        using multiword_integer_rep_t = typename multiword_integer_rep<Word, NumWords>::type;
    }
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_REP_H
