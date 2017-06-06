
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SG14_number_base_H)
#define SG14_number_base_H 1

#if !defined(SG14_GODBOLT_ORG)
#include <sg14/auxiliary/const_integer.h>
#include <sg14/bits/common.h>
#include <sg14/numeric_traits>
#endif

#include <limits>
#include <type_traits>

namespace sg14 {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base {
            enum _from_data {
                _from_data
            };

        public:
            using rep = Rep;

            number_base() = default;

            constexpr number_base(const rep& r)
                : _rep(r) { }

            template<class T>
            number_base& operator=(const T& r) {
                _rep = r;
                return static_cast<Derived&>(*this);
            }

            explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }

            constexpr const rep& data() const
            {
                return _rep;
            }

            static constexpr Derived from_data(const rep& r)
            {
                return Derived(r);
            }

        private:
            rep _rep;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_class_derived_from_number_base

        // true iff T's base class is sg14::_impl::number_base;
        // T must be a class;
        // used by sg14::_impl::is_derived_from_number_base
        template<class Derived, class Enable = void>
        struct is_class_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_class_derived_from_number_base<
                Derived,
                enable_if_t<std::is_base_of<number_base<Derived, typename Derived::rep>, Derived>::value>>
                : std::true_type {};

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_derived_from_number_base

        // true if T is the Derived parameter of a number_base type
        template<class T, class Enable = void>
        struct is_derived_from_number_base : std::false_type {};

        template<class Derived>
        struct is_derived_from_number_base<Derived, enable_if_t<std::is_class<Derived>::value>>
        : is_class_derived_from_number_base<Derived> { };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::enable_if_precedes
        
        template<class Former, class Latter>
        struct precedes {
            static constexpr bool value =
                    (std::is_floating_point<Former>::value && !std::is_floating_point<Latter>::value)
                            || (is_derived_from_number_base<Former>::value &&
                                    !(is_derived_from_number_base<Latter>::value
                                            || std::is_floating_point<Latter>::value));
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::operate

        // higher OP number_base<>
        template<
                class Operator, class Lhs, class RhsDerived, class RhsRep,
                enable_if_t <precedes<Lhs, RhsDerived>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const Lhs& lhs, const number_base<RhsDerived, RhsRep>& rhs, Operator op)
        -> decltype(op(lhs, to_rep(static_cast<const RhsDerived&>(rhs))))
        {
            return op(lhs, to_rep(static_cast<const RhsDerived&>(rhs)));
        }

        // number_base<> OP higher
        template<
                class Operator, class LhsDerived, class LhsRep, class Rhs,
                enable_if_t <precedes<Rhs, LhsDerived>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const number_base<LhsDerived, LhsRep>& lhs, const Rhs& rhs, Operator op)
        -> decltype(op(to_rep(static_cast<const LhsDerived&>(lhs)), rhs))
        {
            return op(to_rep(static_cast<const LhsDerived&>(lhs)), rhs);
        }

        // lower OP number_base<>
        template<
                class Operator, class Lhs, class RhsDerived, class RhsRep,
                enable_if_t <precedes<RhsDerived, Lhs>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const Lhs& lhs, const number_base<RhsDerived, RhsRep>& rhs, Operator op)
        -> decltype(op(numeric_traits<RhsDerived>::make(lhs), static_cast<const RhsDerived&>(rhs))) {
            return op(numeric_traits<RhsDerived>::make(lhs), static_cast<const RhsDerived&>(rhs));
        }

        // number_base<> OP lower
        template<
                class Operator, class LhsDerived, class LhsRep, class Rhs,
                enable_if_t <precedes<LhsDerived, Rhs>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const number_base<LhsDerived, LhsRep>& lhs, const Rhs& rhs, Operator op)
        -> decltype(op(static_cast<const LhsDerived&>(lhs), numeric_traits<LhsDerived>::make(rhs)))
        {
            return op(static_cast<const LhsDerived&>(lhs), numeric_traits<LhsDerived>::make(rhs));
        }

        // unary operate
        template<class Operator, class RhsDerived, class RhsRep>
        constexpr auto operate(const number_base<RhsDerived, RhsRep>& rhs, Operator op)
#if ! defined(_MSC_VER)
        -> decltype(op(rhs.data()))
#endif
        {
            return op(rhs.data());
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::number_base operators

        // compound assignment

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator+=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs + rhs)
        {
            return lhs = lhs + rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator-=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs - rhs)
        {
            return lhs = lhs - rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator*=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs * rhs)
        {
            return lhs = lhs * rhs;
        }

        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator/=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs / rhs)
        {
            return lhs = lhs / rhs;
        }

        // unary operators

        template<class RhsDerived, class RhsRep>
        constexpr auto operator+(const number_base<RhsDerived, RhsRep>& rhs)
        -> decltype(operate(rhs, plus_tag))
        {
            return operate(rhs, plus_tag);
        }

        template<class RhsDerived, class RhsRep>
        constexpr auto operator-(const number_base<RhsDerived, RhsRep>& rhs)
#if ! defined(_MSC_VER)
        -> decltype(operate(rhs, minus_tag))
#endif
        {
            return operate(rhs, minus_tag);
        }

        // binary arithmetic operators

        template<class Lhs, class Rhs>
        constexpr auto operator+(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, add_tag))
        {
            return operate(lhs, rhs, add_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator-(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, subtract_tag))
        {
            return operate(lhs, rhs, subtract_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator*(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, multiply_tag))
        {
            return operate(lhs, rhs, multiply_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator/(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, divide_tag))
        {
            return operate(lhs, rhs, divide_tag);
        }

        // binary bitwise logic operators
        
        template<class Lhs, class Rhs>
        constexpr auto operator|(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, bitwise_or_tag))
        {
            return operate(lhs, rhs, bitwise_or_tag);
        }
        
        template<class Lhs, class Rhs>
        constexpr auto operator&(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, bitwise_and_tag))
        {
            return operate(lhs, rhs, bitwise_and_tag);
        }
        
        template<class Lhs, class Rhs>
        constexpr auto operator^(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, bitwise_xor_tag))
        {
            return operate(lhs, rhs, bitwise_xor_tag);
        }

        // comparison operator

        template<class Lhs, class Rhs>
        constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, equal_tag))
        {
            return operate(lhs, rhs, equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, not_equal_tag))
        {
            return operate(lhs, rhs, not_equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, less_than_tag))
        {
            return operate(lhs, rhs, less_than_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, greater_than_tag))
        {
            return operate(lhs, rhs, greater_than_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, less_than_or_equal_tag))
        {
            return operate(lhs, rhs, less_than_or_equal_tag);
        }

        template<class Lhs, class Rhs>
        constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, greater_than_or_equal_tag))
        {
            return operate(lhs, rhs, greater_than_or_equal_tag);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::numeric_traits<_impl::number_base<>>

    template<class Derived, class Rep>
    struct numeric_traits<_impl::number_base<Derived, Rep>> {
        using _rep = Rep;
        using _rep_numeric_traits = numeric_traits<_rep>;

        using _number_base = _impl::number_base<Derived, _rep>;
        using value_type = Derived;

        static constexpr bool is_specialized = true;

        static constexpr value_type from_rep(const _rep& r)
        {
            return _number_base::from_data(r);
        }

        static constexpr _rep to_rep(const value_type& number)
        {
            return number.data();
        }
        
        template<typename Input>
        static constexpr value_type make(const Input& input) {
            return value_type(input);
        }

        static constexpr value_type scale(const value_type& number, int base, int exp)
        {
            return from_rep(sg14::_impl::scale(to_rep(number), base, exp));
        }

        static constexpr value_type zero() {
            return from_rep(_rep_numeric_traits::zero());
        }
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for sg14::_impl::numeric_limits

    template<class Derived, class Rep>
    struct numeric_limits<sg14::_impl::number_base<Derived, Rep>>
    : numeric_limits<Rep> {
        // fixed-point-specific helpers
        using _value_type = Derived;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members

        static constexpr _value_type min() noexcept
        {
            return sg14::numeric_traits<_value_type>::from_rep(_rep_numeric_limits::min());
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::lowest());
        }

        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::round_error());
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

#endif  // SG14_NUMBER_BASE_H
