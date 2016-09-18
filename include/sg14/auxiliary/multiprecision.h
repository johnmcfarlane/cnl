
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.Multiprecision for use with @ref sg14::fixed_point

#if !defined(SG14_MULTIPRECISION_H)
#define SG14_MULTIPRECISION_H 1

#include <sg14/fixed_point.h>

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

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_signed

    // sg14::is_signed<boost::multiprecision::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct is_signed<_bmp::number<Backend, ExpressionTemplates>>
            : _bmp::is_signed_number<_bmp::number<Backend, ExpressionTemplates>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_unsigned

    // sg14::is_unsigned<boost::multiprecision::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct is_unsigned<_bmp::number<Backend, ExpressionTemplates>>
            : _bmp::is_unsigned_number<_bmp::number<Backend, ExpressionTemplates>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed

    // sg14::make_signed<<boost::multiprecision::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_signed<_bmp::cpp_int_backend<
            MinBits, MaxBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<
                MinBits, MaxBits, _bmp::signed_magnitude, Checked, Allocator>;
    };

    // sg14::make_signed<boost::multiprecision::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct make_signed<_bmp::number<
            Backend,
            ExpressionTemplates>> {
        using type = _bmp::number<
                typename make_signed<Backend>::type,
                ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_unsigned

    // sg14::make_unsigned<<boost::multiprecision::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_unsigned<_bmp::cpp_int_backend<
            MinBits, MaxBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<
                MinBits, MaxBits, _bmp::unsigned_magnitude, Checked, Allocator>;
    };

    // sg14::make_unsigned<boost::multiprecision::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct make_unsigned<_bmp::number<
            Backend,
            ExpressionTemplates>> {
        using type = _bmp::number<
                typename make_unsigned<Backend>::type,
                ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_integral

    // sg14::is_integral<boost::multiprecision::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct is_integral<_bmp::number<
            Backend,
            ExpressionTemplates>> : std::true_type {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::set_width

    // sg14::set_width<boost::multiprecision::cpp_int_backend<>>
    template<
            unsigned MinBits,
            unsigned MaxBits,
            _bmp::cpp_integer_type SignType,
            _bmp::cpp_int_check_type Checked,
            _width_type MinNumBits>
    struct set_width<
            _bmp::cpp_int_backend<
                    MinBits, MaxBits, SignType, Checked>,
            MinNumBits> {
        using type = _bmp::cpp_int_backend<
                MinNumBits,
                MinNumBits,
                _bmp::unsigned_magnitude,
                Checked>;
    };

    // sg14::set_width<boost::multiprecision::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates,
            _width_type MinNumBits>
    struct set_width<
            _bmp::number<
                    Backend,
                    ExpressionTemplates>,
            MinNumBits> {
        using type = _bmp::number<
                typename set_width<Backend, MinNumBits>::type,
                ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width

    // sg14::width<boost::multiprecision::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked>
    struct width<_bmp::cpp_int_backend<MinBits, MaxBits, SignType, Checked>>
            : std::integral_constant<_width_type, MaxBits> {
    };

    // sg14::width<boost::multiprecision::number<>>
    template<class Backend, _bmp::expression_template_option ExpressionTemplates>
    struct width<_bmp::number<Backend, ExpressionTemplates>> : width<Backend> {
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
    template<unsigned NumBits = width<int>::value>
    using signed_multiprecision = _bmp::number<_sized_integer_impl::backend<NumBits, _bmp::signed_magnitude>, _bmp::et_off>;

    // sg14::unsigned_multiprecision - an unsigned integer of arbitrary size
    template<unsigned NumBits = width<int>::value>
    using unsigned_multiprecision = _bmp::number<_sized_integer_impl::backend<NumBits, _bmp::unsigned_magnitude>, _bmp::et_off>;

    // sg14::unsigned_multiprecision - an integer of arbitrary size
    template<unsigned NumBits = width<int>::value>
    using multiprecision = signed_multiprecision<NumBits>;
}

#endif // SG14_MULTIPRECISION_H
