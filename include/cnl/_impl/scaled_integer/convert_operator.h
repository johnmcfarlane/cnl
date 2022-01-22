
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H

#include "../../integer.h"
#include "../num_traits/to_rep.h"
#include "../overflow/overflow_operator.h"
#include "../power_value.h"
#include "../rounding/native_rounding_tag.h"
#include "../rounding/nearest_rounding_tag.h"
#include "../rounding/neg_inf_rounding_tag.h"
#include "../rounding/tie_to_pos_inf_rounding_tag.h"
#include "../scaled/is_scaled_tag.h"
#include "definition.h"
#include "from_rep.h"

#include <concepts>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////
    /// cnl::nearest_rounding_tag

    template<typename Input, typename Result, int Radix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, nearest_rounding_tag>,
            op_value<Result, power<0, Radix>>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<Input, power<0, Radix>>,
                  op_value<Result, nearest_rounding_tag>> {
    };

    /// \cond
    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    requires(!(ResultExponent <= InputExponent)) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, power<0, Radix>>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, nearest_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, Radix>>;
        using input = scaled_integer<InputRep, power<InputExponent, Radix>>;

        [[nodiscard]] static constexpr auto half()
        {
            return static_cast<input>(_impl::from_rep<result>(1)) / 2;
        }

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<result>(from + ((from >= 0) ? half() : -half()));
        }
    };

    // conversion between two scaled_integer types where rounding *isn't* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    requires(ResultExponent <= InputExponent) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, power<0, Radix>>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, nearest_rounding_tag>> {
    };
    /// \endcond

    // conversion from float to scaled_integer
    template<std::floating_point Input, typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, power<0, ResultRadix>>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>;

        [[nodiscard]] static constexpr auto half()
        {
            return _impl::power_value<Input, ResultExponent - 1, ResultRadix>();
        }

    public:
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<result>(from + ((from >= 0) ? half() : -half()));
        }
    };

    template<integer Input, typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, power<0, ResultRadix>>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<Input>, power<0, ResultRadix>>,
                  op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>> {
    };

    template<typename InputRep, int InputExponent, int InputRadix, integer Result>
    struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, InputRadix>>, power<0, InputRadix>>,
            op_value<Result, nearest_rounding_tag>> {
    private:
        using input = scaled_integer<InputRep, power<InputExponent, InputRadix>>;

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const
        {
            return _impl::to_rep(custom_operator<
                                 _impl::convert_op,
                                 op_value<input, power<0, InputRadix>>,
                                 op_value<scaled_integer<Result>, nearest_rounding_tag>>{}(from));
        }
    };

    template<typename Input, typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>> {

        [[nodiscard]] constexpr auto
        operator()(Input const& from) const
        {
            return custom_operator<
                    _impl::convert_op,
                    op_value<Input, power<0, ResultRadix>>,
                    op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>>{}(from);
        }
    };

    ////////////////////////////////////////////////////////
    /// cnl::tie_to_pos_inf_rounding_tag

    // conversion between two scaled_integer types where rounding *isn't* an issue
    /// \cond
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    requires(ResultExponent <= InputExponent) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, tie_to_pos_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
                  op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, native_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    requires(!(ResultExponent <= InputExponent)) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, tie_to_pos_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, Radix>>;
        using input = scaled_integer<InputRep, power<InputExponent, Radix>>;

        [[nodiscard]] static constexpr auto half()
        {
            return static_cast<input>(_impl::from_rep<result>(1)) / 2;
        }

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const -> result
        {
            // TODO: unsigned specialization
            return _impl::from_rep<result>(
                    _impl::to_rep(from + half()) >> (ResultExponent - InputExponent));
        }
    };
    /// \endcond

    // conversion from float to scaled_integer
    template<
            std::floating_point Input,
            typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, tie_to_pos_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>;

        [[nodiscard]] static constexpr auto half()
        {
            return _impl::power_value<Input, ResultExponent - 1, ResultRadix>();
        }

    public:
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<result>(from + half());
        }
    };

    template<integer Input, integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<Input>, _impl::native_tag>,
                  op_value<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>> {
    };

    template<integer InputRep, scaled_tag InputScale, integer Result>
    struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            op_value<Result, tie_to_pos_inf_rounding_tag>> {
    private:
        using input = scaled_integer<InputRep, InputScale>;

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const
        {
            return _impl::to_rep(custom_operator<
                                 _impl::convert_op,
                                 op_value<input, _impl::native_tag>,
                                 op_value<scaled_integer<Result>, tie_to_pos_inf_rounding_tag>>{}(from));
        }
    };

    ////////////////////////////////////////////////////////
    /// cnl::neg_inf_rounding_tag

    // conversion between two scaled_integer types where rounding *isn't* an issue
    /// \cond
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    requires(ResultExponent <= InputExponent) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, neg_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
                  op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, native_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    requires(!(ResultExponent <= InputExponent)) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, Radix>>, neg_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, Radix>>;
        using input = scaled_integer<InputRep, power<InputExponent, Radix>>;

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const
        {
            // TODO: unsigned specialization
            return _impl::from_rep<result>(
                    _impl::to_rep(from) >> (ResultExponent - InputExponent));
        }
    };
    /// \endcond

    // conversion from float to scaled_integer
    template<
            std::floating_point Input,
            typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, neg_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>;

    public:
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<result>(from);
        }
    };

    template<integer Input, integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<Input>, _impl::native_tag>,
                  op_value<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>> {
    };

    template<integer InputRep, scaled_tag InputScale, integer Result>
    struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            op_value<Result, neg_inf_rounding_tag>> {
    private:
        using input = scaled_integer<InputRep, InputScale>;

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const -> Result
        {
            return _impl::to_rep(custom_operator<
                                 _impl::convert_op,
                                 op_value<input, _impl::native_tag>,
                                 op_value<scaled_integer<Result>, neg_inf_rounding_tag>>{}(from));
        }
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H
