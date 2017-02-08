
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of sg14::elastic alias

#include <sg14/auxiliary/elastic.h>

using std::is_same;

using sg14::elastic;
using sg14::make_elastic;
using sg14::make_signed;
using sg14::make_unsigned;

////////////////////////////////////////////////////////////////////////////////
// fast tests of sg14::elastic<> at its limits;
// if something broke it may show up here first

namespace {
    using sg14::_impl::fp::arithmetic::intermediate;
    using sg14::_impl::fp::arithmetic::result;
    using sg14::_impl::shift_left;
    using sg14::_impl::fp::arithmetic::wide_tag;
    using sg14::_impl::divide_tag;
    using sg14::_impl::identical;
    using sg14::_impl::multiply_tag;
    using sg14::elastic_integer;
    using sg14::fixed_point;
    using sg14::set_width_t;
    using sg14::width;

    static_assert(identical(
            elastic_integer<15, int>{0x7fff}*elastic_integer<16, unsigned>{0xffff},
            elastic_integer<31, int>{0x7FFE8001}), "sg14::elastic_integer test failed");
    static_assert(identical(
            elastic_integer<31, int>{0x7fffffff}*elastic_integer<32, int>{0xffffffffu},
            elastic_integer<63, int>{0x7FFFFFFE80000001LL}), "sg14::elastic_integer test failed");

    static_assert(shift_left<1, sg14::elastic_integer<31, int>>(0) == 0, "");
    static_assert(shift_left<1, sg14::elastic_integer<32, unsigned>>(0) == 0u, "");
    static_assert(shift_left<1, sg14::elastic_integer<63, int>>(0) == 0, "");
    static_assert(shift_left<1, sg14::elastic_integer<64, unsigned>>(0) == 0u, "");

    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<15, int>, 0>, fixed_point<elastic_integer<15, int>, 0>>::type{0},
            fixed_point<elastic_integer<30, int>, -15>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<16, unsigned>, 0>, fixed_point<elastic_integer<16, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<32, unsigned>, -16>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<31, int>, 0>, fixed_point<elastic_integer<31, int>, 0>>::type{0},
            fixed_point<elastic_integer<62, int>, -31>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_tag, fixed_point<elastic_integer<32, unsigned>, 0>, fixed_point<elastic_integer<32, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<64, unsigned>, -32>{0}), "sg14::elastic_integer test failed");

    static_assert(identical(
            intermediate<wide_tag, multiply_tag, fixed_point<elastic_integer<27, unsigned int>, -27>, fixed_point<elastic_integer<27, unsigned int>, -27>>::lhs_type{0},
            fixed_point<elastic_integer<27, unsigned int>, -27>{0}), "sg14::elastic_integer test failed");

    static_assert(width<set_width_t<elastic_integer<15, uint8_t>, 22>>::value == 22, "sg14::elastic_integer test failed");

    static_assert(identical(
            set_width_t<elastic_integer<15, uint8_t>, 22>{10000},
            elastic_integer<22, uint8_t>{10000}), "sg14::elastic_integer test failed");
}

////////////////////////////////////////////////////////////////////////////////
// useful constants

template<class T, bool IsSigned>
struct test_traits {
    static_assert(std::numeric_limits<T>::is_signed==IsSigned,
                  "std::numeric_limits<T>::is_signed fails for give type, T");
    static_assert(std::numeric_limits<typename sg14::make_signed<T>::type>::is_signed,
                  "sg14::make_signed failed std::numeric_limits test; please reboot");
    static_assert(!std::numeric_limits<typename sg14::make_unsigned<T>::type>::is_signed,
                  "sg14::make_unsigned failed std::numeric_limits test; please reboot");
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
// useful functions

template<typename T>
constexpr int bit_count(const T& n)
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

////////////////////////////////////////////////////////////////////////////////
// test specific operations

// Lhs == Rhs
template<class Lhs, class Rhs>
constexpr bool is_equal_to(const Lhs& lhs, const Rhs& rhs)
{
    return ((lhs==rhs)==true)
           && ((lhs!=rhs)==false)
           && ((lhs<rhs)==false)
           && ((lhs>rhs)==false)
           && ((lhs<=rhs)==true)
           && ((lhs>=rhs)==true);
}

static_assert(is_equal_to<int>(0, 0), "less_than_test test failed");

// lesser < greater
template<class Lesser, class Greater>
constexpr bool is_less_than(const Lesser& lesser, const Greater& greater)
{
    return ((lesser==greater)==false)
           && ((lesser!=greater)==true)
           && ((lesser<greater)==true)
           && ((lesser>greater)==false)
           && ((lesser<=greater)==true)
           && ((lesser>=greater)==false);
}

static_assert(is_less_than<int>(0, 1), "less_than_test test failed");

// greater > lesser
template<class Greater, class Lesser>
constexpr bool is_greater_than(const Greater& greater, const Lesser& lesser)
{
    return ((greater==lesser)==false)
           && ((greater!=lesser)==true)
           && ((greater<lesser)==false)
           && ((greater>lesser)==true)
           && ((greater<=lesser)==false)
           && ((greater>=lesser)==true);
}

static_assert(is_less_than<int>(0, 1), "less_than_test test failed");

namespace test_elastic_constant_literal {
    using namespace sg14::literals;
    using sg14::_impl::identical;
    static_assert(identical(0_elastic, elastic<0, 0>{0}), "");
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using rep = typename elastic_type::rep;
    using numeric_limits = std::numeric_limits<elastic_type>;

    using signed_type = typename make_signed<elastic_type>::type;
    using unsigned_type = typename make_unsigned<elastic_type>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr rep rep_zero{0.};
    static constexpr elastic_type zero{0.};
    static constexpr elastic_type negative_zero{-zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    static constexpr int integer_digits = elastic_type::integer_digits;
    static constexpr int fractional_digits = elastic_type::fractional_digits;
    static constexpr int digits = integer_digits+fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(std::numeric_limits<elastic_type>::is_signed==std::numeric_limits<rep>::is_signed,
                  "signedness of elastic type differns from underlying fixed-point type");
    static_assert(std::numeric_limits<typename sg14::make_signed<elastic_type>::type>::is_signed,
                  "signed version of elastic type is not signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test elastic_integer type

    static_assert(rep::digits>=digits,
                  "not enough digits in rep type to represent elastic values");

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic>

    static_assert(min==elastic_type::from_data(rep{1}), "numeric_limits test failed");
    static_assert(!is_less_than(max, min), "numeric_limits test failed");
    static_assert(is_greater_than(min, zero), "numeric_limits test failed");
    static_assert(!is_greater_than(lowest, zero), "numeric_limits test failed");
    static_assert(is_greater_than(min, lowest), "numeric_limits test failed");
    static_assert(std::numeric_limits<elastic_type>::is_signed==numeric_limits::is_signed,
                  "numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || (elastic_type{.5} != .5), "numeric_limits test failed");

    static constexpr rep max_integer{max.data()};
    static_assert(bit_count<typename rep::rep>(max_integer.data())==digits, "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between zero
    static_assert(is_equal_to(zero, zero), "comparison test error");

    // comparisons between zero and literal zero
    static_assert(is_equal_to(zero, 0.), "comparison test error");

    // comparisons between zero and zero-initialized value
    static_assert(is_equal_to(zero, elastic_type{0.}), "zero-initialized value is not represented using zero");

    // comparisons between zero and negative zero
    static_assert(is_equal_to(zero, negative_zero), "comparison test error");

    // comparisons between minimum value
    static_assert(is_equal_to(min, min), "comparison test error");

    // zero vs min
    static_assert(is_less_than<elastic_type>(zero, min), "comparison test error");

    // min vs zero
    static_assert(is_greater_than<elastic_type>(min, zero), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test negate operators

    using negate_result = decltype(-zero);
    static_assert(
            negate_result::integer_digits==elastic_type::integer_digits,
            "negative of positive value has wrong number of integer digits");
    static_assert(
            negate_result::fractional_digits==elastic_type::fractional_digits,
            "negative of positive value has wrong number of fractional digits");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(zero+zero==zero, "operator+ test failed");
    static_assert(zero+zero+zero==zero, "operator+ test failed");
    static_assert(std::numeric_limits<decltype(zero+zero)>::is_signed
                  ==std::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during add");
    static_assert(std::numeric_limits<decltype(signed_type{zero}+unsigned_type{zero})>::is_signed,
                  "signedness is lost during add");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_equal_to(zero-zero, zero), "operator- test failed");
    static_assert(is_equal_to(zero-zero-zero, zero), "operator- test failed");

    static_assert(is_equal_to(min-min, zero), "operator- test failed");
    static_assert(is_equal_to(min-zero, min), "operator- test failed");

    static_assert(is_equal_to(max-max, zero), "operator- test failed");
    static_assert(is_less_than(max-min, max), "operator- test failed");

    static_assert(std::numeric_limits<decltype(zero-zero)>::is_signed,
                  "signedness is lost during subtract");
    static_assert(std::numeric_limits<decltype(signed_type{zero}-unsigned_type{zero})>::is_signed,
                  "signedness is lost during subtract");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator*

    static_assert(is_equal_to(min*make_elastic<0>(), zero), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic<1>(), min), "operator* test failed");
#if ! defined(_MSC_VER)
    static_assert(is_equal_to(min*make_elastic<2>(), min+min), "operator* test failed");
#endif
    static_assert(is_equal_to(min*make_elastic<3>(), min+min+min), "operator* test failed");

    static_assert(std::numeric_limits<decltype(zero*zero)>::is_signed
                  ==std::numeric_limits<decltype(zero)>::is_signed,
                  "signedness is lost during multiply");
    static_assert(std::numeric_limits<decltype(zero*zero)>::is_signed
                  ==std::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(std::numeric_limits<decltype(signed_type{zero}*unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator/

#if ! defined(_MSC_VER)
    static_assert(!is_greater_than(min/make_elastic<2>(), min), "operator/ test failed");
#endif
    static_assert(is_equal_to(min/make_elastic<1>(), min), "operator/ test failed");
    static_assert(is_equal_to((min+min)/make_elastic<2>(), min), "operator/ test failed");
    static_assert(is_equal_to((min+min+min)/make_elastic<3>(), min), "operator/ test failed");
    static_assert(std::numeric_limits<decltype(zero/zero)>::is_signed
                  ==std::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(std::numeric_limits<decltype(signed_type{zero}/unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles negative values;
// should pass for all signed specializations

template<class Elastic>
struct signed_elastic_test :
        // test type traits given this type is signed
        test_traits<Elastic, true>,

        // perform positive value tests against signed elastic specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using rep = typename elastic_type::rep;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr rep rep_zero{0.};
    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};
    static constexpr elastic_type negative_min{-min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    // not much point testing negative value properties of unsigned type, eh?
    static_assert(std::numeric_limits<elastic_type>::is_signed, "subject of test class is not reported as signed");
    static_assert(is_same<typename sg14::make_signed<elastic_type>::type, elastic_type>::value,
                  "subject of test class is not reported as signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic>

    static_assert(is_greater_than(min, negative_min), "numeric_limits test failed");
#if ! defined(_MSC_VER)
    static_assert(is_greater_than(-max, lowest), "comparison test error");
#endif
    static_assert(is_equal_to(elastic_type{min+max+lowest}, zero), "comparison test error");
    static_assert(numeric_limits::is_signed, "numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || elastic_type{-.5} != -.5, "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between negative_min
    static_assert(is_equal_to(negative_min, negative_min), "comparison test error");

    // min vs negative_min
    static_assert(is_greater_than<elastic_type>(min, negative_min), "comparison test error");

    // negative_min vs zero
    static_assert(is_less_than<elastic_type>(negative_min, zero), "comparison test error");

    // negative_min vs lowest
    static_assert(is_greater_than(negative_min, lowest), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(is_equal_to(min+max+lowest, elastic_type{0.}), "operator+ test failed");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles positive values;
// should pass for all unsigned specializations

template<class Elastic>
struct unsigned_elastic_test :
        // test type traits given this type is not signed
        test_traits<Elastic, false>,

        // perform positive value tests against unsigned elastic specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic>

    static_assert(is_equal_to(lowest, zero), "numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "numeric_limits test failed");
};

////////////////////////////////////////////////////////////////////////////////
// given values for IntegerDigits and FractionalDigits parameters,
// triggers elastic<> tests with signed and unsigned specializations

template<int IntegerDigits, int FractionalDigits>
struct elastic_test :
        // perform unsigned-specific value tests against unsigned elastic specialization
        unsigned_elastic_test<elastic<IntegerDigits, FractionalDigits, unsigned>>,

        // perform negative value tests against signed elastic specialization
        signed_elastic_test<elastic<IntegerDigits, FractionalDigits, signed>> {
};

////////////////////////////////////////////////////////////////////////////////
// given a value for IntegerDigits parameter,
// triggers elastic<> tests against a range of values for FractionalDigits parameter

template<int IntegerDigits>
struct elastic_test_with_integer_digits
        : elastic_test<IntegerDigits, -IntegerDigits+1>
                , elastic_test<IntegerDigits, -IntegerDigits+2>
                , elastic_test<IntegerDigits, -IntegerDigits+3>
                , elastic_test<IntegerDigits, -IntegerDigits+7>
                , elastic_test<IntegerDigits, -IntegerDigits+8>
                , elastic_test<IntegerDigits, -IntegerDigits+12>
                , elastic_test<IntegerDigits, -IntegerDigits+16>
                , elastic_test<IntegerDigits, -IntegerDigits+27>
                , elastic_test<IntegerDigits, -IntegerDigits+31>
{
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic tests against a range of values for IntegerDigits parameter

template
struct elastic_test_with_integer_digits<-43>;

template
struct elastic_test_with_integer_digits<-31>;

template
struct elastic_test_with_integer_digits<-19>;

template
struct elastic_test_with_integer_digits<-5>;

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
struct elastic_test_with_integer_digits<8>;

template
struct elastic_test_with_integer_digits<13>;

template
struct elastic_test_with_integer_digits<16>;

template
struct elastic_test_with_integer_digits<19>;

template
struct elastic_test_with_integer_digits<31>;
