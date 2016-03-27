
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <elastic.h>

#include <tuple>

using std::is_same;
using sg14::elastic;

////////////////////////////////////////////////////////////////////////////////
// useful constants

template <typename T, bool IsSigned>
struct test_traits {
    static_assert(sg14::is_signed<T>::value == IsSigned, "sg14::is_signed fails for give type, T");
    static_assert(sg14::is_unsigned<T>::value != IsSigned, "sg14::is_signed fails for give type, T");

    static_assert(sg14::is_signed<typename sg14::make_signed<T>::type>::value, "sg14::make_signed failed sg14::is_signed test; please reboot");
    static_assert(! sg14::is_unsigned<typename sg14::make_signed<T>::type>::value, "sg14::make_signed failed sg14::is_unsigned test; please reboot");

    static_assert(! sg14::is_signed<typename sg14::make_unsigned<T>::type>::value, "sg14::make_unsigned failed sg14::is_signed test; please reboot");
    static_assert(sg14::is_unsigned<typename sg14::make_unsigned<T>::type>::value, "sg14::make_unsigned failed sg14::is_unsigned test; please reboot");
};

template
struct test_traits<std::uint8_t, false>;

template
struct test_traits<std::uint16_t, false>;

template
struct test_traits<std::uint32_t, false>;

template
struct test_traits<std::uint64_t, false>;

template
struct test_traits<std::int8_t, true>;

template
struct test_traits<std::int16_t, true>;

template
struct test_traits<std::int32_t, true>;

template
struct test_traits<std::int64_t, true>;

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles non-negative values;
// should pass for all specializations

template<typename Elastic>
struct positive_elastic_test {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using fixed_point_type = typename elastic_type::_fixed_point_type;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr fixed_point_type fixed_point_zero{0.};
    static constexpr elastic_type zero{fixed_point_zero};

    static constexpr fixed_point_type fixed_point_min{fixed_point_type::from_data(1)};
    static constexpr elastic_type min{fixed_point_min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(sg14::is_elastic<elastic_type>::value, "sg14::is_elastic test failed");
    static_assert(! sg14::is_elastic<fixed_point_type>::value, "sg14::is_elastic test failed");
    static_assert(! sg14::is_elastic<typename fixed_point_type::repr_type>::value, "sg14::is_elastic test failed");

    static_assert(sg14::is_signed<elastic_type>::value == sg14::is_signed<fixed_point_type>::value, "signedness of elastic type differns from underlying fixed-point type");

    static_assert(sg14::is_signed<typename sg14::make_signed<elastic_type>::type>::value, "signed version of elastic type is not signed");
    static_assert(sg14::is_unsigned<typename sg14::make_unsigned<elastic_type>::type>::value, "signed version of elastic type is not signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test fixed-point type

    static constexpr int integer_digits = elastic_type::integer_digits;
    static constexpr int fractional_digits = elastic_type::fractional_digits;

    static_assert(fixed_point_type::integer_digits>=integer_digits,
            "not enough integer digits in fixed-point type to represent elastic values");
    static_assert(fixed_point_type::fractional_digits>=fractional_digits,
            "not enough fractional digits in fixed-point type to represent elastic values");
    static_assert(fixed_point_type::fractional_digits==fractional_digits,
            "too many fractional digits in fixed-point type to represent elastic values accurately");

    ////////////////////////////////////////////////////////////////////////////////
    // test operators

    // before we set any bits at all
    static_assert(elastic_type{0.}==elastic_type{0.}, "comparison of same-type default-initialized types failed");
    static_assert(zero==elastic_type(), "default-initialized value is not represented using zero");
    static_assert(zero==elastic_type{0.}, "zero-initialized value is not represented using zero");
    static_assert(zero==-zero, "negative zero is not zero");

    // comparisons between zero
    static_assert((zero==zero)==true, "comparison of zero and zero failed");
    static_assert((zero!=zero)==false, "comparison of zero and zero failed");
    static_assert((zero<zero)==false, "comparison of zero and zero failed");
    static_assert((zero>zero)==false, "comparison of zero and zero failed");
    static_assert((zero<=zero)==true, "comparison of zero and zero failed");
    static_assert((zero>=zero)==true, "comparison of zero and zero failed");

    // comparisons between minimum value
    static_assert((min==min)==true, "comparison of min and min failed");
    static_assert((min!=min)==false, "comparison of min and min failed");
    static_assert((min<min)==false, "comparison of min and min failed");
    static_assert((min>min)==false, "comparison of min and min failed");
    static_assert((min<=min)==true, "comparison of min and min failed");
    static_assert((min>=min)==true, "comparison of min and min failed");

    // zero vs min
    static_assert((zero==min)==false, "comparison of zero and min failed");
    static_assert((zero!=min)==true, "comparison of zero and min failed");
    static_assert((zero<min)==true, "comparison of zero and min failed");
    static_assert((zero>min)==false, "comparison of zero and min failed");
    static_assert((zero<=min)==true, "comparison of zero and min failed");
    static_assert((zero>=min)==false, "comparison of zero and min failed");

    // min vs zero
    static_assert((min==zero)==false, "comparison of min and zero failed");
    static_assert((min!=zero)==true, "comparison of min and zero failed");
    static_assert((min<zero)==false, "comparison of min and zero failed");
    static_assert((min>zero)==true, "comparison of min and zero failed");
    static_assert((min<=zero)==false, "comparison of min and zero failed");
    static_assert((min>=zero)==true, "comparison of min and zero failed");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles negative values;
// should pass for all signed specializations

template<typename Elastic>
struct negative_elastic_test {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using fixed_point_type = typename elastic_type::_fixed_point_type;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr fixed_point_type fixed_point_zero{0.};
    static constexpr elastic_type zero{fixed_point_zero};

    static constexpr fixed_point_type fixed_point_min{fixed_point_type::from_data(1)};
    static constexpr elastic_type min{fixed_point_min};

    static constexpr fixed_point_type fixed_point_negative_min{fixed_point_type::from_data(-1)};
    static constexpr elastic_type negative_min{fixed_point_negative_min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    // not much point testing negative value properties of unsigned type, eh?
    static_assert(sg14::is_signed<elastic_type>::value, "subject of test class is not reported as signed");
    static_assert(is_same<typename sg14::make_signed<elastic_type>::type, elastic_type>::value, "subject of test class is not reported as signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operators

    // comparisons between negative_min
    static_assert((negative_min==negative_min)==true, "comparison of negative_min and negative_min failed");
    static_assert((negative_min!=negative_min)==false, "comparison of negative_min and negative_min failed");
    static_assert((negative_min<negative_min)==false, "comparison of negative_min and negative_min failed");
    static_assert((negative_min>negative_min)==false, "comparison of negative_min and negative_min failed");
    static_assert((negative_min<=negative_min)==true, "comparison of negative_min and negative_min failed");
    static_assert((negative_min>=negative_min)==true, "comparison of negative_min and negative_min failed");

    // min vs negative_min
    static_assert((min==negative_min)==false, "comparison of min and negative_min failed");
    static_assert((min!=negative_min)==true, "comparison of min and negative_min failed");
    static_assert((min<negative_min)==false, "comparison of min and negative_min failed");
    static_assert((min>negative_min)==true, "comparison of min and negative_min failed");
    static_assert((min<=negative_min)==false, "comparison of min and negative_min failed");
    static_assert((min>=negative_min)==true, "comparison of min and negative_min failed");

    // negative_min vs zero
    static_assert((negative_min==zero)==false, "comparison of negative_min and zero failed");
    static_assert((negative_min!=zero)==true, "comparison of negative_min and zero failed");
    static_assert((negative_min<zero)==true, "comparison of negative_min and zero failed");
    static_assert((negative_min>zero)==false, "comparison of negative_min and zero failed");
    static_assert((negative_min<=zero)==true, "comparison of negative_min and zero failed");
    static_assert((negative_min>=zero)==false, "comparison of negative_min and zero failed");
};

////////////////////////////////////////////////////////////////////////////////
// given values for IntegerDigits and FractionalDigits parameters,
// triggers elastic<> tests with signed and unsigned specializations

template<int IntegerDigits, int FractionalDigits>
struct elastic_test :
        // perform positive value tests against unsigned elastic specialization
        positive_elastic_test<elastic<IntegerDigits, FractionalDigits, unsigned>>,

        // perform positive value tests against signed elastic specialization
        positive_elastic_test<elastic<IntegerDigits, FractionalDigits, signed>>,

        // perform negative value tests against signed elastic specialization
        negative_elastic_test<elastic<IntegerDigits, FractionalDigits, signed>> {
};

////////////////////////////////////////////////////////////////////////////////
// given a value for IntegerDigits parameter,
// triggers elastic<> tests against a range of values for FractionalDigits parameter

template<int IntegerDigits>
struct elastic_test_with_integer_digits :
        elastic_test<IntegerDigits, - IntegerDigits + 1>,
        elastic_test<IntegerDigits, - IntegerDigits + 2>,
        elastic_test<IntegerDigits, - IntegerDigits + 3>,
        elastic_test<IntegerDigits, - IntegerDigits + 5>,
        elastic_test<IntegerDigits, - IntegerDigits + 7>,
        elastic_test<IntegerDigits, - IntegerDigits + 8>,
        elastic_test<IntegerDigits, - IntegerDigits + 12>,
        elastic_test<IntegerDigits, - IntegerDigits + 16>,
        elastic_test<IntegerDigits, - IntegerDigits + 21>,
        elastic_test<IntegerDigits, - IntegerDigits + 27>,
        elastic_test<IntegerDigits, - IntegerDigits + 32>,
        elastic_test<IntegerDigits, - IntegerDigits + 39>,
        elastic_test<IntegerDigits, - IntegerDigits + 44>,
        elastic_test<IntegerDigits, - IntegerDigits + 55> {
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic tests against a range of values for IntegerDigits parameter

template
struct elastic_test_with_integer_digits<-43>;

template
struct elastic_test_with_integer_digits<-31>;

template
struct elastic_test_with_integer_digits<-27>;

template
struct elastic_test_with_integer_digits<-19>;

template
struct elastic_test_with_integer_digits<-16>;

template
struct elastic_test_with_integer_digits<-13>;

template
struct elastic_test_with_integer_digits<-8>;

template
struct elastic_test_with_integer_digits<-6>;

template
struct elastic_test_with_integer_digits<-5>;

template
struct elastic_test_with_integer_digits<-2>;

template
struct elastic_test_with_integer_digits<-1>;

template
struct elastic_test_with_integer_digits<0>;

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<6>;

template
struct elastic_test_with_integer_digits<8>;

template
struct elastic_test_with_integer_digits<13>;

template
struct elastic_test_with_integer_digits<16>;

template
struct elastic_test_with_integer_digits<19>;

template
struct elastic_test_with_integer_digits<27>;

template
struct elastic_test_with_integer_digits<31>;

template
struct elastic_test_with_integer_digits<43>;
