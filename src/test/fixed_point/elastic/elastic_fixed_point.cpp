
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of cnl::elastic_fixed_point alias

#include <cnl/auxiliary/elastic_fixed_point.h>
#include "../../number_test.h"

using std::is_same;

using cnl::elastic_fixed_point;
using namespace cnl::literals;
using cnl::make_elastic_fixed_point;

////////////////////////////////////////////////////////////////////////////////
// fast tests of cnl::elastic_fixed_point<> at its limits;
// if something broke it may show up here first

namespace {
    using cnl::_impl::fp::arithmetic::intermediate;
    using cnl::_impl::fp::arithmetic::result;
    using cnl::_impl::shift_left;
    using cnl::_impl::fp::arithmetic::wide_tag;
    using cnl::_impl::divide_op;
    using cnl::_impl::multiply_op;
    using cnl::digits;
    using cnl::elastic_integer;
    using cnl::is_signed;
    using cnl::fixed_point;
    using cnl::set_digits_t;

    static_assert(shift_left<1, cnl::elastic_integer<31, int>>(0) == 0, "");
    static_assert(shift_left<1, cnl::elastic_integer<32, unsigned>>(0) == 0u, "");
    static_assert(shift_left<1, cnl::elastic_integer<63, int>>(0) == 0, "");
    static_assert(shift_left<1, cnl::elastic_integer<64, unsigned>>(0) == 0u, "");

    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<15, int>, 0>, fixed_point<elastic_integer<15, int>, 0>>::type{0},
            fixed_point<elastic_integer<30, int>, -15>{0}), "cnl::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<16, unsigned>, 0>, fixed_point<elastic_integer<16, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<32, unsigned>, -16>{0}), "cnl::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<31, int>, 0>, fixed_point<elastic_integer<31, int>, 0>>::type{0},
            fixed_point<elastic_integer<62, int>, -31>{0}), "cnl::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<32, unsigned>, 0>, fixed_point<elastic_integer<32, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<64, unsigned>, -32>{0}), "cnl::elastic_integer test failed");

    static_assert(identical(
            intermediate<wide_tag, multiply_op, fixed_point<elastic_integer<27, unsigned int>, -27>, fixed_point<elastic_integer<27, unsigned int>, -27>>::lhs_type{0},
            fixed_point<elastic_integer<27, unsigned int>, -27>{0}), "cnl::elastic_integer test failed");

    static_assert(digits<set_digits_t<elastic_integer<15, uint8_t>, 22>>::value == 22, "cnl::elastic_integer test failed");

    static_assert(identical(
            set_digits_t<elastic_integer<15, uint8_t>, 22>{10000},
            elastic_integer<22, uint8_t>{10000}), "cnl::elastic_integer test failed");

    static_assert(identical(
            elastic_fixed_point<2, 2>{1.5} << 1,
            elastic_fixed_point<2, 2>{3}), "cnl::elastic_fixed_point test failed");

    static_assert(identical(
            elastic_fixed_point<2, 2>{1.5} >> 1,
            elastic_fixed_point<2, 2>{0.75}), "cnl::elastic_fixed_point test failed");
}

////////////////////////////////////////////////////////////////////////////////
// useful constants

template<class T, bool IsSigned>
struct test_traits {
    static_assert(cnl::numeric_limits<T>::is_signed==IsSigned,
                  "cnl::numeric_limits<T>::is_signed fails for give type, T");
    static_assert(is_signed<T>::value==IsSigned,
                  "cnl::make_signed failed cnl::numeric_limits test");
    static_assert(is_signed<cnl::make_signed_t<T>>::value,
                  "cnl::make_signed failed cnl::numeric_limits test");
    static_assert(!is_signed<cnl::make_unsigned_t<T>>::value,
                  "cnl::make_unsigned failed cnl::numeric_limits test");
};

template
struct test_traits<cnl::uint8, false>;

template
struct test_traits<cnl::uint16, false>;

template
struct test_traits<cnl::uint32, false>;

template
struct test_traits<cnl::uint64, false>;

template
struct test_traits<cnl::int8, true>;

template
struct test_traits<cnl::int16, true>;

template
struct test_traits<cnl::int32, true>;

template
struct test_traits<cnl::int64, true>;

////////////////////////////////////////////////////////////////////////////////
// useful functions

template<typename T>
constexpr int bit_count(T const& n)
{
    return n ? 1+bit_count(n & (n-1)) : 0;
}

static_assert(bit_count(0)==0, "bit_count test failed");
static_assert(bit_count(1)==1, "bit_count test failed");
static_assert(bit_count(2)==1, "bit_count test failed");
static_assert(bit_count(3)==2, "bit_count test failed");
static_assert(bit_count(5)==2, "bit_count test failed");
static_assert(bit_count(8)==1, "bit_count test failed");
static_assert(bit_count(13)==3, "bit_count test failed");
static_assert(bit_count(21)==3, "bit_count test failed");
static_assert(bit_count(34)==2, "bit_count test failed");
static_assert(bit_count(55)==5, "bit_count test failed");
static_assert(bit_count(89)==4, "bit_count test failed");
static_assert(bit_count(144)==2, "bit_count test failed");

// http://stackoverflow.com/a/5775825/671509
template<size_t size>
struct print_num_as_error {
    operator char() { return size+256; }
}; //always overflow

namespace test_elastic_constant_literal {
    using cnl::_impl::identical;
    static_assert(identical(0_elastic, elastic_fixed_point<1, 0>{0}), "");

    static_assert(identical(1_elastic, elastic_fixed_point<1, 0>{1}), "");
    static_assert(identical(-1_elastic, elastic_fixed_point<1, 0>{-1}), "");

    static_assert(identical(2_elastic, elastic_fixed_point<1, 1>{2}), "");
    static_assert(identical(-2_elastic, elastic_fixed_point<1, 1>{-2}), "");

    static_assert(identical(3_elastic, elastic_fixed_point<2, 0>{3}), "");
    static_assert(identical(-3_elastic, elastic_fixed_point<2, 0>{-3}), "");

    static_assert(identical(4_elastic, elastic_fixed_point<1, 2>{4}), "");
    static_assert(identical(-4_elastic, elastic_fixed_point<1, 2>{-4}), "");

    static_assert(identical(6_elastic, elastic_fixed_point<2, 1>{6}), "");
    static_assert(identical(-6_elastic, elastic_fixed_point<2, 1>{-6}), "");

    static_assert(identical(0xAA_elastic, elastic_fixed_point<7, 1>{0xaa}), "");
    static_assert(identical(-0xaa_elastic, elastic_fixed_point<7, 1>{-0xaa}), "");

    static_assert(identical(897341888_elastic, elastic_fixed_point<24, 6>{897341888}), "");
    static_assert(identical(-897341888_elastic, elastic_fixed_point<24, 6>{-897341888}), "");
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic_fixed_point handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test
        : number_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using rep = typename elastic_type::rep;
    using numeric_limits = cnl::numeric_limits<elastic_type>;

    using signed_type = cnl::make_signed_t<elastic_type>;
    using unsigned_type = cnl::make_unsigned_t<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};
    static constexpr elastic_type negative_zero{-zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    static constexpr int integer_digits = cnl::_impl::integer_digits<elastic_type>::value;
    static constexpr int fractional_digits = cnl::_impl::fractional_digits<elastic_type>::value;
    static constexpr int digits = integer_digits+fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(cnl::numeric_limits<elastic_type>::is_signed==cnl::numeric_limits<rep>::is_signed,
                  "signedness of elastic_fixed_point type differs from underlying fixed-point type");
    static_assert(cnl::numeric_limits<signed_type>::is_signed,
                  "signed version of elastic_fixed_point type is not signed");
    static_assert(!cnl::numeric_limits<unsigned_type>::is_signed,
                  "unsigned version of elastic_fixed_point type is signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test elastic_integer type

    static_assert(rep::digits>=digits,
                  "not enough digits in rep type to represent elastic_fixed_point values");

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::numeric_limits<elastic_fixed_point>

    static_assert(min==elastic_type::from_data(rep{1}), "cnl::numeric_limits test failed");
    static_assert(!is_less_than(max, min), "cnl::numeric_limits test failed");
    static_assert(is_less_than(zero, min), "cnl::numeric_limits test failed");
    static_assert(!is_less_than(zero, lowest), "cnl::numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "cnl::numeric_limits test failed");
    static_assert(cnl::numeric_limits<elastic_type>::is_signed==numeric_limits::is_signed,
                  "cnl::numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || (elastic_type{.5} != .5), "cnl::numeric_limits test failed");

    static constexpr rep max_integer{max.data()};
    static_assert(bit_count<typename rep::rep>(max_integer.data())==digits, "cnl::numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between zero and negative zero
    static_assert(is_equal_to(zero, negative_zero), "comparison test error");

    // zero vs min
    static_assert(is_less_than<elastic_type>(zero, min), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test negate operators

    using negate_result = decltype(-zero);
    static_assert(
            cnl::_impl::integer_digits<negate_result>::value==cnl::_impl::integer_digits<elastic_type>::value,
            "negative of positive value has wrong number of integer digits");
    static_assert(
            cnl::_impl::fractional_digits<negate_result>::value==cnl::_impl::fractional_digits<elastic_type>::value,
            "negative of positive value has wrong number of fractional digits");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(cnl::numeric_limits<decltype(zero+zero)>::is_signed
                  ==cnl::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during add");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}+unsigned_type{zero})>::is_signed,
                  "signedness is lost during add");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_less_than(max-min, max), "operator- test failed");

    static_assert(cnl::numeric_limits<decltype(zero-zero)>::is_signed,
                  "signedness is lost during subtract");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}-unsigned_type{zero})>::is_signed,
                  "signedness is lost during subtract");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator*

    static_assert(is_equal_to(min*make_elastic_fixed_point(0_c), zero), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_fixed_point(1_c), min), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_fixed_point(2_c), min+min), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_fixed_point(3_c), min+min+min), "operator* test failed");

    static_assert(cnl::numeric_limits<decltype(zero*zero)>::is_signed
                  ==cnl::numeric_limits<decltype(zero)>::is_signed,
                  "signedness is lost during multiply");
    static_assert(cnl::numeric_limits<decltype(zero*zero)>::is_signed
                  ==cnl::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}*unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator/

    static_assert(!is_less_than(min, min/make_elastic_fixed_point(2_c)), "operator/ test failed");
    static_assert(is_equal_to(min/make_elastic_fixed_point(1_c), min), "operator/ test failed");
    static_assert(is_equal_to((min+min)/make_elastic_fixed_point(2_c), min), "operator/ test failed");
    static_assert(is_equal_to((min+min+min)/make_elastic_fixed_point(3_c), min), "operator/ test failed");
    static_assert(cnl::numeric_limits<decltype(zero/zero)>::is_signed
                  ==cnl::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(cnl::numeric_limits<decltype(signed_type{zero}/unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic_fixed_point handles negative values;
// should pass for all signed specializations

template<class Elastic>
struct signed_elastic_test :
        // test type traits given this type is signed
        test_traits<Elastic, true>,

        // perform positive value tests against signed elastic_fixed_point specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using rep = typename elastic_type::rep;
    using numeric_limits = cnl::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};
    static constexpr elastic_type negative_min{-min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    // not much point testing negative value properties of unsigned type, eh?
    static_assert(cnl::numeric_limits<elastic_type>::is_signed, "subject of test class is not reported as signed");
    static_assert(is_same<cnl::make_signed_t<elastic_type>, elastic_type>::value,
                  "subject of test class is not reported as signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::numeric_limits<elastic_fixed_point>

    static_assert(is_less_than(negative_min, min), "cnl::numeric_limits test failed");
    static_assert(is_equal_to(-max, lowest), "comparison test error");
    //static_assert(is_equal_to(elastic_type{min+max+lowest}, elastic_type{1}), "comparison test error");
    static_assert(numeric_limits::is_signed, "cnl::numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || elastic_type{-.5} != -.5, "cnl::numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between negative_min
    static_assert(is_equal_to(negative_min, negative_min), "comparison test error");

    // min vs negative_min
    static_assert(is_less_than<elastic_type>(negative_min, min), "comparison test error");

    // negative_min vs zero
    static_assert(is_less_than<elastic_type>(negative_min, zero), "comparison test error");

    // negative_min vs lowest
    static_assert(numeric_limits::is_signed
                  ? !is_less_than(negative_min, lowest)
                  : is_less_than(negative_min, lowest), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(is_equal_to(min+max+lowest, min), "operator+ test failed");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic_fixed_point handles positive values;
// should pass for all unsigned specializations

template<class Elastic>
struct unsigned_elastic_test :
        // test type traits given this type is not signed
        test_traits<Elastic, false>,

        // perform positive value tests against unsigned elastic_fixed_point specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using numeric_limits = cnl::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    ////////////////////////////////////////////////////////////////////////////////
    // test cnl::numeric_limits<elastic_fixed_point>

    static_assert(is_equal_to(lowest, zero), "cnl::numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "cnl::numeric_limits test failed");
};

////////////////////////////////////////////////////////////////////////////////
// given values for IntegerDigits and FractionalDigits parameters,
// triggers elastic_fixed_point<> tests with signed and unsigned specializations

template<int IntegerDigits, int FractionalDigits>
struct elastic_test :
        // perform unsigned-specific value tests against unsigned elastic_fixed_point specialization
        unsigned_elastic_test<elastic_fixed_point<IntegerDigits+FractionalDigits, -FractionalDigits, unsigned>>,

        // perform negative value tests against signed elastic_fixed_point specialization
        signed_elastic_test<elastic_fixed_point<IntegerDigits+FractionalDigits, -FractionalDigits, signed>> {
};

////////////////////////////////////////////////////////////////////////////////
// given a value for IntegerDigits parameter,
// triggers elastic_fixed_point<> tests against a range of values for FractionalDigits parameter

template<int IntegerDigits>
struct elastic_test_with_integer_digits
        : elastic_test<IntegerDigits, -IntegerDigits+1>
                , elastic_test<IntegerDigits, -IntegerDigits+2>
                , elastic_test<IntegerDigits, -IntegerDigits+31>
{
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic_fixed_point tests against a range of values for IntegerDigits parameter

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<-2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<-16>;

template
struct elastic_test_with_integer_digits<63>;
