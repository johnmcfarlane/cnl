
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TO_CHARS_H)
#define CNL_IMPL_TO_CHARS_H 1

#include "assert.h"
#include "num_traits/rounding.h"

#include <array>
#include <system_error>

/// compositional numeric library
namespace cnl {
    // cnl::to_chars_result - equivalent to std::to_chars_result
    struct to_chars_result {
        char* ptr;
        std::errc ec;
    };

    namespace _impl {
        // cnl::_impl::max_to_chars_chars
        template<typename Scalar, int Base=10>
        struct max_to_chars_chars;

        // cnl::_impl::itoc
        template<typename Scalar>
        char itoc(Scalar value) noexcept
        {
            static_assert(
                    std::is_same<
                            typename rounding<Scalar>::type,
                            native_rounding_tag>::value,
                    "wrong rounding type");
            auto c = '0'+static_cast<int>(value);
            return static_cast<char>(c);
        }

        // cnl::_impl::to_chars_natural
        template<class Integer>
        char* to_chars_natural(char* const ptr, char* const last, Integer const& value)
        {
            auto const quotient = value/10;

            auto const next_ptr = quotient
                    ? to_chars_natural(ptr, last, quotient)
                    : ptr;

            if (next_ptr==last || next_ptr==nullptr) {
                return nullptr;
            }

            auto const remainder = value-(quotient*10);
            *next_ptr = itoc(remainder);

            return next_ptr+1;
        }

        // cnl::_impl::to_chars_non_zero
        template<typename Number, bool is_signed = is_signed<Number>::value>
        struct to_chars_non_zero;

        template<typename Number>
        struct to_chars_non_zero<Number, false> {
            to_chars_result operator()(char* const first, char* const last, Number const& value) const
            {
                // +ve
                return to_chars_positive(first, last, value);
            }
        };

        template<typename Number>
        struct to_chars_non_zero<Number, true> {
            to_chars_result operator()(char* const first, char* const last, Number const& value) const
            {
                if (value>0.) {
                    // +ve
                    return to_chars_positive(first, last, value);
                }
                else {
                    auto const destination_length = std::distance(first, last);
                    if (destination_length<2) {
                        return to_chars_result{last, std::errc::value_too_large};
                    }

                    // -ve
                    *first = '-';
                    return to_chars_positive(first+1, last, -value);
                }
            }
        };
    }

    // overload of cnl::to_chars returning fixed-size array of chars
    // large enough to store any possible result for given input type
    template<typename Number>
    std::array<
            char,
            _impl::max_to_chars_chars<Number>::value+1>
    to_chars(Number const& value)
    {
        constexpr auto max_num_chars = _impl::max_to_chars_chars<Number>::value;

#if defined(__GNUG__) && !defined(__clang__) && __GNUG__ == 6
        // addresses uninitialized data error in operator<<(ostream,fixed_point)
        std::array<char, max_num_chars+1> chars{};
#else
        std::array<char, max_num_chars+1> chars;
#endif

        auto result = to_chars(chars.data(), chars.data()+max_num_chars, value);
        CNL_ASSERT(result.ptr>chars.data());
        CNL_ASSERT(result.ptr<=chars.data()+max_num_chars);
        CNL_ASSERT(result.ec==std::errc{});

        *result.ptr = '\0';
        return chars;
    }
}

#endif  // CNL_IMPL_TO_CHARS_H
