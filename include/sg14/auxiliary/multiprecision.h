
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.Multiprecision for use with @ref sg14::fixed_point

#if !defined(SG14_MULTIPRECISION_H)
#define SG14_MULTIPRECISION_H 1

#include <sg14/num_traits.h>

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

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_signed<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude, Checked, Allocator>;
    };

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_unsigned<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude, Checked, Allocator>;
    };

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct digits<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>>
    : std::integral_constant<_digits_type, NumBits> {
    };

    template<unsigned NumBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator, _digits_type MinNumDigits>
    struct set_digits<_bmp::cpp_int_backend<NumBits, NumBits, SignType, Checked, Allocator>, MinNumDigits> {
        static constexpr unsigned width = MinNumDigits + (SignType == _bmp::signed_magnitude);
        using type = _bmp::cpp_int_backend<width, width, SignType, Checked, Allocator>;
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

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // aliases of _bmp types

    namespace _sized_integer_impl {
        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using backend = _bmp::cpp_int_backend<
                NumBits, NumBits, SignType, _bmp::unchecked, void>;
    }

    // sg14::signed_multiprecision - a signed integer of arbitrary size
    template<unsigned NumDigits = digits<int>::value>
    using signed_multiprecision = _bmp::number<_sized_integer_impl::backend<NumDigits+1, _bmp::signed_magnitude>, _bmp::et_off>;

    // sg14::unsigned_multiprecision - an unsigned integer of arbitrary size
    template<unsigned NumDigits = digits<unsigned>::value>
    using unsigned_multiprecision = _bmp::number<_sized_integer_impl::backend<NumDigits, _bmp::unsigned_magnitude>, _bmp::et_off>;

    // sg14::unsigned_multiprecision - an integer of arbitrary size
    template<unsigned NumDigits = digits<int>::value>
    using multiprecision = signed_multiprecision<NumDigits+1>;
}

#endif // SG14_MULTIPRECISION_H
