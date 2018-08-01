
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_NUMBER_BASE_H)
#define CNL_NUMBER_BASE_H 1

#include <cnl/bits/num_traits/from_rep.h>
#include <cnl/bits/num_traits/from_value.h>
#include <cnl/bits/num_traits/is_composite.h>
#include <cnl/bits/num_traits/scale.h>
#include <cnl/bits/num_traits/to_rep.h>
#include <cnl/bits/operators.h>
#include <cnl/bits/type_traits/make_signed.h>
#include <cnl/bits/type_traits/make_unsigned.h>
#include <cnl/constant.h>

#include <utility>

namespace cnl {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base;

        template<class Derived, class Rep>
        constexpr Rep& to_rep(number_base<Derived, Rep>& number);
        template<class Derived, class Rep>
        constexpr Rep const& to_rep(number_base<Derived, Rep> const& number);
        template<class Derived, class Rep>
        constexpr Rep&& to_rep(number_base<Derived, Rep>&& number);

        template<class Derived, class Rep>
        class number_base {
        public:
            using rep = Rep;

            explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

        protected:
            static_assert(numeric_limits<Rep>::is_integer, "number_base must be specialized with integer Rep type template parameter");

            number_base() = default;

            explicit constexpr number_base(rep const& r)
                : _rep(r) { }

            template<class T>
            CNL_RELAXED_CONSTEXPR number_base& operator=(T const& r) {
                _rep = r;
                return static_cast<Derived&>(*this);
            }

#if defined(__GNUG__) && !defined(__clang__) && (__GNUG__ <= 5)
            // GCC5 bug: https://stackoverflow.com/a/29957648/671509
            friend rep& to_rep<>(number_base&);
            friend rep const& to_rep<>(number_base const&);
            friend rep&& to_rep<>(number_base&&);
#else
            friend constexpr rep& to_rep<>(number_base&);
            friend constexpr rep const& to_rep<>(number_base const&);
            friend constexpr rep&& to_rep<>(number_base&&);
#endif

        private:
            rep _rep;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_class_derived_from_number_base

        // true iff T's base class is cnl::_impl::number_base;
        // T must be a class;
        // used by cnl::_impl::is_derived_from_number_base
        template<class Derived, class Enable = void>
        struct is_class_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_class_derived_from_number_base<const Derived> : is_class_derived_from_number_base<Derived> {};

        template<class Derived>
        struct is_class_derived_from_number_base<
                Derived,
                enable_if_t<std::is_base_of<number_base<Derived, typename Derived::rep>, Derived>::value>>
                : std::true_type {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_derived_from_number_base

        // true if T is the Derived parameter of a number_base type
        template<class T, class Enable = void>
        struct is_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_derived_from_number_base<Derived, enable_if_t<std::is_class<Derived>::value>>
        : is_class_derived_from_number_base<Derived> { };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::depth

        template<class Wrapper, class Rep = _impl::remove_cvref_t<_impl::to_rep_t<Wrapper>>>
        struct depth {
            static constexpr auto value = depth<Rep>::value + 1;
        };

        template<class T>
        struct depth<T, T> : std::integral_constant<int, 0> {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_wrappable

        template<class Rep, class Wrapper>
        struct is_wrappable;

        template<class Rep, int RepN, class Wrapper>
        struct is_wrappable<Rep[RepN], Wrapper> : std::false_type {};

        template<class Rep, class Wrapper, int WrapperN>
        struct is_wrappable<Rep, Wrapper[WrapperN]> : std::false_type {};

        template<class Rep, class Wrapper>
        struct is_wrappable : std::integral_constant<bool, cnl::numeric_limits<Rep>::is_specialized
                        && !std::is_floating_point<Rep>::value
                        && !std::is_same<from_value_t<Rep, int>, from_value_t<Wrapper, int>>::value
                        && (depth<Rep>::value < depth<Wrapper>::value)> {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::binary_operator

        // higher OP number_base<>
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<std::is_floating_point<Lhs>::value && is_derived_from_number_base<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, static_cast<Lhs>(rhs)))
            {
                return Operator()(lhs, static_cast<Lhs>(rhs));
            }
        };

        // number_base<> OP higher
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && std::is_floating_point<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(static_cast<Rhs>(lhs), rhs))
            {
                return Operator()(static_cast<Rhs>(lhs), rhs);
            }
        };

        // lower OP number_base<>
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_wrappable<Lhs, Rhs>::value && is_derived_from_number_base<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(make_number<Rhs>(lhs), rhs)) {
                return Operator()(make_number<Rhs>(lhs), rhs);
            }
        };

        // number_base<> OP lower
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && is_wrappable<Rhs, Lhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, make_number<Lhs>(rhs)))
            {
                return Operator()(lhs, make_number<Lhs>(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::pre_operator

        // number_base<> OP lower
        template<class Operator, class Derived, typename Rep>
        struct pre_operator<Operator, number_base<Derived, Rep>> {
            CNL_RELAXED_CONSTEXPR Derived& operator()(Derived& rhs) const
            {
                Operator()(to_rep(rhs));
                return rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::post_operator

        // number_base<> OP lower
        template<class Operator, class Derived, typename Rep>
        struct post_operator<Operator, number_base<Derived, Rep>> {
            CNL_RELAXED_CONSTEXPR Derived operator()(Derived& lhs) const
            {
                auto copy = lhs;
                Operator()(to_rep(lhs));
                return copy;
            }
        };

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

    namespace _impl {
        template<class Number>
        struct get_rep;

        template<class Number>
        using get_rep_t = typename get_rep<Number>::type;

        // given a Number type and an alternative Rep type, make a new Number type
        // e.g. set_rep_t<fixed_point<int64_t, 42>, uint8_t> --> fixed_point<uint8_t, 42>
        template<class Number, class NewRep, class Enable = void>
        struct set_rep;

        template<class Number, class NewRep>
        using set_rep_t = typename set_rep<Number, NewRep>::type;
    }

    template<class Number>
    struct make_signed<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        using type = _impl::set_rep_t<Number, make_signed_t<_impl::get_rep_t<Number>>>;
    };

    template<class Number>
    struct make_unsigned<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        using type = _impl::set_rep_t<Number, make_unsigned_t<_impl::get_rep_t<Number>>>;
    };

    namespace _impl {
        template<class Derived, class Rep>
        constexpr Rep& to_rep(number_base<Derived, Rep>& number) {
            return number._rep;
        }
        template<class Derived, class Rep>
        constexpr Rep const& to_rep(number_base<Derived, Rep> const& number) {
            return number._rep;
        }
        template<class Derived, class Rep>
        constexpr Rep&& to_rep(number_base<Derived, Rep>&& number) {
            return std::forward<Rep>(number._rep);
        }
    }

    template<int Digits, int Radix, class Derived>
    struct scale<Digits, Radix, _impl::number_base<Derived, typename Derived::rep>> {
        using _scalar_type = _impl::number_base<Derived, typename Derived::rep>;

        constexpr auto operator()(Derived const &s) const
        -> decltype(from_rep<Derived>{}(_impl::scale<Digits, Radix>(to_rep(s))))
        {
            return from_rep<Derived>{}(_impl::scale<Digits, Radix>(to_rep(s)));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for cnl::_impl::numeric_limits

    template<class Derived, class Rep>
    struct numeric_limits<cnl::_impl::number_base<Derived, Rep>>
    : numeric_limits<Rep> {
        // fixed-point-specific helpers
        using _value_type = Derived;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members

        static constexpr _value_type min() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::min());
        }

        static constexpr _value_type max() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::lowest());
        }

        static constexpr _value_type epsilon() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::round_error());
        }

        static constexpr _value_type round_error() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::round_error());
        }

        static constexpr _value_type infinity() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::infinity());
        }

        static constexpr _value_type quiet_NaN() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::quiet_NaN());
        }

        static constexpr _value_type signaling_NaN() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::signaling_NaN());
        }

        static constexpr _value_type denorm_min() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::denorm_min());
        }
    };
}

#endif  // CNL_NUMBER_BASE_H
