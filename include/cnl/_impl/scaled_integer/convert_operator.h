
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H

#include "../../floating_point.h"
#include "../../integer.h"
#include "../overflow/overflow_operator.h"
#include "../power_value.h"
#include "../rounding/native_rounding_tag.h"
#include "../rounding/nearest_rounding_tag.h"
#include "../rounding/neg_inf_rounding_tag.h"
#include "../rounding/tie_to_pos_inf_rounding_tag.h"
#include "../scaled/is_scaled_tag.h"
#include "definition.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////
    /// cnl::nearest_rounding_tag

    template<integer Input, typename Result, scaled_tag Scale>
    requires(_impl::exponent_v<Scale> == 0) struct custom_operator<
            _impl::convert_op,
            op_value<Input, nearest_rounding_tag>,
            op_value<Result, Scale>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<Input, Scale>,
                  op_value<Result, nearest_rounding_tag>> {
    };

    /// \cond
    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            integer InputRep, scaled_tag InputScale,
            integer ResultRep, scaled_tag ResultScale>
    requires(_impl::exponent_v<InputScale> < _impl::exponent_v<ResultScale>) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, typename InputScale::identity>,
            op_value<scaled_integer<ResultRep, ResultScale>, nearest_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, ResultScale>;
        using input = scaled_integer<InputRep, InputScale>;

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
            integer InputRep, scaled_tag InputScale,
            integer ResultRep, scaled_tag ResultScale>
    requires(_impl::exponent_v<ResultScale> <= _impl::exponent_v<InputScale>) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, typename InputScale::identity>,
            op_value<scaled_integer<ResultRep, ResultScale>, nearest_rounding_tag>> {
    };
    /// \endcond

    // conversion from float to scaled_integer
    template<floating_point Input, integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, typename ResultScale::identity>,
            op_value<scaled_integer<ResultRep, ResultScale>, nearest_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, ResultScale>;

        [[nodiscard]] static constexpr auto half()
        {
            return _impl::power_value<
                    Input,
                    _impl::exponent_v<ResultScale> - 1,
                    _impl::radix_v<ResultScale>>();
        }

    public:
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            // TODO: unsigned specialization
            return static_cast<result>(from + ((from >= 0) ? half() : -half()));
        }
    };

    template<integer Input, integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, typename ResultScale::identity>,
            op_value<scaled_integer<ResultRep, ResultScale>, nearest_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<
                          scaled_integer<Input, typename ResultScale::identity>,
                          typename ResultScale::identity>,
                  op_value<
                          scaled_integer<ResultRep, ResultScale>,
                          nearest_rounding_tag>> {
    };

    template<integer InputRep, scaled_tag InputScale, integer Result>
    struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, typename InputScale::identity>,
            op_value<Result, nearest_rounding_tag>> {
    private:
        using input = scaled_integer<InputRep, InputScale>;

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const
        {
            return _impl::to_rep(
                    custom_operator<
                            _impl::convert_op,
                            op_value<
                                    input,
                                    typename InputScale::identity>,
                            op_value<
                                    scaled_integer<Result, typename InputScale::identity>,
                                    nearest_rounding_tag>>{}(from));
        }
    };

    template<typename Input, integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, nearest_rounding_tag>> {

        [[nodiscard]] constexpr auto
        operator()(Input const& from) const
        {
            return custom_operator<
                    _impl::convert_op,
                    op_value<Input, typename ResultScale::identity>,
                    op_value<scaled_integer<ResultRep, ResultScale>, nearest_rounding_tag>>{}(from);
        }
    };

    ////////////////////////////////////////////////////////
    /// cnl::tie_to_pos_inf_rounding_tag

    // conversion between two scaled_integer types where rounding *isn't* an issue
    /// \cond
    template<
            integer InputRep, scaled_tag InputScale,
            integer ResultRep, scaled_tag ResultScale>
    requires(_impl::exponent_v<InputScale> >= _impl::exponent_v<ResultScale>) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
                  op_value<scaled_integer<ResultRep, ResultScale>, native_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            integer InputRep, scaled_tag InputScale,
            integer ResultRep, scaled_tag ResultScale>
    requires(_impl::exponent_v<InputScale> < _impl::exponent_v<ResultScale>) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, ResultScale>;
        using input = scaled_integer<InputRep, InputScale>;

        [[nodiscard]] static constexpr auto half()
        {
            return static_cast<input>(_impl::from_rep<result>(1)) / 2;
        }

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const -> result
        {
            // TODO: unsigned specialization
            return _impl::from_rep<result>(
                    _impl::to_rep(from + half())
                    >> (_impl::exponent_v<ResultScale> - _impl::exponent_v<InputScale>));
        }
    };
    /// \endcond

    // conversion from float to scaled_integer
    template<
            floating_point Input,
            integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, tie_to_pos_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, ResultScale>;

        [[nodiscard]] static constexpr auto half()
        {
            return _impl::power_value<
                    Input,
                    _impl::exponent_v<ResultScale> - 1,
                    _impl::radix_v<ResultScale>>();
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
                  op_value<
                          scaled_integer<Input, typename ResultScale::identity>,
                          _impl::native_tag>,
                  op_value<
                          scaled_integer<ResultRep, ResultScale>,
                          tie_to_pos_inf_rounding_tag>> {
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
                                 op_value<
                                         input,
                                         _impl::native_tag>,
                                 op_value<
                                         scaled_integer<Result, typename InputScale::identity>,
                                         tie_to_pos_inf_rounding_tag>>{}(from));
        }
    };

    ////////////////////////////////////////////////////////
    /// cnl::neg_inf_rounding_tag

    // conversion between two scaled_integer types where rounding *isn't* an issue
    /// \cond
    template<
            integer InputRep, scaled_tag InputScale,
            integer ResultRep, scaled_tag ResultScale>
    requires(_impl::exponent_v<ResultScale> <= _impl::exponent_v<InputScale>) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
                  op_value<scaled_integer<ResultRep, ResultScale>, native_rounding_tag>> {
    };

    // conversion between two scaled_integer types where rounding *is* an issue
    template<
            integer InputRep, scaled_tag InputScale,
            integer ResultRep, scaled_tag ResultScale>
    requires(!(_impl::exponent_v<ResultScale> <= _impl::exponent_v<InputScale>)) struct custom_operator<
            _impl::convert_op,
            op_value<scaled_integer<InputRep, InputScale>, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, ResultScale>;
        using input = scaled_integer<InputRep, InputScale>;

    public:
        [[nodiscard]] constexpr auto operator()(input const& from) const
        {
            // TODO: unsigned specialization
            return _impl::from_rep<result>(
                    _impl::to_rep(from)
                    >> (_impl::exponent_v<ResultScale> - _impl::exponent_v<InputScale>));
        }
    };
    /// \endcond

    // conversion from float to scaled_integer
    template<
            floating_point Input,
            integer ResultRep, scaled_tag ResultScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, _impl::native_tag>,
            op_value<scaled_integer<ResultRep, ResultScale>, neg_inf_rounding_tag>> {
    private:
        using result = scaled_integer<ResultRep, ResultScale>;

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
                  op_value<
                          scaled_integer<Input, typename ResultScale::identity>,
                          _impl::native_tag>,
                  op_value<
                          scaled_integer<ResultRep, ResultScale>,
                          neg_inf_rounding_tag>> {
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
                                 op_value<
                                         input,
                                         _impl::native_tag>,
                                 op_value<
                                         scaled_integer<Result, typename InputScale::identity>,
                                         neg_inf_rounding_tag>>{}(from));
        }
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_TAGGED_CONVERT_OPERATOR_H
