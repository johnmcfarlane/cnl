
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/elastic.h>

using sg14::make_elastic;

////////////////////////////////////////////////////////////////////////////////
// tests size of sg14::make_elastic

// by default, make_elastic generates types that are machine-optimal for speed...
static_assert(sizeof(make_elastic<0>()) <= sizeof(int), "using too many bytes to represent 0");
static_assert(sizeof(make_elastic<1>()) == sizeof(int), "using too many bytes to represent 1");
static_assert(sizeof(make_elastic<255>()) == sizeof(int), "using too many bytes to represent 255");
static_assert(sizeof(make_elastic<256>()) == sizeof(int), "using too many bytes to represent 256");
static_assert(sizeof(make_elastic<257>()) == sizeof(int), "using too many bytes to represent 257");
static_assert(sizeof(make_elastic<510>()) == sizeof(int), "using too many bytes to represent 510");
static_assert(sizeof(make_elastic<511>()) == sizeof(int), "using too many bytes to represent 511");
static_assert(sizeof(make_elastic<512>()) == sizeof(int), "using too many bytes to represent 512");

static_assert(sizeof(make_elastic<-1>()) == sizeof(int), "using too many bytes to represent -1");
static_assert(sizeof(make_elastic<-127>()) == sizeof(int), "using too many bytes to represent -127");
static_assert(sizeof(make_elastic<-128>()) == sizeof(int), "using too many bytes to represent -128");
static_assert(sizeof(make_elastic<-129>()) == sizeof(int), "using too many bytes to represent -129");
static_assert(sizeof(make_elastic<-254>()) == sizeof(int), "using too many bytes to represent -254");
static_assert(sizeof(make_elastic<-255>()) == sizeof(int), "using too many bytes to represent -255");
static_assert(sizeof(make_elastic<-256>()) == sizeof(int), "using too many bytes to represent -256");

// ... but a more compact type can be chosen if size is the constraint
static_assert(sizeof(make_elastic<0, char>()) <= 1, "using too many bytes to represent 0");
static_assert(sizeof(make_elastic<1, char>()) == 1, "using too many bytes to represent 1");
static_assert(sizeof(make_elastic<255, char>()) == 2, "using too many bytes to represent 255");
static_assert(sizeof(make_elastic<256, char>()) == 1, "using too many bytes to represent 256");
static_assert(sizeof(make_elastic<257, char>()) == 2, "using too many bytes to represent 257");
static_assert(sizeof(make_elastic<510, char>()) == 2, "using too many bytes to represent 510");
static_assert(sizeof(make_elastic<511, char>()) == 2, "using too many bytes to represent 511");
static_assert(sizeof(make_elastic<512, char>()) == 1, "using too many bytes to represent 512");

static_assert(sizeof(make_elastic<-1, char>()) == 1, "using too many bytes to represent -1");
static_assert(sizeof(make_elastic<-127, char>()) == 1, "using too many bytes to represent -127");
static_assert(sizeof(make_elastic<-128, char>()) == 1, "using too many bytes to represent -128");
static_assert(sizeof(make_elastic<-129, char>()) == 2, "using too many bytes to represent -129");
static_assert(sizeof(make_elastic<-254, char>()) == 1, "using too many bytes to represent -254");
static_assert(sizeof(make_elastic<-255, char>()) == 2, "using too many bytes to represent -255");
static_assert(sizeof(make_elastic<-256, char>()) == 1, "using too many bytes to represent -256");

// some numbers are so big that you don't have the luxury of choosing
static_assert(sizeof(make_elastic<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 1>()) == sizeof(int), "using too many bytes to represent 2^32");
static_assert(sizeof(make_elastic<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 1, char>()) == 1, "using too many bytes to represent 2^32");

static_assert(sizeof(make_elastic<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 2>()) > sizeof(int), "using too many bytes to represent 2^32 + 1");
static_assert(sizeof(make_elastic<static_cast<long long>(std::numeric_limits<unsigned>::max()) + 2, char>()) > sizeof(int), "using too many bytes to represent 2^32 + 1");

////////////////////////////////////////////////////////////////////////////////
// tests for sg14::make_elastic

template<std::int64_t Value>
struct make_elastic_test {
    using elastic_type = decltype(make_elastic<Value>());
    static constexpr elastic_type elastic_value = make_elastic<Value>();

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
