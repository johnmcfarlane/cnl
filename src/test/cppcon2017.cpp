#include <cnl/fixed_point.h>

#include <cstddef>
#include <iostream>
#include <unordered_map>

#if (__cplusplus > 201700)
#include <experimental/filesystem>
#endif

using cnl::fixed_point;

#if (__cplusplus > 201700)
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
        auto n = fixed_point<int, -8>{ 0.25f };
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
    static_assert(fixed_point<unsigned>{1u} < fixed_point<signed>{-1}, "OK(!)");

#if defined(__cpp_deduction_guides)
    static_assert(fixed_point{1u} < fixed_point{-1});
#endif
}

namespace f {
    //static_assert(numeric_limits<int>::max()+1, "error");
    static_assert(unsigned{1}<signed{-1}, "evaluates to true");
}

#if (__cplusplus > 201700)
namespace g {
    auto n = fixed_point<int, -8>{1.5};
    auto nn = n * n;    // fixed_point<int, -16>;

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
    constexpr auto q = cnl::divide(n, d);

    static_assert(std::is_same<decltype(q), const fixed_point<long, -31>>::value);
    static_assert(q == 0.66666666651144623756408691);
}
#endif

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

#include <cnl/auxiliary/elastic_fixed_point.h>
namespace m {
    constexpr auto fpe = fixed_point<elastic_integer<31>, -31>{0.99609375};

    constexpr auto sq = fpe * fpe;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<62>, -62>{0.9922027587890625}, sq), "");

#if defined(CNL_INT128_ENABLED)
    constexpr auto q = sq / sq;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<124>, -62>{1}, q), "");
#endif
}

#if (__cplusplus > 201700)
#include <cnl/safe_integer.h>
using cnl::safe_integer;
namespace n {
    constexpr auto i = safe_integer<uint8_t>{255};

    constexpr auto j = i + 1;
    static_assert(cnl::_impl::identical(safe_integer<int>{256}, j));

//    constexpr safe_integer<uint8_t> k = i + 1;
//    static_assert(cnl::_impl::identical(safe_integer<int>{256}, k));
}
#endif

#if defined(__cpp_deduction_guides)
using namespace cnl::literals;
namespace o {
    constexpr auto x = fixed_point{42ul}; // fixed_point<unsigned long, 0>{42}
    static_assert(cnl::_impl::identical(fixed_point<unsigned long, 0>{42}, x));

    constexpr auto z = fixed_point{128_c};
    static_assert(cnl::_impl::identical(fixed_point<int, 7>{128}, z));

    constexpr auto a = fixed_point{0b10000000000000000000000000000000000000000_c};
    static_assert(cnl::_impl::identical(fixed_point<int, 40>{0b10000000000000000000000000000000000000000l}, a));

    constexpr auto b = fixed_point{0b11111111111111111111111111111111111111111_c};
    static_assert(cnl::_impl::identical(fixed_point<long, 0>{0b11111111111111111111111111111111111111111l}, b));

    constexpr auto c = elastic_integer{2017_c};
    static_assert(cnl::_impl::identical(elastic_integer<11>{2017}, c));

    constexpr auto e = 0x7f000_elastic;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<7>, 12>{0x7f000}, e));

    constexpr auto s = e >> 1_c;
    static_assert(cnl::_impl::identical(fixed_point<elastic_integer<7>, 11>{0x3f800}, s));
}
#endif
