
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_fixed_point.h>

////////////////////////////////////////////////////////////////////////////////
// cnl::make_elastic_fixed_point

using cnl::make_elastic_fixed_point;
using cnl::_impl::identical;
using namespace cnl::literals;
using cnl::elastic_fixed_point;

static constexpr auto int_digits = cnl::numeric_limits<int>::digits;

static_assert(identical(make_elastic_fixed_point(cnl::constant<-1>{}), cnl::elastic_fixed_point<1, 0, int>{-1}),
              "using too many bytes to represent -1");
static_assert(identical(make_elastic_fixed_point(-1_c), cnl::elastic_fixed_point<1, 0, int>{-1}), "make_elastic_fixed_point");

static_assert(identical(make_elastic_fixed_point(123), elastic_fixed_point<int_digits>{123}), "cnl::make_elastic_fixed_point test failed");
static_assert(
        identical(elastic_fixed_point<32, 0, unsigned>{123U}, make_elastic_fixed_point(123U)),
        "cnl::make_elastic_fixed_point test failed");
static_assert(
        identical(make_elastic_fixed_point(0x100000000_c), elastic_fixed_point<1, 32>{0x100000000ULL}),
        "cnl::make_elastic_fixed_point test failed");
static_assert(
        identical(
                make_elastic_fixed_point<cnl::uint8>(262143),
                elastic_fixed_point<cnl::numeric_limits<decltype(262143)>::digits, 0, cnl::uint8>{262143}),
        "cnl::make_elastic_fixed_point test failed");

static_assert(identical(make_elastic_fixed_point(cnl::constant<4>{}), elastic_fixed_point<1, 2>{4}), "");
static_assert(identical(make_elastic_fixed_point(40_c), elastic_fixed_point<3, 3>{40}), "");
static_assert(identical(make_elastic_fixed_point(0x123400000000_c), elastic_fixed_point<11, 34>{0x123400000000}), "");
static_assert(identical(make_elastic_fixed_point<cnl::int8>(9876543), elastic_fixed_point<31, 0, cnl::int8>{9876543}), "");

namespace test_from_fixed_point {
    static_assert(
            identical(
                    cnl::elastic_fixed_point<16, -8, unsigned>{255.99609375},
                    cnl::make_elastic_fixed_point(cnl::fixed_point<std::uint16_t, -8>{255.99609375})),
            "");

    static_assert(
            identical(
                    cnl::elastic_fixed_point<cnl::digits<long>::value, 3, signed>{88},
                    cnl::make_elastic_fixed_point(cnl::fixed_point<long, 3>{88})),
            "");
}

////////////////////////////////////////////////////////////////////////////////
// tests size of cnl::make_elastic_fixed_point

// by default, make_elastic_fixed_point generates types that are machine-optimal for speed...
static_assert(sizeof(make_elastic_fixed_point(0_c)) <= sizeof(int), "using too many bytes to represent 0");
static_assert(sizeof(make_elastic_fixed_point(1_c)) == sizeof(int), "using too many bytes to represent 1");
static_assert(sizeof(make_elastic_fixed_point(255_c)) == sizeof(int), "using too many bytes to represent 255");
static_assert(sizeof(make_elastic_fixed_point(256_c)) == sizeof(int), "using too many bytes to represent 256");
static_assert(sizeof(make_elastic_fixed_point(257_c)) == sizeof(int), "using too many bytes to represent 257");
static_assert(sizeof(make_elastic_fixed_point(510_c)) == sizeof(int), "using too many bytes to represent 510");
static_assert(sizeof(make_elastic_fixed_point(511_c)) == sizeof(int), "using too many bytes to represent 511");
static_assert(sizeof(make_elastic_fixed_point(512_c)) == sizeof(int), "using too many bytes to represent 512");

static_assert(sizeof(make_elastic_fixed_point(-1_c)) == sizeof(int), "using too many bytes to represent -1");
static_assert(sizeof(make_elastic_fixed_point(-127_c)) == sizeof(int), "using too many bytes to represent -127");
static_assert(sizeof(make_elastic_fixed_point(-128_c)) == sizeof(int), "using too many bytes to represent -128");
static_assert(sizeof(make_elastic_fixed_point(-129_c)) == sizeof(int), "using too many bytes to represent -129");
static_assert(sizeof(make_elastic_fixed_point(-254_c)) == sizeof(int), "using too many bytes to represent -254");
static_assert(sizeof(make_elastic_fixed_point(-255_c)) == sizeof(int), "using too many bytes to represent -255");
static_assert(sizeof(make_elastic_fixed_point(-256_c)) == sizeof(int), "using too many bytes to represent -256");

// ... but a more compact type can be chosen if size is the constraint
static_assert(sizeof(make_elastic_fixed_point<signed char>(0_c)) == 1, "using too many bytes to represent 0");
static_assert(sizeof(make_elastic_fixed_point<signed char>(1_c)) == 1, "using too many bytes to represent 1");
static_assert(sizeof(make_elastic_fixed_point<signed char>(255_c)) == 2, "using too many bytes to represent 255");
static_assert(sizeof(make_elastic_fixed_point<signed char>(256_c)) == 1, "using too many bytes to represent 256");
static_assert(sizeof(make_elastic_fixed_point<signed char>(257_c)) == 2, "using too many bytes to represent 257");
static_assert(sizeof(make_elastic_fixed_point<signed char>(510_c)) == 2, "using too many bytes to represent 510");
static_assert(sizeof(make_elastic_fixed_point<signed char>(511_c)) == 2, "using too many bytes to represent 511");
static_assert(sizeof(make_elastic_fixed_point<signed char>(512_c)) == 1, "using too many bytes to represent 512");

static_assert(sizeof(make_elastic_fixed_point<signed char>(-1_c)) == 1, "using too many bytes to represent -1");
static_assert(sizeof(make_elastic_fixed_point<signed char>(-127_c)) == 1, "using too many bytes to represent -127");
static_assert(sizeof(make_elastic_fixed_point<signed char>(-128_c)) == 1, "using too many bytes to represent -128");
static_assert(sizeof(make_elastic_fixed_point<signed char>(-129_c)) == 2, "using too many bytes to represent -129");
static_assert(sizeof(make_elastic_fixed_point<signed char>(-254_c)) == 1, "using too many bytes to represent -254");
static_assert(sizeof(make_elastic_fixed_point<signed char>(-255_c)) == 2, "using too many bytes to represent -255");
static_assert(sizeof(make_elastic_fixed_point<signed char>(-256_c)) == 1, "using too many bytes to represent -256");

// some numbers are so big that you don't have the luxury of choosing
static constexpr auto unsigned_limit = cnl::intmax{cnl::numeric_limits<unsigned>::max()} + 1;
static_assert(
        sizeof(make_elastic_fixed_point(cnl::constant<unsigned_limit>())) == sizeof(int),
        "using too many bytes to represent 2^32");
static_assert(
        sizeof(make_elastic_fixed_point<char>(cnl::constant<unsigned_limit>{})) == sizeof(char),
        "using too many bytes to represent 2^32");

static_assert(
        sizeof(make_elastic_fixed_point<char>(cnl::constant<unsigned_limit + 1>{})) > sizeof(int),
        "using too many bytes to represent 2^32 + 1");
static_assert(
        sizeof(make_elastic_fixed_point<char>(cnl::constant<unsigned_limit + 1>{})) > sizeof(int),
        "using too many bytes to represent 2^32 + 1");

////////////////////////////////////////////////////////////////////////////////
// tests for cnl::make_elastic_fixed_point

template<cnl::int64 Value>
struct make_elastic_test {
    static constexpr auto value = cnl::constant<Value>{};
    static constexpr auto elastic_value = make_elastic_fixed_point(value);
    using type = typename std::remove_const<decltype(elastic_value)>::type;

    static_assert(Value<=0 || (Value/(1LL << cnl::_impl::integer_digits<type>::value))==0, "elastic_fixed_point type capacity is too big");
    static_assert(Value>=0 || cnl::_impl::integer_digits<type>::value>=60
                  || (Value/(1LL << (cnl::_impl::integer_digits<type>::value+1)))==0,
                  "elastic_fixed_point type capacity is too big");
    static_assert(Value<=0 || (Value >> cnl::_impl::max<int>(0, cnl::_impl::integer_digits<type>::value-1))!=0, "elastic_fixed_point type capacity is too small");
    static_assert(Value>=0 || (Value >> (cnl::_impl::integer_digits<type>::value))!=0, "elastic_fixed_point type capacity is too small");
    static_assert(Value || cnl::_impl::integer_digits<type>::value==0, "elastic_fixed_point type capacity is too small");

    static constexpr int lsz = 1 << (-cnl::_impl::fractional_digits<type>::value);
    static_assert(Value==((Value/lsz)*lsz), "fractional_digits is too low");

    static constexpr int lsz1 = lsz * 2;
    static_assert(Value==0 || Value!=((Value/lsz1)*lsz1), "fractional_digits is too high");

    static_assert(cnl::numeric_limits<type>::is_signed, "signage doesn't match value");
//    static_assert(elastic_value==elastic_fixed_point<63, 0>{Value}, "make_elasticd value doesn't equal its source value");
};

template
struct make_elastic_test<1>;
template
struct make_elastic_test<-1>;
template
struct make_elastic_test<2>;
template
struct make_elastic_test<-2>;
template
struct make_elastic_test<3>;
template
struct make_elastic_test<-3>;
template
struct make_elastic_test<-13>;
template
struct make_elastic_test<169>;
template
struct make_elastic_test<-2197>;
template
struct make_elastic_test<28561>;
template
struct make_elastic_test<-371293>;
template
struct make_elastic_test<4826809>;
template
struct make_elastic_test<-62748517>;
template
struct make_elastic_test<815730721>;
template
struct make_elastic_test<-10604499373>;
template
struct make_elastic_test<137858491849>;
template
struct make_elastic_test<cnl::numeric_limits<cnl::int64>::max()/2>;
template
struct make_elastic_test<-cnl::numeric_limits<cnl::int64>::max()/2>;
#if ! defined(_MSC_VER)
template
struct make_elastic_test<cnl::numeric_limits<cnl::int64>::max()>;
template
struct make_elastic_test<-cnl::numeric_limits<cnl::int64>::max()>;
#endif
