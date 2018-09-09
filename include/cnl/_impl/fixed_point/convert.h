
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
}

#endif  // CNL_IMPL_FIXED_POINT_CONVERT_H
