
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_INSTANTIATE_DUPLEX_INTEGER_H)
#define CNL_IMPL_DUPLEX_INTEGER_INSTANTIATE_DUPLEX_INTEGER_H

#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "../numbers/set_signedness.h"
#include "../numbers/signedness.h"
#include "forward_declaration.h"

#include <algorithm>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::multiword_integer

        // given integer type and count, produces duplex_integer with NumWords Word(s)
        template<typename Word, int NumWords>
        struct multiword_integer;

        template<typename Word>
        struct multiword_integer<Word, 1> {
            using type = Word;
        };

        template<typename Word>
        struct multiword_integer<Word, 2> {
            using upper = Word;
            using lower = numbers::set_signedness_t<upper, false>;
            using type = duplex_integer<upper, lower>;
        };

        template<typename Word, int NumWords>
        struct multiword_integer {
        private:
            static_assert(NumWords > 2);
            static constexpr auto num_words = NumWords;
            static constexpr auto num_words_rounded_up = (1 << used_digits(num_words - 1));
            static constexpr auto upper_num_words = num_words_rounded_up / 2;
            static constexpr auto lower_num_words = num_words - upper_num_words;
            using upper = typename multiword_integer<Word, upper_num_words>::type;
            using lower = typename multiword_integer<
                    numbers::set_signedness_t<Word, false>, lower_num_words>::type;

        public:
            using type = duplex_integer<upper, lower>;
        };

        template<typename Word, int NumWords>
        using multiword_integer_t = typename multiword_integer<Word, NumWords>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // is_power_of_two

        // requires positive N
        template<int N>
        inline constexpr auto is_power_of_two = N > 0 && !(N & (N - 1));

        ////////////////////////////////////////////////////////////////////////////////
        // optimal_duplex

        template<typename Word, typename Signedness = numbers::set_signedness_t<int, numbers::signedness_v<Word>>>
        struct optimal_duplex;

        template<typename Narrowest>
        struct optimal_duplex<Narrowest, unsigned> {
            static constexpr auto double_word_digits = max_digits<Narrowest>;
            static_assert(
                    double_word_digits >= 2 && is_power_of_two<double_word_digits>,
                    "invalid integer type, Narrowest");

            // Because multiword_integer needs to perform double-width arithmetic operations,
            // its word type should be half the maximum width.
            static constexpr auto word_digits = double_word_digits / 2;
            using word = set_digits_t<Narrowest, word_digits>;
            static_assert(digits<word> == word_digits, "failed to half a double-width word");

            using type = word;
        };

        template<typename Narrowest>
        struct optimal_duplex<Narrowest, signed> {
            using unsigned_narrowest = numbers::set_signedness_t<Narrowest, false>;
            using unsigned_multiword_integer = optimal_duplex<unsigned_narrowest, unsigned>;

            using type = numbers::set_signedness_t<typename unsigned_multiword_integer::type, true>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // instantiate_duplex_integer

        template<typename Word>
        [[nodiscard]] constexpr auto duplex_num_words(int min_digits)
        {
            auto const num_sign_bits = numbers::signedness_v<Word>;
            auto const word_digits = digits<Word> + num_sign_bits;
            auto const required_num_words =
                    (min_digits + num_sign_bits + word_digits - 1) / word_digits;

            // Otherwise, it's not multi!
            auto const plural_num_words = std::max(2, required_num_words);

            return plural_num_words;
        }

        template<int Digits, typename Narrowest>
        struct instantiate_duplex_integer {
            using word = typename optimal_duplex<Narrowest>::type;
            using type = multiword_integer_t<word, duplex_num_words<word>(Digits)>;
        };

        template<int Digits, typename Narrowest>
        using instantiate_duplex_integer_t =
                typename instantiate_duplex_integer<Digits, Narrowest>::type;
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_INSTANTIATE_DUPLEX_INTEGER_H
