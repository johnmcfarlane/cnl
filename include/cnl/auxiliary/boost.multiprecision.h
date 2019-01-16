
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.Multiprecision for use with @ref cnl::fixed_point

#if !defined(CNL_BOOST_MULTIPRECISION_H)
#define CNL_BOOST_MULTIPRECISION_H 1

#include "../constant.h"
#include "../num_traits.h"
#include "../rounding_integer.h"
#include "../_impl/type_traits/is_signed.h"
#include "../_impl/type_traits/make_signed.h"
#include "../_impl/type_traits/make_unsigned.h"

#include <boost/multiprecision/cpp_int.hpp>

/// compositional numeric library
namespace cnl {
    namespace _bmp = boost::multiprecision;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // type trait specializations of boost::multiprecision::number

    namespace _impl {
        template<typename T>
        struct is_boost_multiprecision : std::false_type {
        };

        template<typename Backend>
        struct is_boost_multiprecision<_bmp::number<Backend>>
                : std::true_type {
        };
    }

    template<unsigned NumBits>
    struct is_signed<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>>
            : std::true_type {
    };

    template<unsigned NumBits>
    struct is_signed<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>>
            : std::false_type {
    };

    template<unsigned NumBits>
    struct make_signed<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>> {
        using type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>;
    };

    template<unsigned NumBits>
    struct make_signed<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>> {
        using type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>;
    };

    template<unsigned NumBits>
    struct make_unsigned<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>> {
        using type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>;
    };

    template<unsigned NumBits>
    struct make_unsigned<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>> {
        using type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>;
    };

    template<unsigned NumBits>
    struct digits<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>>
            : std::integral_constant<int, NumBits-1> {
    };

    template<unsigned NumBits>
    struct digits<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>>
            : std::integral_constant<int, NumBits> {
    };

    template<unsigned NumBits, int MinNumDigits>
    struct set_digits<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>, MinNumDigits> {
        using type = _bmp::number<_bmp::cpp_int_backend<MinNumDigits, MinNumDigits, _bmp::unsigned_magnitude>>;
    };

    template<unsigned NumBits, int MinNumDigits>
    struct set_digits<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>, MinNumDigits> {
        using type = _bmp::number<_bmp::cpp_int_backend<MinNumDigits+1, MinNumDigits+1, _bmp::signed_magnitude>>;
    };

    template<unsigned NumBits>
    struct to_rep<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>> {
        using _number_type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>;
        constexpr _number_type& operator()(_number_type& number) const {
            return number;
        };
        constexpr _number_type const& operator()(_number_type const& number) const {
            return number;
        };
        constexpr _number_type operator()(_number_type&& number) const {
            return number;
        };
    };

    template<unsigned NumBits>
    struct to_rep<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>> {
        using _number_type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>;
        constexpr _number_type& operator()(_number_type& number) const {
            return number;
        };
        constexpr _number_type const& operator()(_number_type const& number) const {
            return number;
        };
        constexpr _number_type operator()(_number_type&& number) const {
            return number;
        };
    };

    template<unsigned NumBits, class Value>
    struct from_value<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>,
            Value,
            _impl::enable_if_t<!_impl::is_boost_multiprecision<Value>::value>> {
    private:
        static constexpr auto _bits = digits<Value>::value+1;
    public:
        constexpr auto operator()(Value const& value) const
        -> _bmp::number<_bmp::cpp_int_backend<_bits, _bits, _bmp::signed_magnitude>>
        {
            return value;
        }
    };

    template<unsigned NumBits, class Value>
    struct from_value<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>,
            Value,
            _impl::enable_if_t<!_impl::is_boost_multiprecision<Value>::value>> {
    private:
        static constexpr auto _bits = digits<Value>::value;
    public:
        constexpr auto operator()(Value const& value) const
        -> _bmp::number<_bmp::cpp_int_backend<_bits, _bits, _bmp::unsigned_magnitude>>
        {
            return value;
        }
    };

    template<class LhsBackend, class RhsBackend>
    struct from_value<_bmp::number<LhsBackend>, _bmp::number<RhsBackend>> {
        constexpr auto operator()(_bmp::number<RhsBackend> const& value) const
        -> _bmp::number<RhsBackend> {
            return value;
        }
    };

    template<int Digits, int Radix, class Backend>
    struct scale<Digits, Radix, _bmp::number<Backend>>
            : _impl::default_scale<Digits, Radix, _bmp::number<Backend>> {
    };

    template<class Backend>
    struct rounding<_bmp::number<Backend>>
            : _impl::type_identity<native_rounding_tag> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // aliases of boost::multiprecision types

    namespace _sized_integer_impl {
        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using backend = _bmp::cpp_int_backend<NumBits, NumBits, SignType>;

        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using number = _bmp::number<_sized_integer_impl::backend<NumBits, SignType>, _bmp::et_off>;
    }

    /// \brief alias to a signed, fixed-size Boost.Multiprecision type
    /// suitable for instantiating \ref cnl::fixed_point
    ///
    /// \tparam NumDigits number of digits constituting the integer value (excluding sign bit)
    /// \sa cnl::unsigned_multiprecision, cnl::multiprecision
    template<unsigned NumDigits = digits<int>::value>
    using signed_multiprecision = _sized_integer_impl::number<NumDigits+1, _bmp::signed_magnitude>;

    /// \brief alias to an unsigned, fixed-size Boost.Multiprecision type
    /// suitable for instantiating \ref cnl::fixed_point
    ///
    /// \tparam NumDigits number of digits constituting the integer value (excluding sign bit)
    /// \sa cnl::signed_multiprecision, cnl::multiprecision
    template<unsigned NumDigits = digits<unsigned>::value>
    using unsigned_multiprecision = _sized_integer_impl::number<NumDigits, _bmp::unsigned_magnitude>;

    /// \brief alias to a signed, fixed-size Boost.Multiprecision type
    /// suitable for instantiating \ref cnl::fixed_point
    ///
    /// \tparam NumDigits number of digits constituting the integer value (excluding sign bit)
    /// \sa cnl::signed_multiprecision, cnl::unsigned_multiprecision
    template<unsigned NumDigits = digits<int>::value>
    using multiprecision = signed_multiprecision<NumDigits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // boost::multiprecision bitwise shift operators

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto operator<<(_sized_integer_impl::number<NumBits, _bmp::signed_magnitude> const& lhs, constant<Value>)
    -> decltype(lhs << Value)
    {
        return lhs << Value;
    }

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto operator<<(_sized_integer_impl::number<NumBits, _bmp::unsigned_magnitude> const& lhs, constant<Value>)
    -> decltype(lhs << Value)
    {
        return lhs << Value;
    }

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto operator>>(_sized_integer_impl::number<NumBits, _bmp::unsigned_magnitude> const& lhs, constant<Value>)
    -> decltype(lhs >> Value)
    {
        return lhs >> Value;
    }

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto operator>>(_sized_integer_impl::number<NumBits, _bmp::signed_magnitude> const& lhs, constant<Value>)
    -> decltype(lhs >> Value)
    {
        return lhs >> Value;
    }
}

#endif  // CNL_BOOST_MULTIPRECISION_H
