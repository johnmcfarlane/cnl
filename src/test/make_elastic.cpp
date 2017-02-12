
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/elastic.h>

////////////////////////////////////////////////////////////////////////////////
// sg14::make_elastic

using sg14::make_elastic;
using sg14::_impl::identical;
using namespace sg14::literals;
using sg14::const_integer;
using sg14::elastic;

static constexpr auto int_digits = std::numeric_limits<int>::digits;

static_assert(identical(make_elastic(const_integer<std::int64_t, -1>{}), sg14::elastic<1, 0, int>{-1}),
              "using too many bytes to represent -1");
#if ! defined(_MSC_VER)
static_assert(identical(make_elastic(-1_c), sg14::elastic<1, 0, int>{-1}), "using too many bits to represent -1");
#endif

static_assert(identical(make_elastic(123), elastic<int_digits>{123}), "sg14::make_elastic test failed");
static_assert(
        identical(make_elastic(0x100000000_c), elastic<33, -32>{0x100000000ULL}),
        "sg14::make_elastic test failed");
static_assert(
        identical(
                make_elastic<std::uint8_t>(262143),
                elastic<std::numeric_limits<decltype(262143)>::digits, 0, std::uint8_t>{262143}),
        "sg14::make_elastic test failed");

static_assert(identical(make_elastic(const_integer<int, 4>{}), elastic<3, -2>{4}), "");
static_assert(identical(make_elastic(40_c), elastic<6, -3>{40}), "");
static_assert(identical(make_elastic(0x123400000000_c), elastic<45, -34>{0x123400000000}), "");
static_assert(identical(make_elastic<std::int8_t>(9876543), elastic<31, 0, std::int8_t>{9876543}), "");

////////////////////////////////////////////////////////////////////////////////
// tests size of sg14::make_elastic

// by default, make_elastic generates types that are machine-optimal for speed...
static_assert(sizeof(make_elastic(0_c)) <= sizeof(int), "using too many bytes to represent 0");
static_assert(sizeof(make_elastic(1_c)) == sizeof(int), "using too many bytes to represent 1");
static_assert(sizeof(make_elastic(255_c)) == sizeof(int), "using too many bytes to represent 255");
static_assert(sizeof(make_elastic(256_c)) == sizeof(int), "using too many bytes to represent 256");
static_assert(sizeof(make_elastic(257_c)) == sizeof(int), "using too many bytes to represent 257");
static_assert(sizeof(make_elastic(510_c)) == sizeof(int), "using too many bytes to represent 510");
static_assert(sizeof(make_elastic(511_c)) == sizeof(int), "using too many bytes to represent 511");
static_assert(sizeof(make_elastic(512_c)) == sizeof(int), "using too many bytes to represent 512");

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
static_assert(sizeof(make_elastic(-1_c)) == sizeof(int), "using too many bytes to represent -1");
static_assert(sizeof(make_elastic(-127_c)) == sizeof(int), "using too many bytes to represent -127");
static_assert(sizeof(make_elastic(-128_c)) == sizeof(int), "using too many bytes to represent -128");
static_assert(sizeof(make_elastic(-129_c)) == sizeof(int), "using too many bytes to represent -129");
static_assert(sizeof(make_elastic(-254_c)) == sizeof(int), "using too many bytes to represent -254");
static_assert(sizeof(make_elastic(-255_c)) == sizeof(int), "using too many bytes to represent -255");
static_assert(sizeof(make_elastic(-256_c)) == sizeof(int), "using too many bytes to represent -256");
#endif

// ... but a more compact type can be chosen if size is the constraint
static_assert(sizeof(make_elastic<char>(0_c)) == 1, "using too many bytes to represent 0");
static_assert(sizeof(make_elastic<char>(1_c)) == 1, "using too many bytes to represent 1");
static_assert(sizeof(make_elastic<char>(255_c)) == 2, "using too many bytes to represent 255");
static_assert(sizeof(make_elastic<char>(256_c)) == 1, "using too many bytes to represent 256");
static_assert(sizeof(make_elastic<char>(257_c)) == 2, "using too many bytes to represent 257");
static_assert(sizeof(make_elastic<char>(510_c)) == 2, "using too many bytes to represent 510");
static_assert(sizeof(make_elastic<char>(511_c)) == 2, "using too many bytes to represent 511");
static_assert(sizeof(make_elastic<char>(512_c)) == 1, "using too many bytes to represent 512");

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
static_assert(sizeof(make_elastic<char>(-1_c)) == 1, "using too many bytes to represent -1");
static_assert(sizeof(make_elastic<char>(-127_c)) == 1, "using too many bytes to represent -127");
static_assert(sizeof(make_elastic<char>(-128_c)) == 1, "using too many bytes to represent -128");
static_assert(sizeof(make_elastic<char>(-129_c)) == 2, "using too many bytes to represent -129");
static_assert(sizeof(make_elastic<char>(-254_c)) == 1, "using too many bytes to represent -254");
static_assert(sizeof(make_elastic<char>(-255_c)) == 2, "using too many bytes to represent -255");
static_assert(sizeof(make_elastic<char>(-256_c)) == 1, "using too many bytes to represent -256");
#endif

// some numbers are so big that you don't have the luxury of choosing
static constexpr auto unsigned_limit = std::intmax_t{std::numeric_limits<unsigned>::max()} + 1;
static_assert(
        sizeof(make_elastic(const_integer<std::intmax_t, unsigned_limit>())) == sizeof(int),
        "using too many bytes to represent 2^32");
static_assert(
        sizeof(make_elastic<char>(const_integer<std::intmax_t, unsigned_limit>{})) == sizeof(char),
        "using too many bytes to represent 2^32");

static_assert(
        sizeof(make_elastic<char>(const_integer<std::intmax_t, unsigned_limit + 1>{})) > sizeof(int),
        "using too many bytes to represent 2^32 + 1");
static_assert(
        sizeof(make_elastic<char>(const_integer<std::intmax_t, unsigned_limit + 1>{})) > sizeof(int),
        "using too many bytes to represent 2^32 + 1");

////////////////////////////////////////////////////////////////////////////////
// tests for sg14::make_elastic

template<std::int64_t Value>
struct make_elastic_test {
    static constexpr auto value = const_integer<std::int64_t, Value>{};
    static constexpr auto elastic_value = make_elastic(value);
    using elastic_type = decltype(elastic_value);

    static_assert(Value<=0 || (Value/(1LL << elastic_type::integer_digits))==0, "elastic type capacity is too big");
    static_assert(Value>=0 || elastic_type::integer_digits>=60
                  || (Value/(1LL << (elastic_type::integer_digits+1)))==0,
                  "elastic type capacity is too big");
    static_assert(Value<=0 || (Value >> sg14::_impl::max<int>(0, elastic_type::integer_digits-1))!=0, "elastic type capacity is too small");
    static_assert(Value>=0 || (Value >> (elastic_type::integer_digits))!=0, "elastic type capacity is too small");
    static_assert(Value || elastic_type::integer_digits==0, "elastic type capacity is too small");

    static constexpr int lsz = 1 << (-elastic_type::fractional_digits);
    static_assert(Value==((Value/lsz)*lsz), "fractional_digits is too low");

    static constexpr int lsz1 = lsz * 2;
    static_assert(Value==0 || Value!=((Value/lsz1)*lsz1), "fractional_digits is too high");

    static_assert(std::numeric_limits<elastic_type>::is_signed, "signage doesn't match value");
#if ! defined(_MSC_VER)
//    static_assert(elastic_value==elastic<63, 0>{Value}, "make_elasticd value doesn't equal its source value");
#endif
};

template
struct make_elastic_test<-0>;
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
struct make_elastic_test<std::numeric_limits<std::int64_t>::max()/2>;
template
struct make_elastic_test<-std::numeric_limits<std::int64_t>::max()/2>;
#if ! defined(_MSC_VER)
template
struct make_elastic_test<std::numeric_limits<std::int64_t>::max()>;
template
struct make_elastic_test<-std::numeric_limits<std::int64_t>::max()>;
#endif
