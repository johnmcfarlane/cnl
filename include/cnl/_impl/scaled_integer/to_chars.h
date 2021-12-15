
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TO_CHARS_H)
#define CNL_IMPL_SCALED_INTEGER_TO_CHARS_H

#include "../../integer.h"
#include "../../rounding_integer.h"
#include "../charconv/constants.h"
#include "../charconv/descale.h"
#include "../cnl_assert.h"
#include "../cstdint/types.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/to_rep.h"
#include "../scaled/power.h"
#include "../ssize.h"
#include "../ssizeof.h"
#include "definition.h"
#include "num_traits.h"
#include "numbers.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <iterator>
#include <span>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        constexpr auto num_digits_from_binary(int num_digits, int radix)
        {
            switch (radix) {
            case 2:
                return num_digits;
            case 8:
                return (num_digits + 2) / 3;
            case 10:
                return (num_digits * 1000 + 3322) / 3321;
            case 16:
                return (num_digits + 3) / 4;
            default: {
                auto const binary_digits_per_digit{used_digits(radix - 1)};
                return (num_digits + binary_digits_per_digit - 1) / binary_digits_per_digit;
            }
            }
        }

        constexpr auto num_digits_to_binary(int num_digits, int radix)
        {
            switch (radix) {
            case 2:
                return num_digits;
            case 8:
                return num_digits * 3;
            case 10:
                return (num_digits * 3322 + 678) / 1000;
            case 16:
                return num_digits * 4;
            default:
                return num_digits * used_digits(radix - 1);
            }
        }

        template<typename Rep, int Exponent, int Radix>
        struct max_to_chars_chars<scaled_integer<Rep, power<Exponent, Radix>>> {
        private:
            using scalar = cnl::scaled_integer<Rep, power<Exponent, Radix>>;

            // This number is a little pessemistic in the case that Radix != 2.
            static constexpr auto _fractional_digits =
                    std::max(cnl::_impl::fractional_digits<scalar>, 0);

            static constexpr auto _sign_chars = static_cast<int>(cnl::numbers::signedness_v<scalar>);
            static constexpr auto _num_significant_integer_bits{cnl::digits_v<scalar> - _fractional_digits};
            static constexpr auto _num_trailing_integer_bits{
                    num_digits_to_binary(std::max(0, Exponent), Radix)};
            static constexpr auto _num_integer_bits{
                    _num_significant_integer_bits + _num_trailing_integer_bits};
            static constexpr auto _integer_chars = num_digits_from_binary(_num_integer_bits, 10);
            static constexpr auto _radix_chars = static_cast<int>(_fractional_digits > 0);
            static constexpr auto _fractional_chars = std::max(0, _fractional_digits);

        public:
            static constexpr auto value =
                    _sign_chars + _integer_chars + _radix_chars + _fractional_chars;
        };

        struct descaled_info {
            std::string_view significand_digits;
            std::string_view exponent_chars;
            std::span<char> output;
            int num_significand_digits{};
            int exponent{};
            int max_chars{};
            bool exponent_has_sign{};
        };

        struct fixed_solution {
            int num_significand_digits;
            int num_chars;
            int leading_zeros;
            int trailing_zeros;
            bool has_radix;
        };

        [[nodiscard]] constexpr auto solve_fixed(descaled_info const& info)
        {
            auto const num_integer_digits{info.num_significand_digits + info.exponent};
            if (num_integer_digits > info.max_chars) {
                return fixed_solution{};
            }

            auto const leading_zeros{std::max(0, -num_integer_digits)};
            auto const has_radix{info.exponent < 0};
            auto const trailing_zeros{std::max(0, info.exponent)};
            auto const unbounded_num_chars{info.num_significand_digits + leading_zeros + has_radix + trailing_zeros};
            auto const chars_truncated{std::max(0, unbounded_num_chars - info.max_chars)};
            auto const truncated_significand_digits{info.num_significand_digits - chars_truncated};
            auto const truncated_num_chars{unbounded_num_chars - chars_truncated};

            return fixed_solution{
                    truncated_significand_digits,
                    truncated_num_chars,
                    leading_zeros,
                    trailing_zeros,
                    has_radix};
        }

        [[nodiscard]] constexpr auto fill(
                descaled_info const& info,
                fixed_solution const& solution)
        {
            auto* const output_begin{std::data(info.output)};
            auto* const output_end{output_begin + std::ssize(info.output)};
            auto out{output_begin};

            auto significand_digits_first = std::begin(info.significand_digits);

            // copy part of significand before the period, including the sign
            for (auto n{std::max(0, info.num_significand_digits + std::min(0, info.exponent))};
                 n;
                 --n) {
                CNL_ASSERT(*significand_digits_first);
                *out++ = *significand_digits_first++;
            }

            if (solution.trailing_zeros) {
                out = std::fill_n(out, solution.trailing_zeros, zero_char);
            } else if (out < output_end) {  // NOLINT(hicpp-use-nullptr,modernize-use-nullptr)
                if (solution.has_radix) {
                    *out++ = radix_char;
                }

                out = std::fill_n(out, solution.leading_zeros, zero_char);

                // copy part of significand after the period
                out = std::copy(
                        significand_digits_first,
                        std::next(std::begin(info.significand_digits), solution.num_significand_digits),
                        out);
            }

            CNL_ASSERT(significand_digits_first <= std::end(info.significand_digits));
            CNL_ASSERT(out == output_begin + solution.num_chars);
            CNL_ASSERT(out <= output_end);  // NOLINT(hicpp-use-nullptr,modernize-use-nullptr)
            return std::to_chars_result{out, std::errc{}};
        }

        struct scientific_solution {
            int num_significand_digits;
            int num_chars;
        };

        [[nodiscard]] constexpr auto solve_scientific(descaled_info const& info)
        {
            auto const num_exponent_chars{_impl::ssize(info.exponent_chars)};

            auto const unbounded_num_chars{
                    info.num_significand_digits + _impl::ssizeof(radix_char) + _impl::ssizeof(e_char) + num_exponent_chars};
            auto const chars_truncated{std::max(0, unbounded_num_chars - info.max_chars)};

            return scientific_solution{
                    info.num_significand_digits - chars_truncated,
                    unbounded_num_chars - chars_truncated};
        }

        [[nodiscard]] constexpr auto fill(
                descaled_info const& info,
                scientific_solution const& solution)
        {
            auto* const output_begin{std::data(info.output)};
            auto* const output_end{output_begin + std::ssize(info.output)};
            auto out{output_begin};

            auto significand_digits_first = std::begin(info.significand_digits);

            // copy part of significand after the period
            while (!isdigit(*out++ = *significand_digits_first++)) {
                CNL_ASSERT(out < output_end);  // NOLINT(hicpp-use-nullptr,modernize-use-nullptr)
            }

            // add the radix point
            *out++ = radix_char;

            // copy part of significand after the period
            out = std::copy(
                    significand_digits_first,
                    std::next(std::begin(info.significand_digits), solution.num_significand_digits),
                    out);

            // add the 'e'
            *out++ = e_char;

            // copy exponent value
            out = std::copy(
                    std::begin(info.exponent_chars),
                    std::next(std::begin(info.exponent_chars), _impl::ssize(info.exponent_chars)),
                    out);

            CNL_ASSERT(out == output_begin + solution.num_chars);
            CNL_ASSERT(out <= output_end);  // NOLINT(hicpp-use-nullptr,modernize-use-nullptr)
            return std::to_chars_result{out, std::errc{}};
        }

        [[nodiscard]] inline constexpr auto to_chars_positive(
                char* const first,
                char* const last,
                std::string_view const& significand_digits,
                int exponent)
        {
            _impl::descaled_info info;
            info.significand_digits = significand_digits;
            info.output = std::span<char>{first, last};
            info.num_significand_digits = _impl::ssize(info.significand_digits);
            info.exponent = exponent;
            info.max_chars = _impl::ssize(info.output);

            auto const exponent_chars_static{to_chars_static(exponent + info.num_significand_digits - 1)};
            info.exponent_chars = std::string_view(
                    exponent_chars_static.chars.data(),
                    exponent_chars_static.length);
            info.exponent_has_sign = info.exponent_chars[0] == _impl::minus_char;

            CNL_ASSERT(isdigit(info.exponent_chars[0]) || info.exponent_chars[0] == _impl::minus_char);

            auto const scientific_solution{_impl::solve_scientific(info)};
            auto const fixed_solution{_impl::solve_fixed(info)};

            if (std::tuple{
                        scientific_solution.num_significand_digits,
                        -scientific_solution.num_chars}
                > std::tuple{// NOLINT(hicpp-use-nullptr,modernize-use-nullptr)
                             fixed_solution.num_significand_digits,
                             -fixed_solution.num_chars}) {
                CNL_ASSERT(scientific_solution.num_significand_digits > 0);
                return _impl::fill(info, scientific_solution);
            }

            if (fixed_solution.num_significand_digits > 0) {
                return _impl::fill(info, fixed_solution);
            }

            return std::to_chars_result{last, std::errc::value_too_large};
        }

        template<integer Significand, int Radix>
        [[nodiscard]] constexpr auto to_chars_non_zero(
                char* const first,
                char* const last,
                descaled<Significand, Radix> const& descaled)
        {
            CNL_ASSERT(descaled.significand);

            auto const significand_chars_static{to_chars_static(descaled.significand)};
            auto const significand_chars_cstr{significand_chars_static.chars.data()};
            if (*significand_chars_cstr == minus_char) {
                *first = minus_char;
                return to_chars_positive(first + 1, last, std::string_view(significand_chars_cstr + 1, significand_chars_static.length - 1), descaled.exponent);
            }

            return to_chars_positive(first, last, std::string_view(significand_chars_cstr, significand_chars_static.length), descaled.exponent);
        }
    }

    // a partial implementation of std::to_chars overloaded on cnl::scaled_integer;
    // known to exhibit rounding errors; not yet tested with Radix!=2
    template<integer Rep, int Exponent, int Radix>
    [[nodiscard]] inline constexpr auto to_chars(
            char* const first,
            char* const last,
            cnl::scaled_integer<Rep, power<Exponent, Radix>> const& value)
    {
        if (first == last) {
            // buffer too small to contain "0"
            return std::to_chars_result{last, std::errc::value_too_large};
        }

        if (!value) {
            // zero
            *first = _impl::zero_char;
            return std::to_chars_result{first + 1, std::errc{}};
        }

        using significand_type = std::conditional_t<(digits_v<Rep> > digits_v<std::int64_t>), Rep, std::int64_t>;
        auto const descaled{_impl::descale<significand_type, 10>(
                _impl::to_rep(value), power<Exponent, Radix>{})};

        return _impl::to_chars_non_zero(first, last, descaled);
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_TO_CHARS_H
