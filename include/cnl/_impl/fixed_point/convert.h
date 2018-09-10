
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FIXED_POINT_CONVERT_H)
#define CNL_IMPL_FIXED_POINT_CONVERT_H

#include "../rounding/convert.h"
#include "../rounding/rounding_tag.h"
#include "../type_traits/enable_if.h"
#include "declaration.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    // conversion between two fixed_point types where rounding *isn't* an issue
    template<
            typename ResultRep, int ResultExponent,
            typename InputRep, int InputExponent,
            int Radix>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, Radix>,
            fixed_point<InputRep, InputExponent, Radix>,
            _impl::enable_if_t<ResultExponent<=InputExponent>>  // If result has no fewer fractional digits
            : convert<
                    _impl::native_rounding_tag,  // defer to native specialization.
                    fixed_point<ResultRep, ResultExponent, Radix>,
                    fixed_point<InputRep, InputExponent, Radix>> {
    };

    // conversion between two fixed_point types where rounding *is* an issue
    template<
            typename ResultRep, int ResultExponent,
            typename InputRep, int InputExponent,
            int Radix>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, Radix>,
            fixed_point<InputRep, InputExponent, Radix>,
            _impl::enable_if_t<(ResultExponent>InputExponent)>> {
    private:
        using _result = fixed_point<ResultRep, ResultExponent, Radix>;
        using _input = fixed_point<InputRep, InputExponent, Radix>;

        static constexpr _input half()
        {
            return static_cast<_input>(_impl::from_rep<_result>(1))/2;
        }

    public:
        constexpr _result operator()(_input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<_result>(from+((from>=0) ? half() : -half()));
        }
    };

    // conversion from float to fixed_point
    template<
            typename ResultRep, int ResultExponent, int ResultRadix,
            typename Input>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, ResultRadix>,
            Input,
            _impl::enable_if_t<std::is_floating_point<Input>::value>> {
    private:
        using _result = fixed_point<ResultRep, ResultExponent, ResultRadix>;

        static constexpr Input half()
        {
            return power<Input, ResultExponent-1, ResultRadix>();
        }

    public:
        constexpr _result operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<_result>(from+((from>=0) ? half() : -half()));
        }
    };

    template<
            typename ResultRep, int ResultExponent, int ResultRadix,
            typename Input>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, ResultRadix>,
            Input,
            _impl::enable_if_t<cnl::numeric_limits<Input>::is_integer>>
            : convert<
                    _impl::nearest_rounding_tag,
                    fixed_point<ResultRep, ResultExponent, ResultRadix>,
                    fixed_point<Input>> {
    };

    template<
            typename Result,
            typename InputRep, int InputExponent, int InputRadix>
    struct convert<
            _impl::nearest_rounding_tag,
            Result,
            fixed_point<InputRep, InputExponent, InputRadix>,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer>> {
        using _input = fixed_point<InputRep, InputExponent, InputRadix>;
        constexpr Result operator()(_input const& from) const
        {
            return _impl::to_rep(_impl::convert<_impl::nearest_rounding_tag, fixed_point<Result>>(from));
        }
    };
}

#endif  // CNL_IMPL_FIXED_POINT_CONVERT_H
