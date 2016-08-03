
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief like fixed_point_common.h this file contains tests which can be
/// performed on different types from different .cpp files.

#if !defined(SG14_ELASTIC_COMMON_H)
#define SG14_ELASTIC_COMMON_H 1

#include <sg14/auxiliary/elastic.h>

#include <sg14/bits/int128.h>

#include <tuple>

using std::is_same;

using sg14::elastic;
using sg14::elasticate;
using sg14::make_signed;
using sg14::make_unsigned;

////////////////////////////////////////////////////////////////////////////////
// useful constants

template<class T, bool IsSigned>
struct test_traits {
    static_assert(sg14::is_signed<T>::value==IsSigned, "sg14::is_signed fails for give type, T");
    static_assert(sg14::is_unsigned<T>::value!=IsSigned, "sg14::is_signed fails for give type, T");

    static_assert(sg14::is_signed<typename sg14::make_signed<T>::type>::value,
            "sg14::make_signed failed sg14::is_signed test; please reboot");
    static_assert(!sg14::is_unsigned<typename sg14::make_signed<T>::type>::value,
            "sg14::make_signed failed sg14::is_unsigned test; please reboot");

    static_assert(!sg14::is_signed<typename sg14::make_unsigned<T>::type>::value,
            "sg14::make_unsigned failed sg14::is_signed test; please reboot");
    static_assert(sg14::is_unsigned<typename sg14::make_unsigned<T>::type>::value,
            "sg14::make_unsigned failed sg14::is_unsigned test; please reboot");
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

////////////////////////////////////////////////////////////////////////////////
// tests for sg14::elasticate

template<std::int64_t Value>
struct elasticate_test {
    using elastic_type = decltype(elasticate<Value>());
    static constexpr elastic_type elastic_value = elasticate<Value>();
//    using x = print_num_as_error<((Value/2)*2)>();

    static_assert(Value==0 || (Value/(1LL << elastic_type::integer_digits))==0, "elastic type capacity is too big");
    static_assert(Value==0 || (Value >> (elastic_type::integer_digits-1))!=0, "elastic type capacity is too small");
    static_assert(Value || elastic_type::integer_digits==1, "elastic type capacity is too small");

    static constexpr int lsz = 1 << (-elastic_type::fractional_digits);
    static_assert(Value==((Value/lsz)*lsz), "fractional_digits is too low");

    static constexpr int lsz1 = lsz * 2;
    static_assert(Value==0 || Value!=((Value/lsz1)*lsz1), "fractional_digits is too high");

    static_assert(sg14::is_signed<elastic_type>::value==(Value<0), "signage doesn't match value");
#if ! defined(_MSC_VER)
    static_assert(elastic_value==elastic<63, 0>{Value}, "elasticated value doesn't equal its source value");
#endif
};

// by default, elasticate generates types that are machine-optimal for speed...
static_assert(sizeof(elasticate<0>()) <= sizeof(int), "using too many bytes to represent 0");
static_assert(sizeof(elasticate<1>()) == sizeof(int), "using too many bytes to represent 1");
static_assert(sizeof(elasticate<255>()) == sizeof(int), "using too many bytes to represent 255");
static_assert(sizeof(elasticate<256>()) == sizeof(int), "using too many bytes to represent 256");
static_assert(sizeof(elasticate<257>()) == sizeof(int), "using too many bytes to represent 257");
static_assert(sizeof(elasticate<510>()) == sizeof(int), "using too many bytes to represent 510");
static_assert(sizeof(elasticate<511>()) == sizeof(int), "using too many bytes to represent 511");
static_assert(sizeof(elasticate<512>()) == sizeof(int), "using too many bytes to represent 512");

static_assert(sizeof(elasticate<-1>()) == sizeof(int), "using too many bytes to represent -1");
static_assert(sizeof(elasticate<-127>()) == sizeof(int), "using too many bytes to represent -127");
static_assert(sizeof(elasticate<-128>()) == sizeof(int), "using too many bytes to represent -128");
static_assert(sizeof(elasticate<-129>()) == sizeof(int), "using too many bytes to represent -129");
static_assert(sizeof(elasticate<-254>()) == sizeof(int), "using too many bytes to represent -254");
static_assert(sizeof(elasticate<-255>()) == sizeof(int), "using too many bytes to represent -255");
static_assert(sizeof(elasticate<-256>()) == sizeof(int), "using too many bytes to represent -256");

// ... but a more compact type can be chosen if size is the constraint
static_assert(sizeof(elasticate<0, char>()) <= 1, "using too many bytes to represent 0");
static_assert(sizeof(elasticate<1, char>()) == 1, "using too many bytes to represent 1");
static_assert(sizeof(elasticate<255, char>()) == 1, "using too many bytes to represent 255");
static_assert(sizeof(elasticate<256, char>()) == 1, "using too many bytes to represent 256");
static_assert(sizeof(elasticate<257, char>()) == 2, "using too many bytes to represent 257");
static_assert(sizeof(elasticate<510, char>()) == 1, "using too many bytes to represent 510");
static_assert(sizeof(elasticate<511, char>()) == 2, "using too many bytes to represent 511");
static_assert(sizeof(elasticate<512, char>()) == 1, "using too many bytes to represent 512");

static_assert(sizeof(elasticate<-1, char>()) == 1, "using too many bytes to represent -1");
static_assert(sizeof(elasticate<-127, char>()) == 1, "using too many bytes to represent -127");
static_assert(sizeof(elasticate<-128, char>()) == 1, "using too many bytes to represent -128");
static_assert(sizeof(elasticate<-129, char>()) == 2, "using too many bytes to represent -129");
static_assert(sizeof(elasticate<-254, char>()) == 1, "using too many bytes to represent -254");
static_assert(sizeof(elasticate<-255, char>()) == 2, "using too many bytes to represent -255");
static_assert(sizeof(elasticate<-256, char>()) == 1, "using too many bytes to represent -256");

// some numbers are so big that you don't have the luxury of choosing
static_assert(sizeof(elasticate<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 1>()) == sizeof(int), "using too many bytes to represent 2^32");
static_assert(sizeof(elasticate<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 1, char>()) == 1, "using too many bytes to represent 2^32");

static_assert(sizeof(elasticate<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 2>()) > sizeof(int), "using too many bytes to represent 2^32 + 1");
static_assert(sizeof(elasticate<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 2, char>()) > sizeof(int), "using too many bytes to represent 2^32 + 1");

template
struct elasticate_test<-0>;
template
struct elasticate_test<1>;
template
struct elasticate_test<-1>;
template
struct elasticate_test<2>;
template
struct elasticate_test<-2>;
template
struct elasticate_test<3>;
template
struct elasticate_test<-3>;
template
struct elasticate_test<-13>;
template
struct elasticate_test<169>;
template
struct elasticate_test<-2197>;
template
struct elasticate_test<28561>;
template
struct elasticate_test<-371293>;
template
struct elasticate_test<4826809>;
template
struct elasticate_test<-62748517>;
template
struct elasticate_test<815730721>;
template
struct elasticate_test<-10604499373>;
template
struct elasticate_test<137858491849>;
template
struct elasticate_test<std::numeric_limits<std::int64_t>::max()>;
template
struct elasticate_test<-std::numeric_limits<std::int64_t>::max()>;

////////////////////////////////////////////////////////////////////////////////
// test how elastic handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using fixed_point_type = typename elastic_type::_fixed_point_type;
    using numeric_limits = std::numeric_limits<elastic_type>;

    using signed_type = typename make_signed<elastic_type>::type;
    using unsigned_type = typename make_unsigned<elastic_type>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr fixed_point_type fixed_point_zero{0.};
    static constexpr elastic_type zero{fixed_point_zero};
    static constexpr elastic_type negative_zero{-zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    static constexpr int integer_digits = elastic_type::integer_digits;
    static constexpr int fractional_digits = elastic_type::fractional_digits;
    static constexpr int digits = integer_digits+fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(sg14::is_elastic<elastic_type>::value, "sg14::is_elastic test failed");
    static_assert(!sg14::is_elastic<fixed_point_type>::value, "sg14::is_elastic test failed");
    static_assert(!sg14::is_elastic<typename fixed_point_type::rep>::value, "sg14::is_elastic test failed");

    static_assert(sg14::is_signed<elastic_type>::value==sg14::is_signed<fixed_point_type>::value,
            "signedness of elastic type differns from underlying fixed-point type");

    static_assert(sg14::is_signed<typename sg14::make_signed<elastic_type>::type>::value,
            "signed version of elastic type is not signed");
    static_assert(sg14::is_unsigned<typename sg14::make_unsigned<elastic_type>::type>::value,
            "signed version of elastic type is not signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test fixed-point type

    static_assert(fixed_point_type::integer_digits>=integer_digits,
            "not enough integer digits in fixed-point type to represent elastic values");
    static_assert(fixed_point_type::fractional_digits>=fractional_digits,
            "not enough fractional digits in fixed-point type to represent elastic values");
    static_assert(fixed_point_type::fractional_digits==fractional_digits,
            "too many fractional digits in fixed-point type to represent elastic values accurately");

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic>

    static_assert(min==elastic_type{fixed_point_type::from_data(1)}, "numeric_limits test failed");
    static_assert(!is_less_than(max, min), "numeric_limits test failed");
    static_assert(is_greater_than(min, zero), "numeric_limits test failed");
    static_assert(!is_greater_than(lowest, zero), "numeric_limits test failed");
    static_assert(is_greater_than(min, lowest), "numeric_limits test failed");
    static_assert(sg14::is_signed<elastic_type>::value==numeric_limits::is_signed, "numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || (elastic_type{.5} != .5), "numeric_limits test failed");

    static constexpr typename elastic_type::_integer_type max_integer{max._data().data()};
    static_assert(bit_count(max_integer)==digits, "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between zero
    static_assert(is_equal_to(zero, zero), "comparison test error");

    // comparisons between zero and literal zero
    static_assert(is_equal_to(zero, 0.), "comparison test error");

    // comparisons between zero and default-initialized value
    static_assert(is_equal_to(zero, elastic_type()), "default-initialized value is not represented using zero");

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
    static_assert(sg14::is_signed<negate_result>::value, "negative of positive value is not signed");
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
    static_assert(sg14::is_signed<decltype(zero+zero)>::value
                    ==sg14::is_signed<elastic_type>::value,
            "signedness is lost during add");
    static_assert(sg14::is_signed<decltype(signed_type{zero}+unsigned_type{zero})>::value,
            "signedness is lost during add");
    static_assert(is_same<
                    typename sg14::_elastic_impl::add_result_type<
                            integer_digits, fractional_digits, typename elastic_type::archetype,
                            integer_digits, fractional_digits, typename elastic_type::archetype>,
                    elastic<integer_digits+1, fractional_digits, typename elastic_type::archetype>>::value,
            "sg14::_elastic_impl::add_result_type test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_equal_to(zero-zero, zero), "operator- test failed");
    static_assert(is_equal_to(zero-zero-zero, zero), "operator- test failed");

    static_assert(is_equal_to(min-min, zero), "operator- test failed");
    static_assert(is_equal_to(min-zero, min), "operator- test failed");

    static_assert(is_equal_to(max-max, zero), "operator- test failed");
    static_assert(is_less_than(max-min, max), "operator- test failed");

    static_assert(sg14::is_signed<decltype(zero-zero)>::value,
            "signedness is lost during subtract");
    static_assert(sg14::is_signed<decltype(signed_type{zero}-unsigned_type{zero})>::value,
            "signedness is lost during subtract");
    static_assert(is_same<
                    typename sg14::_elastic_impl::subtract_result_type<
                            integer_digits, fractional_digits, typename elastic_type::archetype,
                            integer_digits, fractional_digits, typename elastic_type::archetype>,
                    elastic<
                            integer_digits+1,
                            fractional_digits,
                            typename sg14::make_signed<typename elastic_type::archetype>::type>>::value,
            "sg14::_elastic_impl::add_result_type test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator*

    static_assert(is_equal_to(min*elasticate<0>(), zero), "operator* test failed");
    static_assert(is_equal_to(min*elasticate<1>(), min), "operator* test failed");
    static_assert(is_equal_to(min*elasticate<2>(), min+min), "operator* test failed");
    static_assert(is_equal_to(min*elasticate<3>(), min+min+min), "operator* test failed");
    static_assert(sg14::is_signed<decltype(zero*zero)>::value
                    ==sg14::is_signed<elastic_type>::value,
            "signedness is lost during multiply");
    static_assert(sg14::is_signed<decltype(signed_type{zero}*unsigned_type{zero})>::value,
            "signedness is lost during multiply");
    static_assert(is_same<
                    typename sg14::_elastic_impl::multiply_result_type<
                            integer_digits, fractional_digits, typename elastic_type::archetype,
                            integer_digits, fractional_digits, typename elastic_type::archetype>,
                    elastic<integer_digits*2, fractional_digits*2, typename elastic_type::archetype>>::value,
            "sg14::_elastic_impl::multiply_result_type test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator/

    static_assert(!is_greater_than(min/elasticate<2>(), min), "operator/ test failed");
    static_assert(is_equal_to(min/elasticate<1>(), min), "operator/ test failed");
    static_assert(is_equal_to((min+min)/elasticate<2>(), min), "operator/ test failed");
    static_assert(is_equal_to((min+min+min)/elasticate<3>(), min), "operator/ test failed");
    static_assert(sg14::is_signed<decltype(zero/zero)>::value
                    ==sg14::is_signed<elastic_type>::value,
            "signedness is lost during multiply");
    static_assert(sg14::is_signed<decltype(signed_type{zero}/unsigned_type{zero})>::value,
            "signedness is lost during multiply");
    static_assert(is_same<
                    typename sg14::_elastic_impl::divide_result_type<
                            integer_digits, fractional_digits, typename elastic_type::archetype,
                            integer_digits, fractional_digits, typename elastic_type::archetype>,
                    elastic<integer_digits+fractional_digits, integer_digits+fractional_digits+1, typename elastic_type::archetype>>::value,
            "sg14::_elastic_impl::divide_result_type test failed");
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
    using fixed_point_type = typename elastic_type::_fixed_point_type;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr fixed_point_type fixed_point_zero{0.};
    static constexpr elastic_type zero{fixed_point_zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};
    static constexpr elastic_type negative_min{-min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    // not much point testing negative value properties of unsigned type, eh?
    static_assert(sg14::is_signed<elastic_type>::value, "subject of test class is not reported as signed");
    static_assert(is_same<typename sg14::make_signed<elastic_type>::type, elastic_type>::value,
            "subject of test class is not reported as signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic>

    static_assert(is_equal_to(negative_min, elastic_type{fixed_point_type::from_data(-1)}),
            "numeric_limits test failed");
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
#if defined(SG14_INT128_ENABLED)
        , elastic_test<IntegerDigits, -IntegerDigits+31>
        , elastic_test<IntegerDigits, -IntegerDigits+39>
        , elastic_test<IntegerDigits, -IntegerDigits+55>
#endif
        {
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic tests against a range of values for IntegerDigits parameter

// (from elastic?.cpp source files)

#endif  // defined(SG14_ELASTIC_COMMON_H)
