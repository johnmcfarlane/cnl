
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_INSTANTIATE_DUPLEX_INTEGER_H)
#define CNL_IMPL_DUPLEX_INTEGER_INSTANTIATE_DUPLEX_INTEGER_H 1

#include "../common.h"
#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/make_signed.h"
#include "../type_traits/set_signedness.h"
#include "forward_declaration.h"

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
            using lower = set_signedness_t<upper, false>;
            using type = duplex_integer<upper, lower>;
        };

        template<typename Word, int NumWords>
        struct multiword_integer {
        private:
            static_assert(NumWords>2, "");
            static constexpr auto num_words = NumWords;
            static constexpr auto num_words_rounded_up = (1 << used_digits(num_words-1));
            static constexpr auto upper_num_words = num_words_rounded_up/2;
            static constexpr auto lower_num_words = num_words-upper_num_words;
            using upper = typename multiword_integer<Word, upper_num_words>::type;
            using lower = typename multiword_integer<set_signedness_t<Word, false>, lower_num_words>::type;
        public:
            using type = duplex_integer<upper, lower>;
        };

        template<typename Word, int NumWords>
        using multiword_integer_t = typename multiword_integer<Word, NumWords>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // is_power_of_two

        // requires positive N
        template<int N, class Enable = void>
        struct is_power_of_two;

        template<int N>
        struct is_power_of_two<N, enable_if_t<(N>0)>>
                : std::integral_constant<bool, !(N & (N-1))> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // optimal_duplex

        template<typename Word, typename Signedness = set_signedness_t<int, is_signed<Word>::value>>
        struct optimal_duplex;

        template<typename Narrowest>
        struct optimal_duplex<Narrowest, unsigned> {
            static constexpr auto double_word_digits = max_digits<Narrowest>::value;
            static_assert(double_word_digits>=2 && is_power_of_two<double_word_digits>::value,
                    "invalid integer type, Narrowest");

            // Because multiword_integer needs to perform double-width arithmetic operations,
            // its word type should be half the maximum width.
            static constexpr auto word_digits = double_word_digits/2;
            using word = set_digits_t<Narrowest, word_digits>;
            static_assert(digits<word>::value==word_digits, "failed to half a double-width word");

            using type = word;
        };

        template<typename Narrowest>
        struct optimal_duplex<Narrowest, signed> {
            using unsiged_narrowest = make_unsigned_t<Narrowest>;
            using unsigned_multiword_integer = optimal_duplex<unsiged_narrowest, unsigned>;

            using type = make_signed_t<typename unsigned_multiword_integer::type>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // instantiate_duplex_integer

        template<int Digits, typename Narrowest>
        struct instantiate_duplex_integer {
            using word = typename optimal_duplex<Narrowest>::type;
            static constexpr auto num_sign_bits = is_signed<word>::value;
            static constexpr auto word_digits = digits<word>::value+num_sign_bits;
            static constexpr auto required_num_words = (Digits+num_sign_bits+word_digits-1)/word_digits;

            // Otherwise, it's not multi!
            static constexpr auto plural_num_words = max(2, required_num_words);

            using type = multiword_integer_t<word, plural_num_words>;
        };

        template<int Digits, typename Narrowest>
        using instantiate_duplex_integer_t = typename instantiate_duplex_integer<Digits, Narrowest>::type;
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_INSTANTIATE_DUPLEX_INTEGER_H
