
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SG14_number_base_H)
#define SG14_number_base_H 1

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

            template<class T>
            constexpr number_base(const T& r)
                : _rep(r) { }

            explicit constexpr operator bool() const
            {
                return data() != rep{};
            }

            constexpr const rep& data() const
            {
                return _rep;
            }

            static constexpr Derived from_data(rep r)
            {
                return Derived(r);
            }

        private:
            rep _rep;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_number_base

        template<class T, class Enable = void>
        struct is_number_base : std::false_type {};

        template<class Derived, class Rep>
        struct is_number_base<number_base<Derived, Rep>> : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_number

        template<class T, class Enable = void>
        struct is_number : std::false_type {};

        template<class Derived>
        struct is_number<Derived, typename std::enable_if<std::is_class<Derived>::value>::type> {
            static constexpr bool value
                    = std::is_base_of<number_base<Derived, typename Derived::rep>, Derived>::value;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::number_base operators

        // comparison

        template<class Derived>
        constexpr auto operator==(
                const _impl::number_base<Derived, typename Derived::rep>& lhs,
                const _impl::number_base<Derived, typename Derived::rep>& rhs)
        -> typename std::enable_if<_impl::is_number<Derived>::value, bool>::type
        {
            return lhs.data()==rhs.data();
        }

        template<class Derived>
        constexpr auto operator!=(
                const _impl::number_base<Derived, typename Derived::rep>& lhs,
                const _impl::number_base<Derived, typename Derived::rep>& rhs)
        -> typename std::enable_if<_impl::is_number<Derived>::value, bool>::type
        {
            return lhs.data()!=rhs.data();
        }

        template<class Derived>
        constexpr auto operator>(
                const _impl::number_base<Derived, typename Derived::rep>& lhs,
                const _impl::number_base<Derived, typename Derived::rep>& rhs)
        -> typename std::enable_if<_impl::is_number<Derived>::value, bool>::type
        {
            return lhs.data()>rhs.data();
        }

        template<class Derived>
        constexpr auto operator<(
                const _impl::number_base<Derived, typename Derived::rep>& lhs,
                const _impl::number_base<Derived, typename Derived::rep>& rhs)
        -> typename std::enable_if<_impl::is_number<Derived>::value, bool>::type
        {
            return lhs.data()<rhs.data();
        }

        template<class Derived>
        constexpr auto operator>=(
                const _impl::number_base<Derived, typename Derived::rep>& lhs,
                const _impl::number_base<Derived, typename Derived::rep>& rhs)
        -> typename std::enable_if<_impl::is_number<Derived>::value, bool>::type
        {
            return lhs.data()>=rhs.data();
        }

        template<class Derived>
        constexpr auto operator<=(
                const _impl::number_base<Derived, typename Derived::rep>& lhs,
                const _impl::number_base<Derived, typename Derived::rep>& rhs)
        -> typename std::enable_if<_impl::is_number<Derived>::value, bool>::type
        {
            return lhs.data()<=rhs.data();
        }

        // compound assignment

        template<class Lhs, class Rhs>
        auto operator+=(Lhs& lhs, const Rhs& rhs)
        -> typename std::enable_if<_impl::is_number<Lhs>::value, Lhs&>::type
        {
            return lhs = lhs + rhs;
        }

        template<class Lhs, class Rhs>
        auto operator-=(Lhs& lhs, const Rhs& rhs)
        -> typename std::enable_if<_impl::is_number<Lhs>::value, Lhs&>::type
        {
            return lhs = lhs - rhs;
        }

        template<class Lhs, class Rhs>
        auto operator*=(Lhs& lhs, const Rhs& rhs)
        -> typename std::enable_if<_impl::is_number<Lhs>::value, Lhs&>::type
        {
            return lhs = lhs * rhs;
        }

        template<class Lhs, class Rhs>
        auto operator/=(Lhs& lhs, const Rhs& rhs)
        -> typename std::enable_if<_impl::is_number<Lhs>::value, Lhs&>::type
        {
            return lhs = lhs / rhs;
        }

        // unary

        template<class Rhs>
        constexpr auto operator+(
                const Rhs& rhs)
        -> typename std::enable_if<_impl::is_number<Rhs>::value, Rhs>::type
        {
            return rhs;
        }

        template<class Rhs>
        constexpr auto operator-(
                const Rhs& rhs)
        -> typename std::enable_if<_impl::is_number<Rhs>::value, Rhs>::type
        {
            return Rhs::from_data(-rhs.data());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::to_rep

    namespace _to_rep_impl {
        template<class Rep, class Enable = void>
        struct to_rep;

        template<class Rep>
        struct to_rep<Rep, typename std::enable_if<!_impl::is_number<Rep>::value>::type> {
            constexpr const Rep& operator()(const Rep& component) const
            {
                static_assert(!_impl::is_number<Rep>::value, "");
                static_assert(!_impl::is_number_base<Rep>::value, "");
                static_assert(!_impl::is_number<typename std::decay<Rep>::type>::value, "");
                static_assert(!_impl::is_number_base<typename std::decay<Rep>::type>::value, "");
                return component;
            }
        };

        template<class Derived>
        struct to_rep<Derived, typename std::enable_if<_impl::is_number<Derived>::value>::type> {
            constexpr const typename Derived::rep& operator()(const Derived& component) const
            {
                return component.data();
            }
        };
    }

    template<class Component>
    constexpr auto to_rep(Component&& component)
    -> decltype(_to_rep_impl::to_rep<typename std::decay<Component>::type>()(component)) {
        return _to_rep_impl::to_rep<typename std::decay<Component>::type>()(component);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::from_rep

    namespace _from_rep_impl {
        template<class Rep, class Enable = void>
        struct from_rep;

        template<class Rep>
        struct from_rep<Rep, typename std::enable_if<!_impl::is_number<Rep>::value>::type> {
            constexpr const Rep& operator()(const Rep& component) const
            {
                static_assert(!_impl::is_number<Rep>::value, "");
                static_assert(!_impl::is_number_base<Rep>::value, "");
                static_assert(!_impl::is_number<typename std::decay<Rep>::type>::value, "");
                static_assert(!_impl::is_number_base<typename std::decay<Rep>::type>::value, "");
                return component;
            }
        };

        template<class Derived>
        struct from_rep<Derived, typename std::enable_if<_impl::is_number<Derived>::value>::type> {
            constexpr const Derived operator()(const typename Derived::rep& rep) const
            {
                return Derived::from_data(rep);
            }
        };
    }

    template<class Derived, class Rep>
    constexpr auto from_rep(Rep const& rep)
    -> decltype(_from_rep_impl::from_rep<Derived>()(rep)) {
        return _from_rep_impl::from_rep<Derived>()(rep);
    }

    template<class Rep>
    constexpr auto from_rep(Rep const& component)
    -> Rep const& {
        return component;
    }
}

#endif
