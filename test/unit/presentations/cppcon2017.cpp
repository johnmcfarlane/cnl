
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/fixed_point.h>
#include <cnl/fraction.h>

#include <cstddef>
#include <iostream>
#include <unordered_map>

#include <filesystem>
namespace filesystem = std::filesystem;

using cnl::fixed_point;

template<>
struct std::hash<filesystem::path> {
    size_t operator()(filesystem::path const& p) const
    {
        return filesystem::hash_value(p);
    }
};

namespace a {
    using std::unique_ptr;
    using std::unordered_map;
    using byte = std::uint8_t;

    unordered_map<filesystem::path, unique_ptr<byte[]>> cache;  // NOLINT(cppcoreguidelines-avoid-c-arrays)
}

namespace b {
    void f() {
        auto n = fixed_point<int, -8>{ 0.25F };
        std::cout << float(n * 5); // prints "1.25"
    }
}

namespace c {
    bool foo(float f) {
        auto fixed = fixed_point<int, -16>{f};
        auto fixed_plus_one = fixed + 1;
        return fixed_plus_one > fixed;
    }

    bool foo_reduced(float) {
        return true;
    }
}

namespace d {
    using std::numeric_limits;

    float bar() {
        return static_cast<float>(std::numeric_limits<fixed_point<int, -16>>::max() + 1);
    }

    static_assert(1 == 1, "this does compile");
    //static_assert(1 != 1, "this does not compile");
    //static_assert(1 << 1000, "this does not compile");

    static_assert(numeric_limits<fixed_point<int, -16>>::max() - 1, "this does compile");
    //static_assert(numeric_limits<fixed_point<int, -16>>::max() + 1, "this does not compile!");
}

namespace e {
    static_assert(fixed_point<unsigned>{1U} < fixed_point<signed>{-1}, "OK(!)");

#if defined(CNL_P1021)
    static_assert(fixed_point{1u} < fixed_point{-1});
#endif
}

namespace f {
    //static_assert(numeric_limits<int>::max()+1, "error");

    static_assert(unsigned{1}<signed{-1}, "evaluates to true");
}

namespace g {
    auto n = fixed_point<int, -8>{1.5};
    auto nn = n * n;

    static_assert(std::is_same<decltype(nn), fixed_point<int, -16>>::value);
}

namespace h {
    constexpr auto n = fixed_point<int, -8>{1.5};
    constexpr auto d = fixed_point<int, -8>{2.25};
    constexpr auto q = n / d;    // fixed_point<int, 0>;

    static_assert(std::is_same<decltype(q), const fixed_point<int, 0>>::value);
    static_assert(q == 0);
}

namespace i {
    constexpr auto n = fixed_point<int, -8>{1.5};
    constexpr auto d = fixed_point<int, -8>{2.25};
    constexpr auto q = cnl::quotient(n, d);

    static_assert(std::is_same<decltype(q), const fixed_point<cnl::int64, -31>>::value);
    static_assert(q == 0.66666666651144623756408691);
}

namespace j {
    constexpr auto n = fixed_point<uint8_t, -8>{0.99609375};
    constexpr auto nn = n * n;    // fixed_point<int, -16>{0.9922027587890625};
    static_assert(cnl::_impl::identical(fixed_point<int, -16>{0.9922027587890625}, nn), "");
}

namespace k {
    void f() {
        auto n = fixed_point<int, -31>{0.99609375};
        auto nn = n*n;    // fixed_point<int, -62>{0.9922027587890625};
        static_assert(std::is_same<fixed_point<int, -62>, decltype(nn)>::value, "");
        (void)nn;
    }
}

#include <cnl/elastic_integer.h>
using cnl::elastic_integer;
namespace l {
    constexpr auto e = elastic_integer<31>{0x7FFFFFFF};   // r has 31 or more digits

    constexpr auto ee = e * e;
    static_assert(cnl::_impl::identical(elastic_integer<62>{INT64_C(0x3FFFFFFF00000001)}, ee), "");

    constexpr auto _2ee = ee + ee;
    static_assert(cnl::_impl::identical(elastic_integer<63>{INT64_C(0x7FFFFFFE00000002)}, _2ee), "");
}

namespace m {
    constexpr auto fpe = fixed_point<elastic_integer<31>, -31>{0.99609375};

    constexpr auto sq = fpe * fpe;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<62>, -62>{0.9922027587890625}, sq), "");

#if defined(CNL_INT128_ENABLED)
    constexpr auto q = make_fixed_point(make_fraction(sq, sq));
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<124>, -62>{1}, q), "");
#endif
}

#include <cnl/overflow_integer.h>
using cnl::overflow_integer;
namespace n {
    constexpr auto i = overflow_integer<uint8_t>{255};

    constexpr auto j = i + 1;
    static_assert(cnl::_impl::identical(overflow_integer<int>{256}, j));

//    constexpr overflow<uint8_t> k = i + 1;
//    static_assert(cnl::_impl::identical(overflow<int>{256}, k));
}

#if defined(CNL_P1021) && !defined(_MSC_VER)
#include <cnl/elastic_fixed_point.h>

using namespace cnl::literals;
namespace o {
    constexpr auto x = cnl::fixed_point{42ul}; // fixed_point<unsigned long, 0>{42}
    static_assert(cnl::_impl::identical(fixed_point<unsigned long, 0>{42}, x));

    constexpr auto z = cnl::fixed_point{128_c};
    static_assert(cnl::_impl::identical(fixed_point<int, 7>{128}, z));

    constexpr auto a = cnl::fixed_point{0b10000000000000000000000000000000000000000_c};
    static_assert(cnl::_impl::identical(fixed_point<int, 40>{0b10000000000000000000000000000000000000000L}, a));

    constexpr auto b = cnl::fixed_point{0b11111111111111111111111111111111111111111_c};
    static_assert(cnl::_impl::identical(fixed_point<cnl::int64, 0>{0b11111111111111111111111111111111111111111L}, b));

    constexpr auto c = elastic_integer{2017_c};
    static_assert(cnl::_impl::identical(elastic_integer<11>{2017}, c));

    constexpr auto e = 0x7f000_elastic;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<7>, 12>{0x7f000}, e));

    constexpr auto s = e >> 1_c;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<7>, 11>{0x3f800}, s));
}
#endif
