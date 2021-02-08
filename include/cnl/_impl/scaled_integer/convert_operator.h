
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H

#include "../../limits.h"
#include "../overflow/overflow_operator.h"
#include "../power_value.h"
#include "../rounding/native_rounding_tag.h"
#include "../rounding/nearest_rounding_tag.h"
#include "../rounding/neg_inf_rounding_tag.h"
#include "../rounding/tie_to_pos_inf_rounding_tag.h"
#include "../type_traits/enable_if.h"
#include "declaration.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////
    /// cnl::nearest_rounding_tag

    template<typename Input, typename Result, int Radix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, nearest_rounding_tag>,
            operand<Result, power<0, Radix>>>
        : custom_operator<
                  _impl::convert_op,
                  operand<Input, power<0, Radix>>,
                  operand<Result, nearest_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    /// \cond
    requires(!(ResultExponent <= InputExponent))
    /// \endcond
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, Radix>>, power<0, Radix>>,
            operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, nearest_rounding_tag>> {
    private:
        using _result = scaled_integer<ResultRep, power<ResultExponent, Radix>>;
        using _input = scaled_integer<InputRep, power<InputExponent, Radix>>;

        CNL_NODISCARD static constexpr _input half()
        {
            return static_cast<_input>(_impl::from_rep<_result>(1)) / 2;
        }

    public:
        CNL_NODISCARD constexpr _result operator()(_input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<_result>(from + ((from >= 0) ? half() : -half()));
        }
    };

    // conversion between two scaled_integer types where rounding *isn't* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    /// \cond
    requires(ResultExponent <= InputExponent)
    /// \endcond
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, Radix>>, power<0, Radix>>,
            operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, nearest_rounding_tag>> {
    };

    // conversion from float to scaled_integer
    template<_impl::floating_point Input, typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, power<0, ResultRadix>>,
            operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>;

        CNL_NODISCARD static constexpr Input half()
        {
            return _impl::power_value<Input, ResultExponent - 1, ResultRadix>();
        }

    public:
        CNL_NODISCARD constexpr result operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<result>(from + ((from >= 0) ? half() : -half()));
        }
    };

    template<_impl::integer Input, typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, power<0, ResultRadix>>,
            operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  operand<scaled_integer<Input>, power<0, ResultRadix>>,
                  operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>> {
    };

    template<typename InputRep, int InputExponent, int InputRadix, _impl::integer Result>
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, InputRadix>>, power<0, InputRadix>>,
            operand<Result, nearest_rounding_tag>> {
        using _input = scaled_integer<InputRep, power<InputExponent, InputRadix>>;

        CNL_NODISCARD constexpr Result operator()(_input const& from) const
        {
            return _impl::to_rep(custom_operator<
                                 _impl::convert_op,
                                 operand<_input, power<0, InputRadix>>,
                                 operand<scaled_integer<Result>, nearest_rounding_tag>>{}(from));
        }
    };

    template<typename Input, typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>> {

        CNL_NODISCARD constexpr scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>
        operator()(Input const& from) const
        {
            return custom_operator<
                    _impl::convert_op,
                    operand<Input, power<0, ResultRadix>>,
                    operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, nearest_rounding_tag>>{}(from);
        }
    };

    ////////////////////////////////////////////////////////
    /// cnl::tie_to_pos_inf_rounding_tag

    // conversion between two scaled_integer types where rounding *isn't* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    /// \cond
    requires(ResultExponent <= InputExponent)
    /// \endcond
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, tie_to_pos_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  operand<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
                  operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, native_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    /// \cond
    requires(!(ResultExponent <= InputExponent))
    /// \endcond
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, tie_to_pos_inf_rounding_tag>> {
    private:
        using _result = scaled_integer<ResultRep, power<ResultExponent, Radix>>;
        using _input = scaled_integer<InputRep, power<InputExponent, Radix>>;

        CNL_NODISCARD static constexpr _input half()
        {
            return static_cast<_input>(_impl::from_rep<_result>(1)) / 2;
        }

    public:
        CNL_NODISCARD constexpr _result operator()(_input const& from) const
        {
            // TODO: unsigned specialization
            return _impl::from_rep<_result>(
                    _impl::to_rep(from + half()) >> (ResultExponent - InputExponent));
        }
    };

    // conversion from float to scaled_integer
    template<
            _impl::floating_point Input,
            typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, tie_to_pos_inf_rounding_tag>> {
    private:
        using _result = scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>;

        CNL_NODISCARD static constexpr Input half()
        {
            return _impl::power_value<Input, ResultExponent - 1, ResultRadix>();
        }

    public:
        CNL_NODISCARD constexpr _result operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<_result>(from + half());
        }
    };

    template<_impl::integer Input, typename ResultRep, class ResultScale>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, _impl::native_tag>,
            operand<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  operand<scaled_integer<Input>, _impl::native_tag>,
                  operand<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>> {
    };

    template<typename InputRep, class InputScale, _impl::integer Result>
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            operand<Result, tie_to_pos_inf_rounding_tag>> {
        using _input = scaled_integer<InputRep, InputScale>;

        CNL_NODISCARD constexpr Result operator()(_input const& from) const
        {
            return _impl::to_rep(custom_operator<
                                 _impl::convert_op,
                                 operand<_input, _impl::native_tag>,
                                 operand<scaled_integer<Result>, tie_to_pos_inf_rounding_tag>>{}(from));
        }
    };

    ////////////////////////////////////////////////////////
    /// cnl::neg_inf_rounding_tag

    // conversion between two scaled_integer types where rounding *isn't* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    /// \cond
    requires(ResultExponent <= InputExponent)
    /// \endcond
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, neg_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  operand<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
                  operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, native_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            typename InputRep, int InputExponent,
            typename ResultRep, int ResultExponent,
            int Radix>
    /// \cond
    requires(!(ResultExponent <= InputExponent))
    /// \endcond
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, power<InputExponent, Radix>>, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, Radix>>, neg_inf_rounding_tag>> {
    private:
        using _result = scaled_integer<ResultRep, power<ResultExponent, Radix>>;
        using _input = scaled_integer<InputRep, power<InputExponent, Radix>>;

    public:
        CNL_NODISCARD constexpr _result operator()(_input const& from) const
        {
            // TODO: unsigned specialization
            return _impl::from_rep<_result>(
                    _impl::to_rep(from) >> (ResultExponent - InputExponent));
        }
    };

    // conversion from float to scaled_integer
    template<
            _impl::floating_point Input,
            typename ResultRep, int ResultExponent, int ResultRadix>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, _impl::native_tag>,
            operand<scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>, neg_inf_rounding_tag>> {
    private:
        using _result = scaled_integer<ResultRep, power<ResultExponent, ResultRadix>>;

    public:
        CNL_NODISCARD constexpr _result operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<_result>(from);
        }
    };

    template<_impl::integer Input, typename ResultRep, class ResultScale>
    struct custom_operator<
            _impl::convert_op,
            operand<Input, _impl::native_tag>,
            operand<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  operand<scaled_integer<Input>, _impl::native_tag>,
                  operand<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>> {
    };

    template<typename InputRep, class InputScale, _impl::integer Result>
    struct custom_operator<
            _impl::convert_op,
            operand<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            operand<Result, neg_inf_rounding_tag>> {
        using _input = scaled_integer<InputRep, InputScale>;

        CNL_NODISCARD constexpr Result operator()(_input const& from) const
        {
            return _impl::to_rep(custom_operator<
                                 _impl::convert_op,
                                 operand<_input, _impl::native_tag>,
                                 operand<scaled_integer<Result>, neg_inf_rounding_tag>>{}(from));
        }
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H
