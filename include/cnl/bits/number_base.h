
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_NUMBER_BASE_H)
#define CNL_NUMBER_BASE_H 1

#include <cnl/bits/common.h>
#include <cnl/constant.h>
#include <cnl/num_traits.h>

#include <limits>
#include <type_traits>

namespace cnl {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base {
        public:
            static_assert(numeric_limits<Rep>::is_integer, "number_base must be specialized with integer Rep type template parameter");

            using rep = Rep;
            using _derived = Derived;

            number_base() = default;

            constexpr number_base(rep const& r)
                : _rep(r) { }

            template<class T>
            CNL_COPY_CONSTEXPR number_base& operator=(T const& r) {
                _rep = r;
                return static_cast<Derived&>(*this);
            }

            explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

            friend struct cnl::to_rep<_derived>;
            friend struct cnl::to_rep<number_base<_derived, rep>>;

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
        // cnl::_impl::enable_if_precedes
        
        template<class Former, class Latter>
        struct precedes {
            static constexpr bool value =
                    (std::is_floating_point<Former>::value && !std::is_floating_point<Latter>::value)
                            || (is_derived_from_number_base<Former>::value &&
                                    !(is_derived_from_number_base<Latter>::value
                                            || std::is_floating_point<Latter>::value));
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::operate

        // higher OP number_base<>
        template<
                class Operator, class Lhs, class RhsDerived, class RhsRep,
                enable_if_t <precedes<Lhs, RhsDerived>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(Lhs const& lhs, number_base<RhsDerived, RhsRep> const& rhs, Operator op)
        -> decltype(op(lhs, static_cast<Lhs>(static_cast<RhsDerived const&>(rhs))))
        {
            return op(lhs, static_cast<Lhs>(static_cast<RhsDerived const&>(rhs)));
        }

        // number_base<> OP higher
        template<
                class Operator, class LhsDerived, class LhsRep, class Rhs,
                enable_if_t <precedes<Rhs, LhsDerived>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(number_base<LhsDerived, LhsRep> const& lhs, Rhs const& rhs, Operator op)
        -> decltype(op(static_cast<Rhs>(static_cast<LhsDerived const&>(lhs)), rhs))
        {
            return op(static_cast<Rhs>(static_cast<LhsDerived const&>(lhs)), rhs);
        }

        // lower OP number_base<>
        template<
                class Operator, class Lhs, class RhsDerived, class RhsRep,
                enable_if_t <precedes<RhsDerived, Lhs>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(Lhs const& lhs, number_base<RhsDerived, RhsRep> const& rhs, Operator op)
        -> decltype(op(_impl::from_value<RhsDerived>(lhs), static_cast<RhsDerived const&>(rhs))) {
            return op(from_value<RhsDerived>(lhs), static_cast<RhsDerived const&>(rhs));
        }

        // number_base<> OP lower
        template<
                class Operator, class LhsDerived, class LhsRep, class Rhs,
                enable_if_t <precedes<LhsDerived, Rhs>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(number_base<LhsDerived, LhsRep> const& lhs, Rhs const& rhs, Operator op)
        -> decltype(op(static_cast<LhsDerived const&>(lhs), from_value<LhsDerived>(rhs)))
        {
            return op(static_cast<LhsDerived const&>(lhs), from_value<LhsDerived>(rhs));
        }

        // unary operate
        template<class Operator, class RhsDerived, class RhsRep>
        constexpr auto operate(number_base<RhsDerived, RhsRep> const& rhs, Operator op)
        -> decltype(_impl::from_value<RhsDerived>(op(_impl::to_rep(rhs))))
        {
            return _impl::from_value<RhsDerived>(op(_impl::to_rep(rhs)));
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::number_base operators

        // compound assignment

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator+=(Lhs& lhs, Rhs const& rhs)
        -> decltype(lhs = lhs + rhs)
        {
            return lhs = lhs + rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator-=(Lhs& lhs, Rhs const& rhs)
        -> decltype(lhs = lhs - rhs)
        {
            return lhs = lhs - rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator*=(Lhs& lhs, Rhs const& rhs)
        -> decltype(lhs = lhs * rhs)
        {
            return lhs = lhs * rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator/=(Lhs& lhs, Rhs const& rhs)
        -> decltype(lhs = lhs / rhs)
        {
            return lhs = lhs / rhs;
        }

        // unary operators

        template<class RhsDerived, class RhsRep>
        constexpr auto operator+(number_base<RhsDerived, RhsRep> const& rhs)
        -> decltype(operate(rhs, plus_tag))
        {
            return operate(rhs, plus_tag);
        }

        template<class RhsDerived, class RhsRep>
        constexpr auto operator-(number_base<RhsDerived, RhsRep> const& rhs)
        -> decltype(operate(rhs, minus_tag))
        {
            return operate(rhs, minus_tag);
        }

        // binary arithmetic operators

        template<class Lhs, class Rhs>
        constexpr auto operator+(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, add_tag))
        {
            return operate(lhs, rhs, add_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator-(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, subtract_tag))
        {
            return operate(lhs, rhs, subtract_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator*(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, multiply_tag))
        {
            return operate(lhs, rhs, multiply_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator/(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, divide_tag))
        {
            return operate(lhs, rhs, divide_tag);
        }

        // binary bitwise logic operators
        
        template<class Lhs, class Rhs>
        constexpr auto operator|(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, bitwise_or_tag))
        {
            return operate(lhs, rhs, bitwise_or_tag);
        }
        
        template<class Lhs, class Rhs>
        constexpr auto operator&(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, bitwise_and_tag))
        {
            return operate(lhs, rhs, bitwise_and_tag);
        }
        
        template<class Lhs, class Rhs>
        constexpr auto operator^(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, bitwise_xor_tag))
        {
            return operate(lhs, rhs, bitwise_xor_tag);
        }

        // comparison operator

        template<class Lhs, class Rhs>
        constexpr auto operator==(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, equal_tag))
        {
            return operate(lhs, rhs, equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator!=(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, not_equal_tag))
        {
            return operate(lhs, rhs, not_equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator<(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, less_than_tag))
        {
            return operate(lhs, rhs, less_than_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator>(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, greater_than_tag))
        {
            return operate(lhs, rhs, greater_than_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator<=(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, less_than_or_equal_tag))
        {
            return operate(lhs, rhs, less_than_or_equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator>=(Lhs const& lhs, Rhs const& rhs)
        -> decltype(operate(lhs, rhs, greater_than_or_equal_tag))
        {
            return operate(lhs, rhs, greater_than_or_equal_tag);
        }
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

    template<class Derived, class Rep>
    struct to_rep<_impl::number_base<Derived, Rep>> {
        constexpr auto operator()(_impl::number_base<Derived, Rep> const& number) const -> Rep {
            return number._rep;
        }
    };

    template<class Number>
    struct to_rep<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        constexpr auto operator()(Number const& number) const
        -> typename Number::rep {
            return number._rep;
        }
    };

    template<class Derived, class Rep>
    struct scale<_impl::number_base<Derived, Rep>> {
        template<class Input>
        constexpr Rep operator()(Input const&i, int base, int exp) const {
            return (exp < 0)
                   ? _impl::to_rep(i) / _num_traits_impl::pow<Rep>(base, -exp)
                   : _impl::to_rep(i) * _num_traits_impl::pow<Rep>(base, exp);
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
            return _impl::from_rep<_value_type>(_rep_numeric_limits::min());
        }

        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }

        static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::round_error());
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
