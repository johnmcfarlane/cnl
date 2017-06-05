
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.Multiprecision for use with @ref sg14::fixed_point

#if !defined(SG14_MULTIPRECISION_H)
#define SG14_MULTIPRECISION_H 1

#include <sg14/numeric_traits>

#include <boost/multiprecision/cpp_int.hpp>

/// study group 14 of the C++ working group
namespace sg14 {
    namespace _bmp = boost::multiprecision;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // type trait specializations of boost::multiprecision types
    //
    // These are the definitions needed to use any custom integer type with
    // sg14::fixed_point

    // numeric_traits<boost::multiprecision::cpp_int_backend<...>>
    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct numeric_traits<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>> {

        using make_signed = _bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude, Checked, Allocator>;
        using make_unsigned = _bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude, Checked, Allocator>;

        static_assert(SignType == _bmp::signed_magnitude || SignType == _bmp::unsigned_magnitude, "following sign test may fail");
        static constexpr bool is_signed = (SignType == _bmp::signed_magnitude);

        static constexpr _digits_type digits = NumBits-is_signed;

        template<_digits_type NewNumDigits>
        using set_digits = _bmp::cpp_int_backend<NewNumDigits+is_signed, NewNumDigits+is_signed, SignType, Checked, Allocator>;
    };

    // numeric_traits<boost::multiprecision::number<>>
    template<class Backend, _bmp::expression_template_option ExpressionTemplates>
    struct numeric_traits<_bmp::number<Backend, ExpressionTemplates>>
    : sg14::_impl::numeric_traits_base<_bmp::number<Backend, ExpressionTemplates>> {
        using _backend_traits = numeric_traits<Backend>;

        using make_signed = _bmp::number<typename _backend_traits::make_signed, ExpressionTemplates>;
        using make_unsigned = _bmp::number<typename _backend_traits::make_unsigned, ExpressionTemplates>;

        static constexpr _digits_type digits = _backend_traits::digits;

        template<_digits_type NumDigits>
        using set_digits = _bmp::number<_impl::set_digits_t<Backend, NumDigits>, ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // aliases of _bmp types

    namespace _sized_integer_impl {
        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using backend = _bmp::cpp_int_backend<
                NumBits, NumBits, SignType, _bmp::unchecked, void>;
    }

    // sg14::signed_multiprecision - a signed integer of arbitrary size
    template<unsigned NumDigits = numeric_traits<int>::digits>
    using signed_multiprecision = _bmp::number<_sized_integer_impl::backend<NumDigits+1, _bmp::signed_magnitude>, _bmp::et_off>;

    // sg14::unsigned_multiprecision - an unsigned integer of arbitrary size
    template<unsigned NumDigits = numeric_traits<int>::digits>
    using unsigned_multiprecision = _bmp::number<_sized_integer_impl::backend<NumDigits+1, _bmp::unsigned_magnitude>, _bmp::et_off>;

    // sg14::unsigned_multiprecision - an integer of arbitrary size
    template<unsigned NumDigits = numeric_traits<int>::digits>
    using multiprecision = signed_multiprecision<NumDigits+1>;
}

#endif // SG14_MULTIPRECISION_H
