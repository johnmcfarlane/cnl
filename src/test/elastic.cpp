
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <elastic.h>

#include <tuple>

using std::is_same;
using sg14::elastic;

template <class T>
using numeric_limits = std::numeric_limits<T>;

////////////////////////////////////////////////////////////////////////////////
// useful constants

template <class T, bool IsSigned>
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
// test specific operations

// Lhs == Rhs
template <class T>
constexpr bool is_equal_to(const T& Lhs, const T& Rhs) {
    return ((Lhs == Rhs) == false) ? (throw std::logic_error("Lhs is *not* equal to Rhs"))
            : ((Lhs != Rhs) == true) ? (throw std::logic_error("Lhs *is* not equal to Rhs"))
            : ((Lhs < Rhs) == true) ? (throw std::logic_error("Lhs is less than Rhs"))
            : ((Lhs > Rhs) == true) ? (throw std::logic_error("Lhs is greater than Rhs"))
            : ((Lhs <= Rhs) == false) ? (throw std::logic_error("Lhs is not less than or equal to Rhs"))
            : ((Lhs >= Rhs) == false) ? (throw std::logic_error("Lhs is not greater than or equal to Rhs"))
            : true;
}

static_assert(is_equal_to<int>(0, 0), "less_than_test test failed");

// Lesser < Greater
template <class T>
constexpr bool is_less_than(const T& Lesser, const T& Greater) {
    return ((Lesser == Greater) == true) ? (throw std::logic_error("Lesser is equal to Greater"))
            : ((Lesser != Greater) == false) ? (throw std::logic_error("Lesser is not not equal to Greater"))
            : ((Lesser < Greater) == false) ? (throw std::logic_error("Lesser is not less than Greater"))
            : ((Lesser > Greater) == true) ? (throw std::logic_error("Lesser is greater than Greater"))
            : ((Lesser <= Greater) == false) ? (throw std::logic_error("Lesser is not less than or equal to Greater"))
            : ((Lesser >= Greater) == true) ? (throw std::logic_error("Lesser is greater than or equal to Greater"))
            : true;
}

static_assert(is_less_than<int>(0, 1), "less_than_test test failed");

// Greater > Lesser
template <class T>
constexpr bool is_greater_than(const T& Greater, const T& Lesser) {
    return ((Greater == Lesser) == true) ? (throw std::logic_error("Greater is equal to Lesser"))
            : ((Greater != Lesser) == false) ? (throw std::logic_error("Greater is not not equal to Lesser"))
            : ((Greater < Lesser) == true) ? (throw std::logic_error("Greater is less than Lesser"))
            : ((Greater > Lesser) == false) ? (throw std::logic_error("Greater is not greater than Lesser"))
            : ((Greater <= Lesser) == true) ? (throw std::logic_error("Greater is less than or equal to Lesser"))
            : ((Greater >= Lesser) == false) ? (throw std::logic_error("Greater is not greater than or equal to Lesser"))
            : true;
}

static_assert(is_less_than<int>(0, 1), "less_than_test test failed");

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using fixed_point_type = typename elastic_type::_fixed_point_type;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr fixed_point_type fixed_point_zero{0.};
    static constexpr elastic_type zero{ fixed_point_zero };
    static constexpr elastic_type negative_zero{ -zero };

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
    // test numeric_limits<elastic>

    static_assert(is_equal_to(min, numeric_limits<elastic_type>::min()), "numeric_limits test failed");
    static_assert(min <= numeric_limits<elastic_type>::max(), "numeric_limits test failed");
    static_assert(is_greater_than(min, numeric_limits<elastic_type>::lowest()), "numeric_limits test failed");
    static_assert(sg14::is_signed<elastic_type>::value == numeric_limits<elastic_type>::is_signed, "numeric_limits test failed");
    //static_assert(numeric_limits<elastic_type>::is_integer == elastic_type{.5} != .5, "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between zero
    static_assert(is_equal_to<elastic_type>(zero, zero), "comparison test error");

    // comparisons between zero and default-initialized value
    static_assert(zero==elastic_type(), "default-initialized value is not represented using zero");

    // comparisons between zero and zero-initialized value
    static_assert(zero==elastic_type{0.}, "zero-initialized value is not represented using zero");

    // comparisons between zero and negative zero
    static_assert(is_equal_to<elastic_type>(zero, negative_zero), "comparison test error");

    // comparisons between minimum value
    static_assert(is_equal_to<elastic_type>(min, min), "comparison test error");

    // zero vs min
    static_assert(is_less_than<elastic_type>(zero, min), "comparison test error");

    // min vs zero
    static_assert(is_greater_than<elastic_type>(min, zero), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test negate operators

    using negate_result = decltype(-zero);
    static_assert(sg14::is_signed<negate_result>::value, "negative of positive value is not signed");
    static_assert(negate_result::integer_digits == elastic_type::integer_digits, "negative of positive value has wrong number of integer digits");
    static_assert(negate_result::fractional_digits == elastic_type::fractional_digits, "negative of positive value has wrong number of fractional digits");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles negative values;
// should pass for all signed specializations

template<class Elastic>
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
    // test comparison operators

    // comparisons between negative_min
    static_assert(is_equal_to<elastic_type>(negative_min, negative_min), "comparison test error");

    // min vs negative_min
    static_assert(is_greater_than<elastic_type>(min, negative_min), "comparison test error");

    // negative_min vs zero
    static_assert(is_less_than<elastic_type>(negative_min, zero), "comparison test error");
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
struct elastic_test_with_integer_digits<0>;

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<-1>;

template
struct elastic_test_with_integer_digits<2>;

template
struct elastic_test_with_integer_digits<-2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<-5>;

template
struct elastic_test_with_integer_digits<6>;

template
struct elastic_test_with_integer_digits<-6>;

template
struct elastic_test_with_integer_digits<8>;

template
struct elastic_test_with_integer_digits<-8>;

template
struct elastic_test_with_integer_digits<13>;

template
struct elastic_test_with_integer_digits<-13>;

template
struct elastic_test_with_integer_digits<16>;

template
struct elastic_test_with_integer_digits<-16>;

template
struct elastic_test_with_integer_digits<19>;

template
struct elastic_test_with_integer_digits<-19>;

template
struct elastic_test_with_integer_digits<27>;

template
struct elastic_test_with_integer_digits<-27>;

template
struct elastic_test_with_integer_digits<31>;

template
struct elastic_test_with_integer_digits<-31>;

template
struct elastic_test_with_integer_digits<43>;

template
struct elastic_test_with_integer_digits<-43>;
