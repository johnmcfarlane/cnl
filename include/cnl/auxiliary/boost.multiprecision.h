
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.Multiprecision for use with @ref cnl::fixed_point

#if !defined(CNL_BOOST_MULTIPRECISION_H)
#define CNL_BOOST_MULTIPRECISION_H 1

#include <cnl/constant.h>
#include <cnl/num_traits.h>

#include <boost/multiprecision/cpp_int.hpp>

/// compositional numeric library
namespace cnl {
    namespace _bmp = boost::multiprecision;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // type trait specializations of boost::multiprecision types
    //
    // These are the definitions needed to use any custom integer type with
    // cnl::fixed_point

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_signed<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude, Checked, Allocator>;
    };

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_unsigned<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude, Checked, Allocator>;
    };

    template<unsigned NumBits, _bmp::cpp_int_check_type Checked, class Allocator>
    struct digits<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude, Checked, Allocator>>
            : std::integral_constant<_digits_type, NumBits-1> {
    };

    template<unsigned NumBits, _bmp::cpp_int_check_type Checked, class Allocator>
    struct digits<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude, Checked, Allocator>>
            : std::integral_constant<_digits_type, NumBits> {
    };

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator, _digits_type MinNumDigits>
    struct set_digits<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>, MinNumDigits> {
        static constexpr unsigned width = MinNumDigits + (SignType == _bmp::signed_magnitude);
        using type = _bmp::cpp_int_backend<width, width, SignType, Checked, Allocator>;
    };

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Value>
    struct from_value<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked>, Value> {
    private:
        static constexpr auto _digits = digits<Value>::value;
        static constexpr auto _is_signed = is_signed<Value>::value;
        static constexpr auto _bits = _digits + _is_signed;
        static constexpr auto _sign_type = _is_signed ? _bmp::signed_magnitude : _bmp::unsigned_magnitude;
    public:
        using type = _bmp::cpp_int_backend<_bits, _bits, _sign_type, Checked, void>;
    };

    template<class Backend, _bmp::expression_template_option ExpressionTemplates>
    struct make_signed<_bmp::number<Backend, ExpressionTemplates>> {
        using type = _bmp::number<make_signed_t<Backend>, ExpressionTemplates>;
    };

    template<class Backend, _bmp::expression_template_option ExpressionTemplates>
    struct make_unsigned<_bmp::number<Backend, ExpressionTemplates>> {
        using type = _bmp::number<make_unsigned_t<Backend>, ExpressionTemplates>;
    };

    template<class Backend, _bmp::expression_template_option ExpressionTemplates>
    struct digits<_bmp::number<Backend, ExpressionTemplates>>
    : digits<Backend> {
    };

    template<class Backend, _bmp::expression_template_option ExpressionTemplates, _digits_type MinNumDigits>
    struct set_digits<_bmp::number<Backend, ExpressionTemplates>, MinNumDigits> {
        using type = _bmp::number<set_digits_t<Backend, MinNumDigits>, ExpressionTemplates>;
    };

    template<class Backend, _bmp::expression_template_option ExpressionTemplates, class Value>
    struct from_value<_bmp::number<Backend, ExpressionTemplates>, Value> {
        using type = _bmp::number<from_value_t<Backend, Value>, ExpressionTemplates>;
    };

    template<int Bits, class Backend, _bmp::expression_template_option ExpressionTemplates>
    struct shift<Bits, 2, _bmp::number<Backend, ExpressionTemplates>> {
        constexpr auto operator()(_bmp::number<Backend, ExpressionTemplates> const& s) const
        -> decltype((Bits>=0) ? s << Bits : s >> -Bits)
        {
            return (Bits>=0) ? s << Bits : s >> -Bits;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // aliases of _bmp types

    namespace _sized_integer_impl {
        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using backend = _bmp::cpp_int_backend<
                NumBits, NumBits, SignType, _bmp::unchecked, void>;

        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using number = _bmp::number<_sized_integer_impl::backend<NumBits, SignType>, _bmp::et_off>;
    }

    // cnl::signed_multiprecision - a signed integer of arbitrary size
    template<unsigned NumDigits = digits<int>::value>
    using signed_multiprecision = _sized_integer_impl::number<NumDigits+1, _bmp::signed_magnitude>;

    // cnl::unsigned_multiprecision - an unsigned integer of arbitrary size
    template<unsigned NumDigits = digits<unsigned>::value>
    using unsigned_multiprecision = _sized_integer_impl::number<NumDigits, _bmp::unsigned_magnitude>;

    // cnl::unsigned_multiprecision - an integer of arbitrary size
    template<unsigned NumDigits = digits<int>::value>
    using multiprecision = signed_multiprecision<NumDigits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // _bmp bitwise shift operators

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto operator<<(_sized_integer_impl::number<NumBits, SignType> const& lhs, constant<Value>)
    -> decltype(lhs << Value)
    {
        return lhs << Value;
    }

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto operator>>(_sized_integer_impl::number<NumBits, SignType> const& lhs, constant<Value>)
    -> decltype(lhs >> Value)
    {
        return lhs >> Value;
    }
}

#endif  // CNL_BOOST_MULTIPRECISION_H
