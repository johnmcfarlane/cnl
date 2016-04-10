
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(_SG14_SIZED_INTEGER)
#define _SG14_SIZED_INTEGER 1

#include <boost/multiprecision/cpp_int.hpp>

#include <sg14/elastic.h>

namespace sg14 {
    using boost::multiprecision::cpp_int_backend;
    using boost::multiprecision::cpp_int_check_type;
    using boost::multiprecision::cpp_integer_type;
    using boost::multiprecision::et_off;
    using boost::multiprecision::expression_template_option;
    using boost::multiprecision::is_signed_number;
    using boost::multiprecision::is_unsigned_number;
    using boost::multiprecision::number;
    using boost::multiprecision::signed_magnitude;
    using boost::multiprecision::unchecked;
    using boost::multiprecision::unsigned_magnitude;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // type trait specializations of boost::multiprecision types

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_signed<{un}signed_multiprecision>

    template<
            class Backend,
            expression_template_option ExpressionTemplates>
    struct is_signed<number<Backend, ExpressionTemplates>>
            : is_signed_number<number<Backend, ExpressionTemplates>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_unsigned<{un}signed_multiprecision>

    template<
            class Backend,
            expression_template_option ExpressionTemplates>
    struct is_unsigned<number<Backend, ExpressionTemplates>>
            : is_unsigned_number<number<Backend, ExpressionTemplates>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed specializations

    // sg14::make_signed<cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, cpp_integer_type SignType, cpp_int_check_type Checked, class Allocator>
    struct make_signed<cpp_int_backend<
            MinBits, MaxBits, SignType, Checked, Allocator>> {
        using type = cpp_int_backend<
                MinBits, MaxBits, signed_magnitude, Checked, Allocator>;
    };

    // sg14::make_signed<number>
    template<
            class Backend,
            expression_template_option ExpressionTemplates>
    struct make_signed<number<
            Backend,
            ExpressionTemplates>> {
        using type = number<
                typename make_signed<Backend>::type,
                ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_unsigned specializations

    // sg14::make_unsigned<cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, cpp_integer_type SignType, cpp_int_check_type Checked, class Allocator>
    struct make_unsigned<cpp_int_backend<
            MinBits, MaxBits, SignType, Checked, Allocator>> {
        using type = cpp_int_backend<
                MinBits, MaxBits, unsigned_magnitude, Checked, Allocator>;
    };

    // sg14::make_unsigned<number>
    template<
            class Backend,
            expression_template_option ExpressionTemplates>
    struct make_unsigned<number<
            Backend,
            ExpressionTemplates>> {
        using type = number<
                typename make_unsigned<Backend>::type,
                ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_integral specialization

    // sg14::is_integral<number<>>
    template<
            class Backend,
            expression_template_option ExpressionTemplates>
    struct is_integral<number<
            Backend,
            ExpressionTemplates>> : std::true_type {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::resize specializations

    // sg14::resize<cpp_int_backend<>>
    template<
            unsigned MinBits,
            unsigned MaxBits,
            cpp_integer_type SignType,
            cpp_int_check_type Checked,
            std::size_t NumBytes>
    struct resize<
            cpp_int_backend<
                    MinBits, MaxBits, SignType, Checked>,
            NumBytes> {
        using type = cpp_int_backend<
                NumBytes*CHAR_BIT,
                NumBytes*CHAR_BIT,
                unsigned_magnitude,
                Checked>;
    };

    // sg14::resize<number>
    template<
            class Backend,
            expression_template_option ExpressionTemplates,
            std::size_t NumBytes>
    struct resize<
            number<
                    Backend,
                    ExpressionTemplates>,
            NumBytes> {
        using type = number<
                typename resize<Backend, NumBytes>::type,
                ExpressionTemplates>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width specializations

    // sg14::width<boost::multiprecision::cpp_int_backend<>>
    template<unsigned MinBits, unsigned MaxBits, cpp_integer_type SignType, cpp_int_check_type Checked>
    struct width<cpp_int_backend<MinBits, MaxBits, SignType, Checked>> : std::integral_constant<int, MaxBits> {
    };

    // sg14::width<boost::multiprecision::number>
    template<class Backend, expression_template_option ExpressionTemplates>
    struct width<number<Backend, ExpressionTemplates>> : width<Backend> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // aliases of boost::multiprecision types

    namespace _sized_integer_impl {
        template<unsigned NumBits, cpp_integer_type SignType>
        using backend = cpp_int_backend<
                NumBits, NumBits, SignType, unchecked, void>;
    }

    // sg14::signed_multiprecision - a signed integer of arbitrary size
    template<unsigned NumBits = width<int>::value>
    using signed_multiprecision = number<_sized_integer_impl::backend<NumBits, signed_magnitude>, et_off>;

    // sg14::unsigned_multiprecision - an unsigned integer of arbitrary size
    template<unsigned NumBits = width<int>::value>
    using unsigned_multiprecision = number<_sized_integer_impl::backend<NumBits, unsigned_magnitude>, et_off>;

    // sg14::unsigned_multiprecision - an integer of arbitrary size
    template<unsigned NumBits = width<int>::value>
    using multiprecision = signed_multiprecision<NumBits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // elastic_multiprecision - elastic type with virtually no top limit on capacity

    template<int IntegerDigits, int FractionalDigits = 0, bool IsSigned = true>
    using elastic_multiprecision = elastic<
            IntegerDigits,
            FractionalDigits,
            typename std::conditional<
                    IsSigned,
                    signed_multiprecision<>,
                    unsigned_multiprecision<>>::type>;
}

#endif // _SG14_SIZED_INTEGER
