
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(_SG14_SIZED_INTEGER)
#define _SG14_SIZED_INTEGER 1

#include <boost/multiprecision/cpp_int.hpp>

#include <sg14/auxiliary/elastic.h>

namespace sg14 {
    namespace _bmp {
        using namespace boost::multiprecision;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // type trait specializations of _bmp types

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_signed<{un}signed_multiprecision>

    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct is_signed<_bmp::number<Backend, ExpressionTemplates>>
            : _bmp::is_signed_number<_bmp::number<Backend, ExpressionTemplates>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_unsigned<{un}signed_multiprecision>

    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct is_unsigned<_bmp::number<Backend, ExpressionTemplates>>
            : _bmp::is_unsigned_number<_bmp::number<Backend, ExpressionTemplates>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed specializations

    // sg14::make_signed<_bmp::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_signed<_bmp::cpp_int_backend<
            MinBits, MaxBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<
                MinBits, MaxBits, _bmp::signed_magnitude, Checked, Allocator>;
    };

    // sg14::make_signed<_bmp::number>
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
    // sg14::make_unsigned specializations

    // sg14::make_unsigned<_bmp::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked, class Allocator>
    struct make_unsigned<_bmp::cpp_int_backend<
            MinBits, MaxBits, SignType, Checked, Allocator>> {
        using type = _bmp::cpp_int_backend<
                MinBits, MaxBits, _bmp::unsigned_magnitude, Checked, Allocator>;
    };

    // sg14::make_unsigned<_bmp::number>
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
    // sg14::is_integral specialization

    // sg14::is_integral<_bmp::number<>>
    template<
            class Backend,
            _bmp::expression_template_option ExpressionTemplates>
    struct is_integral<_bmp::number<
            Backend,
            ExpressionTemplates>> : std::true_type {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::resize specializations

    // sg14::resize<_bmp::cpp_int_backend<>>
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

    // sg14::resize<_bmp::number>
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
    // sg14::width specializations

    // sg14::width<_bmp::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, _bmp::cpp_integer_type SignType, _bmp::cpp_int_check_type Checked>
    struct width<_bmp::cpp_int_backend<MinBits, MaxBits, SignType, Checked>> : std::integral_constant<int, MaxBits> {
    };

    // sg14::width<_bmp::number>
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

#endif // _SG14_SIZED_INTEGER
