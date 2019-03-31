
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/scaled_integer.h>
#include <cnl/fraction.h>
#include <cnl/_impl/type_traits/identical.h>

#include <cstddef>
#include <iostream>
#include <unordered_map>

#if (__cplusplus >= 201703L)
#include <experimental/filesystem>
#endif

using cnl::scaled_integer;

#if (__cplusplus >= 201703L)
template<>
struct std::hash<std::experimental::filesystem::path> {
    size_t operator()(std::experimental::filesystem::path const& p) const
    {
        return std::experimental::filesystem::hash_value(p);
    }
};

namespace a {
    using std::unique_ptr;
    using std::unordered_map;
    using namespace std::experimental;
    using byte = std::uint8_t;

    unordered_map<filesystem::path, unique_ptr<byte[]>> cache;
}
#endif

namespace b {
    void f() {
        auto n = cnl::scaled_integer<int, -8>{ 0.25f };
        std::cout << n * 5; // prints "1.25"
    }
}

namespace c {
    bool foo(float f) {
        auto fixed = scaled_integer<int, -16>{f};
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
        return static_cast<float>(std::numeric_limits<scaled_integer<int, -16>>::max() + 1);
    }

    static_assert(1 == 1, "this does compile");
    //static_assert(1 != 1, "this does not compile");
    //static_assert(1 << 1000, "this does not compile");

    static_assert(numeric_limits<scaled_integer<int, -16>>::max() - 1, "this does compile");
    //static_assert(numeric_limits<scaled_integer<int, -16>>::max() + 1, "this does not compile!");
}

namespace e {
    static_assert(scaled_integer<unsigned>{1u} < scaled_integer<signed>{-1}, "OK(!)");

#if defined(__cpp_deduction_guides)
    static_assert(scaled_integer{1u} < scaled_integer{-1});
#endif
}

namespace f {
    //static_assert(numeric_limits<int>::max()+1, "error");

    static_assert(unsigned{1}<signed{-1}, "evaluates to true");
}

#if (__cplusplus >= 201703L)
namespace g {
    auto n = scaled_integer<int, -8>{1.5};
    auto nn = n * n;    // scaled_integer<int, -16>;

    static_assert(std::is_same<decltype(nn), scaled_integer<int, -16>>::value);
}

namespace h {
    constexpr auto n = scaled_integer<int, -8>{1.5};
    constexpr auto d = scaled_integer<int, -8>{2.25};
    constexpr auto q = n / d;    // scaled_integer<int, 0>;

    static_assert(std::is_same<decltype(q), const scaled_integer<int, 0>>::value);
    static_assert(q == 0);
}

namespace i {
    constexpr auto n = scaled_integer<int, -8>{1.5};
    constexpr auto d = scaled_integer<int, -8>{2.25};
    constexpr auto q = cnl::quotient(n, d);

    static_assert(std::is_same<decltype(q), const scaled_integer<cnl::int64, -31>>::value);
    static_assert(q == 0.66666666651144623756408691);
}
#endif

namespace j {
    constexpr auto n = scaled_integer<uint8_t, -8>{0.99609375};
    constexpr auto nn = n * n;    // scaled_integer<int, -16>{0.9922027587890625};
    static_assert(cnl::_impl::identical(scaled_integer<int, -16>{0.9922027587890625}, nn), "");
}

namespace j2 {
    using cnl::fraction;
    using cnl::make_fraction;

    constexpr auto n = scaled_integer<int16_t, -8>{1.5};
    constexpr auto d = scaled_integer<int16_t, -8>{2.25};
#if defined(__cpp_deduction_guides)
    constexpr auto f = fraction{n, d};
    constexpr auto q = scaled_integer{f};
#else
    constexpr auto f = cnl::make_fraction(n, d);
    constexpr auto q = cnl::make_scaled_integer(f);
#endif
    static_assert(identical(scaled_integer<int32_t, -15>{.66666667}, q), "");
}

namespace k {
    void f() {
        auto n = scaled_integer<int, -31>{0.99609375};
        auto nn = n*n;    // scaled_integer<int, -62>{0.9922027587890625};
        static_assert(std::is_same<scaled_integer<int, -62>, decltype(nn)>::value, "");
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
    constexpr auto fpe = scaled_integer<elastic_integer<31>, -31>{0.99609375};

    constexpr auto sq = fpe * fpe;
    static_assert(cnl::_impl::identical(scaled_integer<elastic_integer<62>, -62>{0.9922027587890625}, sq), "");

#if defined(CNL_INT128_ENABLED)
    constexpr auto q = make_scaled_integer(make_fraction(sq, sq));
    static_assert(cnl::_impl::identical(scaled_integer<elastic_integer<124>, -62>{1}, q), "");
#endif
}

#if (__cplusplus >= 201703L)
#include <cnl/overflow_integer.h>
using cnl::overflow_integer;
namespace n {
    constexpr auto i = overflow_integer<uint8_t>{255};

    constexpr auto j = i + 1;
    static_assert(cnl::_impl::identical(overflow_integer<int>{256}, j));

//    constexpr overflow<uint8_t> k = i + 1;
//    static_assert(cnl::_impl::identical(overflow<int>{256}, k));
}
#endif

#if defined(__cpp_deduction_guides) && !defined(_MSC_VER)
#include <cnl/elastic_scaled_integer.h>

using namespace cnl::literals;
namespace o {
    constexpr auto x = scaled_integer{42ul}; // scaled_integer<unsigned long, 0>{42}
    static_assert(cnl::_impl::identical(scaled_integer<unsigned long, 0>{42}, x));

    constexpr auto z = scaled_integer{128_c};
    static_assert(cnl::_impl::identical(scaled_integer<int, 7>{128}, z));

    constexpr auto a = scaled_integer{0b10000000000000000000000000000000000000000_c};
    static_assert(cnl::_impl::identical(scaled_integer<int, 40>{0b10000000000000000000000000000000000000000l}, a));

    constexpr auto b = scaled_integer{0b11111111111111111111111111111111111111111_c};
    static_assert(cnl::_impl::identical(scaled_integer<cnl::int64, 0>{0b11111111111111111111111111111111111111111l}, b));

    constexpr auto c = elastic_integer{2018_c};
    static_assert(cnl::_impl::identical(elastic_integer<11>{2018}, c));

    constexpr auto e = 0x7f000_elastic;
    static_assert(cnl::_impl::identical(scaled_integer<elastic_integer<7>, 12>{0x7f000}, e));

    constexpr auto s = e >> 1_c;
    static_assert(cnl::_impl::identical(scaled_integer<elastic_integer<7>, 11>{0x3f800}, s));
}
#endif
