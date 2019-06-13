
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H

#include "../power_value.h"
#include "../overflow/overflow_operator.h"
#include "../rounding/nearest_rounding_tag.h"
#include "../rounding/native_rounding_tag.h"
#include "../type_traits/enable_if.h"
#include "declaration.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // conversion between two scaled_integer types where rounding *isn't* an issue
        template<
                typename ResultRep, int ResultExponent,
                typename InputRep, int InputExponent,
                int Radix>
        struct tagged_convert_operator<
                nearest_rounding_tag,
                scaled_integer<ResultRep, ResultExponent, Radix>,
                scaled_integer<InputRep, InputExponent, Radix>,
                _impl::enable_if_t<(ResultExponent <= InputExponent)>>
                : tagged_convert_operator<
                        native_rounding_tag,
                        scaled_integer<ResultRep, ResultExponent, Radix>,
                        scaled_integer<InputRep, InputExponent, Radix>> {
        };

        // conversion between two scaled_integer types where rounding *is* an issue
        template<
                typename ResultRep, int ResultExponent,
                typename InputRep, int InputExponent,
                int Radix>
        struct tagged_convert_operator<
                nearest_rounding_tag,
                scaled_integer<ResultRep, ResultExponent, Radix>,
                scaled_integer<InputRep, InputExponent, Radix>,
                _impl::enable_if_t<!(ResultExponent<=InputExponent)>> {
        private:
            using _result = scaled_integer<ResultRep, ResultExponent, Radix>;
            using _input = scaled_integer<InputRep, InputExponent, Radix>;

            CNL_NODISCARD static constexpr _input half()
            {
                return static_cast<_input>(_impl::from_rep<_result>(1))/2;
            }

        public:
            CNL_NODISCARD constexpr _result operator()(_input const& from) const
            {
                // TODO: unsigned specialization
                return static_cast<_result>(from+((from >= 0) ? half() : -half()));
            }
        };

        // conversion from float to scaled_integer
        template<
                typename ResultRep, int ResultExponent, int ResultRadix,
                typename Input>
        struct tagged_convert_operator<
                nearest_rounding_tag,
                scaled_integer<ResultRep, ResultExponent, ResultRadix>,
                Input,
                _impl::enable_if_t<std::is_floating_point<Input>::value>> {
        private:
            using _result = scaled_integer<ResultRep, ResultExponent, ResultRadix>;

            CNL_NODISCARD static constexpr Input half()
            {
                return power_value<Input, ResultExponent-1, ResultRadix>();
            }

        public:
            CNL_NODISCARD constexpr _result operator()(Input const& from) const
            {
                // TODO: unsigned specialization
                return static_cast<_result>(from+((from >= 0) ? half() : -half()));
            }
        };

        template<
                typename ResultRep, int ResultExponent, int ResultRadix,
                typename Input>
        struct tagged_convert_operator<
                nearest_rounding_tag,
                scaled_integer<ResultRep, ResultExponent, ResultRadix>,
                Input,
                _impl::enable_if_t<cnl::numeric_limits<Input>::is_integer>>
                : tagged_convert_operator<
                        nearest_rounding_tag,
                        scaled_integer<ResultRep, ResultExponent, ResultRadix>,
                        scaled_integer<Input>> {
        };

        template<
                typename Result,
                typename InputRep, int InputExponent, int InputRadix>
        struct tagged_convert_operator<
                nearest_rounding_tag,
                Result,
                scaled_integer<InputRep, InputExponent, InputRadix>,
                _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer>> {
            using _input = scaled_integer<InputRep, InputExponent, InputRadix>;

            CNL_NODISCARD constexpr Result operator()(_input const& from) const
            {
                return _impl::to_rep(
                        _impl::tagged_convert_operator<nearest_rounding_tag, scaled_integer<Result>, _input>{}(
                                from));
            }
        };
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H
