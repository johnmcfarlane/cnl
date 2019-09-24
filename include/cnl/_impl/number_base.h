
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_NUMBER_BASE_H)
#define CNL_NUMBER_BASE_H 1

#include "number_base/declaration.h"
#include "number_base/definition.h"
#include "number_base/is_derived_from_number_base.h"
#include "number_base/to_string.h"
#include "num_traits/from_rep.h"
#include "num_traits/from_value.h"
#include "num_traits/is_composite.h"
#include "num_traits/scale.h"
#include "num_traits/to_rep.h"
#include "operators/overloads.h"
#include "../limits.h"

#include <type_traits>
#include <utility>

namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::depth

        template<class Wrapper, bool IsComposite = is_composite<Wrapper>::value>
        struct depth;

        template<class Wrapper>
        struct depth<Wrapper, true> {
            using _rep = _impl::rep_t<Wrapper>;
            static constexpr auto value = depth<_rep>::value + 1;
        };

        template<class T>
        struct depth<T, false> : std::integral_constant<int, 0> {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_be_wrapper

        template<typename Wrapper>
        struct can_be_wrapper : is_derived_from_number_base<Wrapper> {
        };

        template<typename Wrapper, int WrapperN>
        struct can_be_wrapper<Wrapper[WrapperN]> : std::false_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_be_wrapped

        template<typename Rep>
        struct can_be_wrapped : std::integral_constant<bool,
                cnl::numeric_limits<Rep>::is_specialized && !std::is_floating_point<Rep>::value> {
        };

        template<typename Rep, int RepN>
        struct can_be_wrapped<Rep[RepN]> : std::false_type {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_same_wrapper

        template<typename T1, typename T2>
        struct is_same_wrapper : std::integral_constant<bool,
                std::is_same<from_value_t<T1, int>, from_value_t<T2, int>>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_wrap

        template<typename Wrapper, typename Rep>
        struct can_wrap
                : std::integral_constant<bool, can_be_wrapped<Rep>::value
                        && can_be_wrapper<Wrapper>::value
                        && !is_same_wrapper<Wrapper, Rep>::value
                        && (depth<Rep>::value < depth<Wrapper>::value)> {};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::binary_operator

    // higher OP number_base<>
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<std::is_floating_point<Lhs>::value && _impl::is_derived_from_number_base<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, static_cast<Lhs>(rhs)))
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // number_base<> OP higher
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_derived_from_number_base<Lhs>::value && std::is_floating_point<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(static_cast<Rhs>(lhs), rhs))
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP number_base<>
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::can_wrap<Rhs, Lhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(_impl::from_value<Rhs>(lhs), rhs))
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // number_base<> OP lower
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::can_wrap<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, _impl::from_value<Lhs>(rhs)))
        {
            return Operator()(lhs, _impl::from_value<Lhs>(rhs));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::comparison_operator

    // higher OP number_base<>
    template<class Operator, class Lhs, class Rhs>
    struct comparison_operator<
            Operator, Lhs, Rhs,
            _impl::enable_if_t<std::is_floating_point<Lhs>::value && _impl::is_derived_from_number_base<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, static_cast<Lhs>(rhs)))
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // number_base<> OP higher
    template<class Operator, class Lhs, class Rhs>
    struct comparison_operator<
            Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_derived_from_number_base<Lhs>::value && std::is_floating_point<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(static_cast<Rhs>(lhs), rhs))
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP number_base<>
    template<class Operator, class Lhs, class Rhs>
    struct comparison_operator<
            Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::can_wrap<Rhs, Lhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(_impl::from_value<Rhs>(lhs), rhs))
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // number_base<> OP lower
    template<class Operator, class Lhs, class Rhs>
    struct comparison_operator<
            Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::can_wrap<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, _impl::from_value<Lhs>(rhs)))
        {
            return Operator()(lhs, _impl::from_value<Lhs>(rhs));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::pre_operator

    // number_base<> OP lower
    template<class Operator, class Derived, typename Rep>
    struct pre_operator<Operator, _impl::native_tag, _impl::number_base<Derived, Rep>> {
        CNL_RELAXED_CONSTEXPR Derived& operator()(Derived& rhs) const
        {
            Operator()(_impl::to_rep(rhs));
            return rhs;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::post_operator

    // number_base<> OP lower
    template<class Operator, class Derived, typename Rep>
    struct post_operator<Operator, _impl::native_tag, _impl::number_base<Derived, Rep>> {
        CNL_RELAXED_CONSTEXPR Derived operator()(Derived& lhs) const
        {
            auto copy = lhs;
            Operator()(_impl::to_rep(lhs));
            return copy;
        }
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops

        template<class Number>
        struct wants_generic_ops<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // _impl::number_base<> numeric traits

    template<class Number>
    struct is_composite<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
    };

    template<class Derived, class Rep>
    struct to_rep<_impl::number_base<Derived, Rep>> {
        CNL_NODISCARD constexpr Rep& operator()(Derived& number) const {
            return number._rep;
        }
        CNL_NODISCARD constexpr Rep const& operator()(Derived const& number) const {
            return number._rep;
        }
        CNL_NODISCARD constexpr Rep&& operator()(Derived&& number) const {
            return std::forward<Rep>(number._rep);
        }
    };

    template<class Derived>
    struct to_rep<Derived, _impl::enable_if_t<_impl::is_derived_from_number_base<Derived>::value>>
            : to_rep<_impl::number_base<Derived, typename Derived::rep>> {
    };

    template<int Digits, int Radix, class Derived>
    struct scale<Digits, Radix, _impl::number_base<Derived, typename Derived::rep>> {
        using _scalar_type = _impl::number_base<Derived, typename Derived::rep>;

        CNL_NODISCARD constexpr auto operator()(Derived const &s) const
        -> decltype(_impl::from_rep<Derived>(_impl::scale<Digits, Radix>(_impl::to_rep(s))))
        {
            return _impl::from_rep<Derived>(_impl::scale<Digits, Radix>(_impl::to_rep(s)));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for cnl::_impl::numeric_limits

    template<class Derived, class Rep>
    struct numeric_limits<cnl::_impl::number_base<Derived, Rep>>
    : numeric_limits<Rep> {
        // number_base-specific helpers
        using _value_type = Derived;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members

        CNL_NODISCARD static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::min());
        }

        CNL_NODISCARD static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }

        CNL_NODISCARD static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }

        CNL_NODISCARD static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::round_error());
        }

        CNL_NODISCARD static constexpr _value_type round_error() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::round_error());
        }

        CNL_NODISCARD static constexpr _value_type infinity() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::infinity());
        }

        CNL_NODISCARD static constexpr _value_type quiet_NaN() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::quiet_NaN());
        }

        CNL_NODISCARD static constexpr _value_type signaling_NaN() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::signaling_NaN());
        }

        CNL_NODISCARD static constexpr _value_type denorm_min() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::denorm_min());
        }
    };
}

#endif  // CNL_NUMBER_BASE_H
