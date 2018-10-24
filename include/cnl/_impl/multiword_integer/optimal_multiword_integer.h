
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_OPTIMAL_MULTIWORD_INTEGER_H)
#define CNL_IMPL_MULTIWORD_INTEGER_OPTIMAL_MULTIWORD_INTEGER_H 1

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
        // is_power_of_two

        // requires positive N
        template<int N, class Enable = void>
        struct is_power_of_two;

        template<int N>
        struct is_power_of_two<N, enable_if_t<(N>0)>>
                : std::integral_constant<bool, !(N & (N-1))> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // optimal_multi_word

        template<typename Word, typename Signedness = set_signedness_t<int, is_signed<Word>::value>>
        struct optimal_multi_word;

        template<typename Narrowest>
        struct optimal_multi_word<Narrowest, unsigned> {
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
        struct optimal_multi_word<Narrowest, signed> {
            using unsiged_narrowest = make_unsigned_t<Narrowest>;
            using unsigned_multiword_integer = optimal_multi_word<unsiged_narrowest, unsigned>;

            using type = make_signed_t<typename unsigned_multiword_integer::type>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // optimal_multiword_integer

        template<int Digits, typename Narrowest>
        struct optimal_multiword_integer {
            using word = typename optimal_multi_word<Narrowest>::type;
            static constexpr auto num_sign_bits = is_signed<word>::value;
            static constexpr auto word_digits = digits<word>::value+num_sign_bits;
            static constexpr auto required_num_words = (Digits+num_sign_bits+word_digits-1)/word_digits;

            // Otherwise, it's not multi!
            static constexpr auto plural_num_words = max(2, required_num_words);

            using type = multiword_integer<word, plural_num_words>;
        };

        template<int Digits, typename Narrowest>
        using optimal_multiword_integer_t = typename optimal_multiword_integer<Digits, Narrowest>::type;
    }
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_OPTIMAL_MULTIWORD_INTEGER_H
